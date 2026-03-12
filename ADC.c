#include "ADC.h" 
#include "RCC_STM32.h" 
#include "Define.h"
volatile uint16_t adc_buffer[4];
void ADC1_init(void) 
{
	/* Enable Clock */
	CLK_CONTROL->APB2ENR |= (1U << 8); 
	/* 12 bit - Resolution */ 
	ADC1_CONTROL->CR1 	 &= ~(3U << 24); 
	/* Scan mode */ 
	ADC1_CONTROL->CR1 	 |= (1U << 8);   
	/* Data alignment */ 
	ADC1_CONTROL->CR2 	 &= ~(1U << 11); /* Right alignment */ 
	/* Sample time */ 
	ADC1_CONTROL->SMPR2  &= ~(7U << (ADC_CH_LM35 * 3)); 	/* Channel 0 : 84 cycles */
	ADC1_CONTROL->SMPR2  &= ~(7U << (ADC_CH_GAS  * 3));   /* Channel 1 : 84 cycles */
	ADC1_CONTROL->SMPR2  &= ~(7U << (ADC_CH_S3   * 3)); 	/* Channel 2 : 84 cycles */
	ADC1_CONTROL->SMPR2  &= ~(7U << (ADC_CH_S4   * 3));   /* Channel 3 : 84 cycles */
	
	ADC1_CONTROL->SMPR2  |=  (4U << (ADC_CH_LM35 * 3)); 		/* Channel 0 : 84 cycles */
	ADC1_CONTROL->SMPR2  |=  (4U << (ADC_CH_GAS  * 3));  	/* Channel 1 : 84 cycles */
	ADC1_CONTROL->SMPR2  |=  (4U << (ADC_CH_S3   * 3)); 		/* Channel 2 : 84 cycles */
	ADC1_CONTROL->SMPR2  |=  (4U << (ADC_CH_S4   * 3));  	/* Channel 3 : 84 cycles */
	/* Clear sequence register */
  ADC1_CONTROL->SQR3 = 0;
	/* regular sequence */ 
	ADC1_CONTROL->SQR3 	 |=  (0U << 0);   /* Sensor 1 */ 
	ADC1_CONTROL->SQR3 	 |=  (1U << 5); 	/* Sensor 2 */ 
	ADC1_CONTROL->SQR3 	 |=  (2U << 10);	/* Sensor 3 */
	ADC1_CONTROL->SQR3 	 |=  (3U << 15);	/* Sensor 4 */ 
	/* Regular channel sequence length */
  ADC1_CONTROL->SQR1 	 &= ~(0xFU << 20);	
	ADC1_CONTROL->SQR1 	 |=  (3U << 20);
	/* Enable ADC */ 
	ADC1_CONTROL->CR2		 |=  (1U << 0); 
}
void ADC1_Enable_DMA(void)
{
    /* Enable DMA request */
    ADC1_CONTROL->CR2 |= (1U << 8);

    /* DMA continuous request */
    ADC1_CONTROL->CR2 |= (1U << 9);
}
void ADC1_Read_Polling(uint16_t *buffer)
{
    for(uint8_t i = 0; i < 4; i++)
    {
        /* Start conversion */
        ADC1_CONTROL->CR2 |= (1U << 30);

        /* Wait conversion complete */
        while(!(ADC1_CONTROL->SR & (1U << 1)));

        buffer[i] = ADC1_CONTROL->DR;
    }
}
uint16_t ADC1_Read_DMA(uint8_t channel)
{
    if(channel < 4)
        return adc_buffer[channel];
    else
        return 0;
}

