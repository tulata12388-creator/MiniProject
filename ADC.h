#ifndef _ADC_H
#define _ADC_H
#include "RCC_STM32.h" 

/* Define Base Adderessed */ 
#define ADC1_BASE_ADR 0x40012000UL

typedef struct
{
    volatile unsigned long SR;
    volatile unsigned long CR1;
    volatile unsigned long CR2;
    volatile unsigned long SMPR1;
    volatile unsigned long SMPR2;
    volatile unsigned long JOFR1;
    volatile unsigned long JOFR2;
    volatile unsigned long JOFR3;
    volatile unsigned long JOFR4;
    volatile unsigned long HTR;
    volatile unsigned long LTR;
    volatile unsigned long SQR1;
    volatile unsigned long SQR2;
    volatile unsigned long SQR3;
    volatile unsigned long JSQR;
    volatile unsigned long JDR1;
    volatile unsigned long JDR2;
    volatile unsigned long JDR3;
    volatile unsigned long JDR4;
    volatile unsigned long DR;
} ADC_Type;
/* Define ADC */ 
#define ADC1_CONTROL ((ADC_Type*) ADC1_BASE_ADR) 
#define ADC_CHANNEL_COUNT 4
/* buffer			*/
extern volatile unsigned short adc_buffer[4];
/* Function Prototype */ 
void ADC1_init(void); 
void ADC1_Enable_DMA(void);
void ADC1_Read_Polling(unsigned short *buffer);
void ADC1_Start(void);
unsigned short ADC1_Read_DMA(unsigned char channel);
#endif /* _ADC_ */