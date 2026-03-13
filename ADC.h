#ifndef _ADC_H
#define _ADC_H
#include "RCC_STM32.h" 

/* Define Base Adderessed */ 
#define ADC1_BASE_ADR 0x40012000UL

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR1;
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;
    volatile uint32_t LTR;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;
    volatile uint32_t JDR1;
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;
} ADC_Type;
/* Define ADC */ 
#define ADC1_CONTROL ((ADC_Type*) ADC1_BASE_ADR) 
#define ADC_CHANNEL_COUNT 4
/* buffer			*/
extern volatile uint16_t adc_buffer[4];
/* Function Prototype */ 
void ADC1_init(void); 
void ADC1_Enable_DMA(void);
void ADC1_Read_Polling(uint16_t *buffer);
void ADC1_Start(void);
uint16_t ADC1_Read_DMA(uint8_t channel);
#endif /* _ADC_ */