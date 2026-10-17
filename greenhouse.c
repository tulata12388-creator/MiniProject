#include "greenhouse.h"
#include "DMA_LIB.h"        /* g_adc_buf[] extern              */
#include "adc_mgr.h"        /* moving-average filter           */
#include "fire_logic.h"     /* state machine + hysteresis       */
#include "actuators.h"      /* buzzer / motor control           */
#include "SPI_LIB.h"        /* SPI1 TX buffer management        */

/*============================================================
 *  greenhouse.c � Central Logic: ADC ? Alarm ? Actuator ? SPI
 *
 *  Processing chain (called from main loop, deferred from DMA TC):
 *
 *    g_adc_buf[4]  (raw from DMA)
 *         �
 *         ?
 *    ADC_Mgr_FeedSample()      ? push into moving-average
 *         �
 *         +-? ADC_Mgr_GetTempX10()  ? filtered temperature
 *         +-? ADC_Mgr_GetGasRaw()   ? filtered gas ADC
 *                   �
 *                   ?
 *    FireLogic_Update()        ? state machine (hysteresis)
 *         �
 *         +-? FireLogic_GetState()   ? NORMAL/WARN/ALARM
 *         +-? Actuator_SetState()    ? buzzer/motor target
 *         �
 *         ?
 *    build_packet()            ? pack 16-byte SPI frame
 *         �                      (into back-buffer)
 *         ?
 *    SPI1_Slave_SwapBuffer()   ? atomic pointer swap
 *                                Pi reads new frame next poll
 *
 *  DOUBLE-BUFFER STRATEGY:
 *    Two 16-byte buffers: g_spi_buf_a[] and g_spi_buf_b[].
 *    build_packet() always writes to the BACK buffer.
 *    SwapBuffer() switches active ? back pointer only.
 *    g_idx is never reset � it wraps at g_len naturally.
 *
 *    SPI IRQ (prio 1) can preempt the swap, but SwapBuffer
 *    is just two pointer writes � effectively atomic.
 *============================================================*/

/* ----------- Double-buffer SPI TX ----------- */
static volatile uint8_t g_spi_buf_a[PACKET_LEN];
static volatile uint8_t g_spi_buf_b[PACKET_LEN];
// exlain: one for SPI to transfer data 
//         one of buffer for CPU to write data in new packet => avoid ( spi miss, cpu can replay other data, sum, frame
/* Pointer to the buffer currently being WRITTEN (back-buffer).
 * The OTHER buffer is the one SPI is actively reading. */
static volatile uint8_t *g_back = g_spi_buf_b;

/* Public alias for legacy code / header extern */
volatile uint8_t *g_spi_packet_ptr = g_spi_buf_a;

static volatile uint8_t seq = 0;

/*------------------------------------------------------------
 *  build_packet � Pack 16-byte SPI frame (board.h Section 7)
 *
 *  Uses FRAME_OFF_xxx and FRAME_MAGIC_xxx defines from board.h
 *  so the frame layout is defined in exactly ONE place.
 *
 *  Writes into the back-buffer (g_back), NOT the active SPI
 *  buffer.  After build, caller swaps pointers.
 *------------------------------------------------------------*/
static void build_packet(volatile uint8_t *buf,
                         uint8_t status,
                         const uint16_t adc[4],
                         uint16_t temp_x10)
{
    int i;
    uint8_t cs;

    /* Header (protocol magic from board.h) */
    buf[FRAME_OFF_MAGIC0] = FRAME_MAGIC_0;
    buf[FRAME_OFF_MAGIC1] = FRAME_MAGIC_1;
    buf[FRAME_OFF_SEQ]    = seq++;
    buf[FRAME_OFF_STATUS] = status;

    /* 4 � ADC channels, uint16 little-endian */
    buf[FRAME_OFF_ADC0_L] = (uint8_t)(adc[0] & 0xFF);
    buf[FRAME_OFF_ADC0_H] = (uint8_t)(adc[0] >> 8);
    buf[FRAME_OFF_ADC1_L] = (uint8_t)(adc[1] & 0xFF);
    buf[FRAME_OFF_ADC1_H] = (uint8_t)(adc[1] >> 8);
    buf[FRAME_OFF_ADC2_L] = (uint8_t)(adc[2] & 0xFF);
    buf[FRAME_OFF_ADC2_H] = (uint8_t)(adc[2] >> 8);
    buf[FRAME_OFF_ADC3_L] = (uint8_t)(adc[3] & 0xFF);
    buf[FRAME_OFF_ADC3_H] = (uint8_t)(adc[3] >> 8);

    /* Temperature � 10 (0.1�C unit), uint16 little-endian */
    buf[FRAME_OFF_TEMP_L] = (uint8_t)(temp_x10 & 0xFF);
    buf[FRAME_OFF_TEMP_H] = (uint8_t)(temp_x10 >> 8);

    /* XOR checksum over bytes [0..13] */
    cs = 0;
    for (i = 0; i < FRAME_OFF_XOR; i++)
        cs ^= buf[i];
    buf[FRAME_OFF_XOR] = cs;

    /* End-of-frame marker */
    buf[FRAME_OFF_END] = FRAME_END_MARKER;
}

