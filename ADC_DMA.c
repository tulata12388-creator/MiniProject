#include "RCC_STM32.h"
#include "ADC.h"
#include "DMA.h"
#include "Define.h"
void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < ms; i++)
    {
        for(volatile uint32_t j = 0; j < 16000; j++);
    }
}
extern volatile uint16_t adc_buffer[4];
/* Config DMD for ADC */
void DMA2_Stream0_Config_ADC(uint16_t *buffer, uint16_t length)
{
    /* Enable clock DMA2 */
    CLK_CONTROL->AHB1ENR |= (1U << 22);

    /* Disable stream */
    DMA2_CONTROL->STREAM[0].CR &= ~(1U << 0);
    while (DMA2_CONTROL->STREAM[0].CR & (1U << 0))
		{
			//wait
		}
    /* Clear interrupt flags ( interrupt flags) */
		DMA2_CONTROL->LIFCR |= (1<<0); // CFEIF0
		DMA2_CONTROL->LIFCR |= (1<<2); // CDMEIF0
		DMA2_CONTROL->LIFCR |= (1<<3); // CTEIF0
		DMA2_CONTROL->LIFCR |= (1<<4); // CHTIF0
		DMA2_CONTROL->LIFCR |= (1<<5); // CTCIF0
    /* Reset CR */
    DMA2_CONTROL->STREAM[0].CR = 0;

    /* Channel 0 (ADC1) */
    DMA2_CONTROL->STREAM[0].CR |= (0U << 25);

    /* Peripheral to memory ( data transfer direction) */
    DMA2_CONTROL->STREAM[0].CR |= (0U << 6);

    /* Peripheral address = ADC_DR */
    DMA2_CONTROL->STREAM[0].PAR = (uint32_t)&ADC1_CONTROL->DR; /* The conversion result of ADC */ 

    /* Memory address */
    DMA2_CONTROL->STREAM[0].M0AR = (uint32_t)buffer;

    /* Number of data */
    DMA2_CONTROL->STREAM[0].NDTR = length;

    /* Memory increment */
    DMA2_CONTROL->STREAM[0].CR |= (1U << 10);

    /* Peripheral size = 16bit */
    DMA2_CONTROL->STREAM[0].CR |= (1U << 11);

    /* Memory size = 16bit */
    DMA2_CONTROL->STREAM[0].CR |= (1U << 13);

    /* Circular mode */
    DMA2_CONTROL->STREAM[0].CR |= (1U << 8);

    /* Enable stream */
    DMA2_CONTROL->STREAM[0].CR |= (1U << 0);
}