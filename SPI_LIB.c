#include "SPI_LIB.h"
#include "irq_rm0383.h"
#include "board.h"   /* IRQ_PRIO_SPI, PACKET_LEN */

/*+---------------------------------------------------------------+
 *¦  SPI_LIB.c – SPI1 Slave Driver  (TXE-only, no EXTI)         ¦
 *¦                                                               ¦
 *¦  THIRD REWRITE — 2026-03-04                                   ¦
 *¦                                                               ¦
 *¦  Previous approaches and why they failed:                     ¦
 *¦                                                               ¦
 *¦   v1: RXNE+TXE handlers advancing g_idx independently        ¦
 *¦       ? double-advance race ? duplicate byte[0]              ¦
 *¦                                                               ¦
 *¦   v2: RXNE-only + EXTI4 (NSS edge) preload                   ¦
 *¦       ? EXTI4 fires spuriously (noise / AF5 conflict) ?      ¦
 *¦         preload_first_byte() resets DR to 0xAA between        ¦
 *¦         every byte ? all-0xAA on every read                  ¦
 *¦       ? ALSO: SwapBuffer called preload_first_byte()         ¦
 *¦         from DMA ISR, same symptom                           ¦
 *¦                                                               ¦
 *¦   v3 (THIS): TXE-only, NO EXTI4, self-resync via counter.   ¦
 *¦       TXE fires when the TX buffer is empty and ready for    ¦
 *¦       the next byte.  We simply load g_tx[g_idx++].          ¦
 *¦       Counter wraps at g_len ? frame auto-aligned.           ¦
 *¦       No EXTI, no RXNE, no preload — nothing can reset       ¦
 *¦       g_idx spuriously.                                      ¦
 *¦                                                               ¦
 *¦  Why TXE-only is correct for SPI slave TX:                   ¦
 *¦   • STM32F4 SPI slave: when master clocks a byte, the shift ¦
 *¦     register empties ? TXE=1 ? we must load the next byte.  ¦
 *¦   • We don't care about received data (Pi sends 0x00 dummy). ¦
 *¦   • RXNE still fires, but we handle everything in TXE.       ¦
 *¦   • OVR on RX side is harmless — we never read RX data.     ¦
 *¦     We clear it periodically to prevent any side-effects.    ¦
 *¦                                                               ¦
 *¦  Double-buffer:                                               ¦
 *¦   greenhouse.c builds into back-buffer, then calls            ¦
 *   SwapBuffer() to SET A PENDING pointer.  The actual swap    ¦
 *   happens in the ISR at the frame boundary (g_idx wraps 0)   ¦
 *   ? the Pi always receives a complete, consistent frame      ¦
 *   ? XOR checksum ALWAYS matches.                             ¦
 *                                                               ¦
 *  BUG FIX v5: Previous SwapBuffer changed g_tx immediately,   ¦
 *  causing mid-frame pointer switch ? mixed frame ? XOR fail.  ¦
 *+---------------------------------------------------------------+*/
/* private prototype */
static void SPI1_NSS_EXTI4_Init(void);

/* global varibale ....*/
static volatile uint8_t  *g_tx  = 0;    /* active TX buffer ptr   */
static volatile uint16_t  g_len = 0;    /* buffer length (16)     */
static volatile uint16_t  g_idx = 0;    /* next byte to write     */

/* -- Pending swap (written by thread, consumed by ISR at frame boundary) --
 *   g_tx_pending != 0 means a new buffer is waiting to become active.
 *   The ISR applies it when g_idx wraps to 0 (frame boundary).
 *   g_len_pending is written BEFORE g_tx_pending to avoid a stale-length
 *   race (ISR checks g_tx_pending first).                                */
static volatile uint8_t  *g_tx_pending  = 0;
static volatile uint16_t  g_len_pending = 0;

/*------------------------------------------------------------
 *  SPI1_Slave_SetTxBuffer — point driver at first valid frame
 *
 *  Called once from Greenhouse_InitPacket(), before SPI starts.
 *------------------------------------------------------------*/
void SPI1_Slave_SetTxBuffer(volatile uint8_t *buf, uint16_t len)
{
    g_tx  = buf;
    g_len = len;
    g_idx = 0;
}