/*------------------------------------------------------------
 *  Greenhouse_InitPacket � Build initial zero-frame
 *
 *  Called once from main() BEFORE ADC starts.
 *  Ensures SPI always has a valid frame for the Pi,
 *  even if Pi polls before the first ADC conversion.
 *------------------------------------------------------------*/
void Greenhouse_InitPacket(void)
{
    uint16_t zeros[4] = {0, 0, 0, 0};

    /* Build into buf_a (will be the initial active buffer) */
    build_packet(g_spi_buf_a, 0, zeros, 0);

    /* Point SPI driver at buf_a */
    SPI1_Slave_SetTxBuffer(g_spi_buf_a, PACKET_LEN);

    /* Back-buffer starts as buf_b */
    g_back = g_spi_buf_b;
}

/*------------------------------------------------------------
 *  Greenhouse_OnAdcReady � Process ADC data (deferred from DMA)
 *
 *  Called from main() loop when DMA TC flag is set.
 *  Runs at thread level � SPI IRQ (prio 1) can preempt.
 *
 *  Steps:
 *    1. Feed raw ADC samples into moving-average filter
 *    2. Read filtered temperature & gas values
 *    3. Update fire-logic state machine (hysteresis)
 *    4. Set actuator target state (buzzer pattern via SysTick)
 *    5. Build STATUS byte using board.h bit positions
 *    6. Collect 4 filtered ADC values for payload
 *    7. Build 16-byte SPI frame into back-buffer
 *    8. Swap active ? back buffer (atomic, Pi reads new frame)
 *------------------------------------------------------------*/
void Greenhouse_OnAdcReady(void)
{
    uint16_t temp_x10;
    uint16_t gas_raw;
    FireState st;
    uint8_t  gas_flag, temp_flag;
    uint8_t  status;
    uint16_t adc[4];
    uint8_t  ch;
    volatile uint8_t *old_back;

    /* 1. Push raw ADC into moving-average filter */
    ADC_Mgr_FeedSample(g_adc_buf);

    /* 2. Read filtered values */
    temp_x10 = ADC_Mgr_GetTempX10();    /* 0.1�C, e.g. 325 = 32.5�C */
    gas_raw  = ADC_Mgr_GetGasRaw();     /* raw ADC 0�4095            */

    /* 3. Update hysteresis state machine */
    FireLogic_Update(temp_x10, gas_raw);
    st = FireLogic_GetState();           /* NORMAL / WARN / ALARM     */

    /* 4. Set actuator target (buzzer pattern runs in SysTick) */
    Actuator_SetState(st);

    /* 5. Build STATUS byte (bit positions from board.h Section 7) */
    gas_flag  = (FireLogic_GetGasState()  >= FIRE_STATE_WARN) ? 1U : 0U;
    temp_flag = (FireLogic_GetTempState() >= FIRE_STATE_WARN) ? 1U : 0U;

    status = 0;
    status |= (Actuator_IsBuzzerOn() ? 1U : 0U) << STATUS_BIT_BUZZER;
    status |= (Actuator_IsMotorOn()  ? 1U : 0U) << STATUS_BIT_MOTOR;
    status |= gas_flag  << STATUS_BIT_GAS_ALARM;
    status |= temp_flag << STATUS_BIT_TEMP_ALARM;

    /* 6. If a previous swap is still pending (SPI mid-frame),
     *    skip packet build.  Otherwise we would write into the
     *    back-buffer that equals the current g_tx � SPI is
     *    reading it!  Sensor/alarm/actuator (steps 1-5) already
     *    ran, so real-time response is unaffected.  The stale
     *    packet is at most one ADC cycle old (~48 �s). */
    if (SPI1_Slave_IsSwapPending())
        return;

    /* 7. Collect 4 filtered ADC values for frame payload */
    for (ch = 0; ch < ADC_NUM_CHANNELS; ch++)
        adc[ch] = ADC_Mgr_GetFiltered(ch);

    /* 8. Build frame into back-buffer */
    build_packet(g_back, status, adc, temp_x10);

    /* 9. Queue swap: ISR will apply at next frame boundary
     *    (g_idx wraps to 0) ? Pi always gets a consistent
     *    frame ? XOR checksum always matches.
     *
     *    BUG FIX v5: Previously SwapBuffer changed g_tx
     *    immediately ? mixed frame ? XOR mismatch. */
    old_back = g_back;
    if (g_back == g_spi_buf_b)
        g_back = g_spi_buf_a;
    else
        g_back = g_spi_buf_b;

    SPI1_Slave_SwapBuffer(old_back, PACKET_LEN);
}
