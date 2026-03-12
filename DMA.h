#ifndef _DMA_H
#define _DMA_H
#include "RCC_STM32.h"
/* Base Addressed */
#define DMA1_BASE_ADR 0x40026000UL
#define DMA2_BASE_ADR 0x40026400UL
/* 1 Stream */
typedef struct
{
    volatile uint32_t CR;     
    volatile uint32_t NDTR;   
    volatile uint32_t PAR;    
    volatile uint32_t M0AR;   
    volatile uint32_t M1AR;   
    volatile uint32_t FCR;    
} DMA_Stream_Type;
/* ALL DMA */
typedef struct
{
    volatile uint32_t LISR;    
    volatile uint32_t HISR;    
    volatile uint32_t LIFCR;   
		volatile uint32_t HIFCR;    
		DMA_Stream_Type STREAM[8];
} DMA_Type;

#define DMA1_CONTROL ((DMA_Type*) DMA1_BASE_ADR)
#define DMA2_CONTROL ((DMA_Type*) DMA2_BASE_ADR)

/* Function Prototype */ 
/* ADC - DMA */
void DMA2_Stream0_Config_ADC(uint16_t *buffer, uint16_t length);
/* 					 */
#endif /*	_DMA_H */