/*------------------------------------------------------------
 *  SPI1_Slave_SwapBuffer — deferred double-buffer swap
 *
 *  BUG FIX v5: Does NOT change g_tx immediately.
 *  Instead, stores the new buffer as "pending".
 *  The SPI ISR applies the swap at the next frame boundary
 *  (g_idx wraps from 15 ? 0), so the Pi always receives
 *  a complete, consistent 16-byte frame.
 *
 *  g_len_pending is written FIRST, then g_tx_pending,
 *  because the ISR tests g_tx_pending before reading
 *  g_len_pending.  On Cortex-M4, 32-bit aligned writes
 *  are single-instruction (STR) — effectively atomic.
 *------------------------------------------------------------*/
void SPI1_Slave_SwapBuffer(volatile uint8_t *new_buf, uint16_t len)
{
    g_len_pending = len;        /* length first (ISR reads second) */
    g_tx_pending  = new_buf;    /* pointer last (ISR checks first) */
}

/*------------------------------------------------------------
 *  SPI1_Slave_IsSwapPending — check if a swap is queued
 *
 *  Returns 1 if the previous SwapBuffer() has not yet been
 *  applied by the ISR (SPI is mid-frame).  greenhouse.c
 *  uses this to avoid writing into the buffer that SPI is
 *  currently reading.
 *------------------------------------------------------------*/
int SPI1_Slave_IsSwapPending(void)
{
    return (g_tx_pending != 0) ? 1 : 0;
}

/*------------------------------------------------------------
 *  SPI1_Slave_ResetIndex — reset counter to byte 0
 *------------------------------------------------------------*/
void SPI1_Slave_ResetIndex(void)
{
    g_idx = 0;
    /* Pre-fill DR so the first byte is ready when master clocks */
    if (g_tx && g_len)
        SPI1->DR = g_tx[0];
}

/*------------------------------------------------------------
 *  SPI1_Slave_Init — SPI1 slave, Mode 0, TXE interrupt
 *
 *  Register setup:
 *    CR1 = 0x0000 ? Slave, CPOL=0, CPHA=0, 8-bit, MSB-first
 *          Hardware NSS (SSM=0) — Pi CE0 drives PA4 directly.
 *    CR2 = TXEIE  ? Interrupt when TX buffer is empty
 *
 *  No EXTI on NSS.  Frame alignment relies solely on the
 *  wrapping g_idx counter.  The first TXE after SPI enable
 *  loads byte[0].
 *------------------------------------------------------------*/
void SPI1_Slave_Init(void)
{
    /* -- 1. Disable SPI, all interrupts OFF during setup -- */
    SPI1->CR1 = 0;
    SPI1->CR2 = 0;

    /* -- 2. CR1: Slave, Mode 0, 8-bit, MSB-first, HW NSS -- */
#if SPI_SLAVE_FORCE_SSM
    SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI;
#else
    SPI1->CR1 = 0;   /* MSTR=0, SSM=0 ? hardware NSS on PA4 */
#endif

    /* -- 3. Clear stale data & flags (read DR then SR) -- */
    (void)SPI1->DR;
    (void)SPI1->SR;

    /* -- 4. Pre-fill DR with byte[0] before enabling anything.
     *       Writing DR clears TXE (from its reset-default 1)
     *       ? prevents a spurious interrupt when TXEIE is set. */
    g_idx = 0;
    if (g_tx && g_len)
    {
        SPI1->DR = g_tx[0];
        g_idx = 1;
    }

    /* -- 5. NVIC — clear any stale pending, then enable -- */
    NVIC_SetPriority(SPI1_IRQn, IRQ_PRIO_SPI);
    NVIC_ClearPendingIRQ(SPI1_IRQn);
    NVIC_EnableIRQ(SPI1_IRQn);
    
		/* EXTI4 interupt init */
		SPI1_NSS_EXTI4_Init();
    /* -- 6. Enable SPI peripheral -- */
    SPI1->CR1 |= SPI_CR1_SPE;

    /* -- 7. Enable TXE interrupt LAST --
     *  DR was pre-filled ? TXE = 0 ? no immediate spurious IRQ.
     *  TXE fires only when master clocks the first byte
     *  (TX buffer ? shift register ? TXE = 1 ? ISR responds).
     *
     *  We do NOT enable RXNEIE — we don't need MOSI data.
     *  RX overruns are harmless (cleared in ISR).              */
    SPI1->CR2 = SPI_CR2_TXEIE;
}

