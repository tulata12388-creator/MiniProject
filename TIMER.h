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
	volatile unsigned long CR1;
	volatile unsigned long CR2;
	volatile unsigned long SMCR;
	volatile unsigned long DIER;
	volatile unsigned long SR;
	volatile unsigned long EGR;
	volatile unsigned long CCMR1;
	volatile unsigned long CCMR2;
	volatile unsigned long CCER;
	volatile unsigned long CNT;
	volatile unsigned long PSC;
	volatile unsigned long ARR;
	volatile unsigned long RCR;  /* ADVANCED TIMER */
	volatile unsigned long CCR1;
	volatile unsigned long CCR2;
	volatile unsigned long CCR3;
	volatile unsigned long CCR4;
	volatile unsigned long BDTR; /* ADVANCED TIMER */
	volatile unsigned long DCR;
	volatile unsigned long DMAR;
	volatile unsigned long OR; 	 /* TIM 2 OR TIM 5 */ 
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