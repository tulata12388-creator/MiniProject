#ifndef _TIMER_H
#define _TIMER_H

#include "RCC_STM32.h"
#include "GPIO.h"

#include <stdint.h>

/* Define base addressed */
// Advanced 
#define TIM1_BASE		0x40010000UL
// General Purpose
#define TIM2_BASE		0x40000000UL
#define TIM3_BASE		0x40000400UL
#define TIM4_BASE		0x40000800UL
#define TIM5_BASE		0x40000C00UL

#define TIM9_BASE		0x40014000UL
#define TIM10_BASE	0x40014400UL
#define TIM11_BASE	0x40014800UL
typedef struct 
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t RCR;  /* ADVANCED TIMER */
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t BDTR; /* ADVANCED TIMER */
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t OR; 	 /* TIM 2 OR TIM 5 */ 
}TIMER_TYPE;
#define TIM1_REG 	((TIMER_TYPE*) TIM1_BASE) 

#define TIM2_REG 	((TIMER_TYPE*) TIM2_BASE) 
#define TIM3_REG 	((TIMER_TYPE*) TIM3_BASE) 
#define TIM4_REG 	((TIMER_TYPE*) TIM4_BASE) 
#define TIM5_REG 	((TIMER_TYPE*) TIM5_BASE)

#define TIM9_REG 	((TIMER_TYPE*) TIM9_BASE) 
#define TIM10_REG ((TIMER_TYPE*) TIM10_BASE) 
#define TIM11_REG ((TIMER_TYPE*) TIM11_BASE) 

#endif //_TIMER_H