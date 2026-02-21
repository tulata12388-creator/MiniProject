#include "greenhouse.h"
#include "DMA_LIB.h"
#include "ADC_LIB.h"
#include "actuators.h"
#include "SPI_LIB.h"

volatile uint8_t g_spi_packet[PACKET_LEN];
static volatile uint8_t seq = 0;

static void build_packet(uint8_t status, uint16_t temp_x10)
{
    g_spi_packet[0]  = 0xAA;
    g_spi_packet[1]  = 0x55;
    g_spi_packet[2]  = seq++;
    g_spi_packet[3]  = status;

    g_spi_packet[4]  = (uint8_t)(g_adc_buf[0] & 0xFF);
    g_spi_packet[5]  = (uint8_t)(g_adc_buf[0] >> 8);

    g_spi_packet[6]  = (uint8_t)(g_adc_buf[1] & 0xFF);
    g_spi_packet[7]  = (uint8_t)(g_adc_buf[1] >> 8);

    g_spi_packet[8]  = (uint8_t)(g_adc_buf[2] & 0xFF);
    g_spi_packet[9]  = (uint8_t)(g_adc_buf[2] >> 8);

    g_spi_packet[10] = (uint8_t)(g_adc_buf[3] & 0xFF);
    g_spi_packet[11] = (uint8_t)(g_adc_buf[3] >> 8);

    g_spi_packet[12] = (uint8_t)(temp_x10 & 0xFF);
    g_spi_packet[13] = (uint8_t)(temp_x10 >> 8);

    uint8_t cs = 0;
    for (int i = 0; i <= 13; i++) cs ^= g_spi_packet[i];
    g_spi_packet[14] = cs;
    g_spi_packet[15] = 0x0D;
}

void Greenhouse_InitPacket(void)
{
    build_packet(0, 0);
    SPI1_Slave_SetTxBuffer(g_spi_packet, PACKET_LEN);
}

/* called from DMA IRQ */
void Greenhouse_OnAdcReady(void)
{
    /* LM35: temp_x10 = mV = adc*3300/4095 */
    uint16_t temp_x10 = (uint16_t)(((uint32_t)g_adc_buf[ADC_IDX_LM35] * 3300U) / 4095U);

    uint8_t gas_alarm  = (g_adc_buf[ADC_IDX_GAS] > GAS_ALARM_ADC) ? 1U : 0U;
    uint8_t temp_alarm = (temp_x10 > TEMP_ALARM_X10) ? 1U : 0U;

    /* control */
    Buzzer_Set(gas_alarm);
    Motor_Set(temp_alarm);

    uint8_t status = 0;
    status |= (Buzzer_Get() ? 1U : 0U) << 0;
    status |= (Motor_Get()  ? 1U : 0U) << 1;
    status |= gas_alarm  << 2;
    status |= temp_alarm << 3;

    build_packet(status, temp_x10);

    /* cho Pi d?c frame m?i t? d?u */
    SPI1_Slave_ResetIndex();
}