/*------------------------------------------------------------
 *  SPI1_IRQHandler — TXE (transmit buffer empty)
 *
 *  Runs at priority 1 (highest — never preempted).
 *
 *  The SPI shift register just finished clocking out a byte
 *  and the TX buffer is empty.  We load the next byte from
 *  g_tx[] and advance g_idx.  When g_idx wraps to 0, the
 *  frame restarts automatically.
 *
 *  Timing analysis:
 *    At 1 MHz SPI, TXE fires every 8 µs (one byte period).
 *    This ISR is ~10 instructions = ~625 ns @ 16 MHz.
 *    Priority 1 = highest ? nothing preempts ? safe margin.
 *
 *  BUG FIX v4: Previously prio 2, preempted by DMA (prio 1).
 *  ADC continuous + DMA circular fires TC every ~48 µs.
 *  At -O0, DMA processing took >48 µs ? DMA ISR re-entered
 *  endlessly ? SPI ISR was starved ? DR never updated.
 *------------------------------------------------------------*/
 static void SPI1_NSS_EXTI4_Init(void)
{
    /* 1. Enable clock for SYSCFG */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    (void)RCC->APB2ENR;   /* dummy read */

    /* 2. Route EXTI4 source to PA4
     * EXTICR[1] controls EXTI4..7
     * EXTI4 uses bits [3:0]
     * PA = 0000
     */
    SYSCFG->EXTICR[1] &= ~(0xFUL << 0);

    /* 3. Configure EXTI4: interrupt mask, falling-edge trigger */
    EXTI->IMR  |= EXTI_LINE4;     /* unmask interrupt */
    EXTI->EMR  &= ~EXTI_LINE4;    /* no event */
    EXTI->RTSR &= ~EXTI_LINE4;    /* disable rising */
    EXTI->FTSR |= EXTI_LINE4;     /* enable falling */

    /* 4. Clear pending before enable NVIC */
    EXTI->PR = EXTI_LINE4;

    /* 5. NVIC */
    NVIC_ClearPendingIRQ(EXTI4_IRQn);
    NVIC_SetPriority(EXTI4_IRQn, EXTI4_IRQ_PRIORITY);
    NVIC_EnableIRQ(EXTI4_IRQn);
}

void SPI1_IRQHandler(void)
{
    uint32_t sr = SPI1->SR;

    if (sr & SPI_SR_TXE)
    {
        if (g_tx && g_len)
        {
            SPI1->DR = g_tx[g_idx];
            g_idx++;
            if (g_idx >= g_len)
            {
                g_idx = 0;

                /* -- Frame boundary: safe to swap buffers --
                 *  We just loaded the last byte (END marker) into DR.
                 *  The next TXE will send byte[0] of the next frame.
                 *  If a pending buffer exists, switch now so the
                 *  entire next frame comes from the new buffer.
                 *
                 *  BUG FIX v5: Previously g_tx was changed mid-frame
                 *  by SwapBuffer() ? mixed frame ? XOR mismatch.
                 *  Now the swap only happens here, at byte[0] boundary.
                 */
                if (g_tx_pending)
                {
                    g_tx  = g_tx_pending;
                    g_len = g_len_pending;
                    g_tx_pending = 0;
                }
            }
        }
        else
        {
            SPI1->DR = 0x00;
        }
    }

    /* Clear RX overrun if present (we never read RX data).
     * OVR clear sequence: read DR, then read SR.            */
    if (sr & SPI_SR_OVR)
    {
        (void)SPI1->DR;
        (void)SPI1->SR;
    }
}

/*------------------------------------------------------------
 *  EXTI4_IRQHandler — DISABLED (stub to avoid Hard Fault)
 *
 *  Previous design used EXTI4 on PA4 (NSS falling edge) to
 *  resync the frame counter.  This caused all-0xAA because:
 *   • Noise on NSS line triggered EXTI4 between bytes
 *   • Each spurious EXTI4 wrote 0xAA to DR and reset g_idx
 *   • Result: every byte the Pi reads is 0xAA
 *
 *  Current design does NOT configure EXTI4 (no IMR, no NVIC).
 *  This stub exists only to prevent a Hard Fault if the EXTI4
 *  vector was somehow pending from a previous firmware load.
 *------------------------------------------------------------*/
void EXTI4_IRQHandler(void)
{
    EXTI->PR = (1U << 4);   /* clear pending, do nothing */
}