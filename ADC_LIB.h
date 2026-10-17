#ifndef _ADC_H_
#define _ADC_H_

#include "c_stdint_type_confirm.h"
#include "stm32f4xx.h"

/*============================================================
 *  ADC_LIB.h – ADC Register Map (STM32F411)
 *
 *  This project uses ADC1 in scan mode + DMA, reading 4 analog
 *  channels continuously (PA0-PA3).  See board.h Section 3.
 *
 *  Key registers used:
 *    CR1  : SCAN mode enable
 *    CR2  : DMA, DDS, CONT, ADON, SWSTART
 *    SMPR2: Sample time for channels 0-9
 *    SQR1 : Number of conversions (L field)
 *    SQR3 : Conversion sequence (channels 0-3)
 *    DR   : Data register (DMA reads from here)
 *============================================================*/

/* ADC1 base address (APB2, RM0383 §2.3) */
#define ADC1_BASE_ADDR   (0x40012000UL)

/*---------------------------------------------------------
 * ADC register map  (RM0383 §13.13)
 *
 * Offset  Register  This project
 * ------  --------  ---------------------------------
 * 0x00    SR        Status (EOC, OVR flags)
 * 0x04    CR1     ? SCAN bit [8]
 * 0x08    CR2     ? DMA [8], DDS [9], CONT [1],
 *                    ADON [0], SWSTART [30]
 * 0x0C    SMPR1     Sample time ch10-18
 * 0x10    SMPR2   ? Sample time ch0-9 (3 bits each)
 * 0x2C    SQR1    ? L[23:20] = num_conversions - 1
 * 0x34    SQR3    ? Channel sequence (5 bits each)
 * 0x4C    DR      ? Conversion result (DMA target)
 *---------------------------------------------------------*/
typedef struct
{
    volatile uint32_t SR;       /* 0x00  status register           */
    volatile uint32_t CR1;      /* 0x04  control register 1        */
    volatile uint32_t CR2;      /* 0x08  control register 2        */
    volatile uint32_t SMPR1;    /* 0x0C  sample time ch10-18       */
    volatile uint32_t SMPR2;    /* 0x10  sample time ch0-9         */
    volatile uint32_t JOFR1;    /* 0x14  injected offset 1         */
    volatile uint32_t JOFR2;    /* 0x18  injected offset 2         */
    volatile uint32_t JOFR3;    /* 0x1C  injected offset 3         */
    volatile uint32_t JOFR4;    /* 0x20  injected offset 4         */
    volatile uint32_t HTR;      /* 0x24  watchdog high threshold   */
    volatile uint32_t LTR;      /* 0x28  watchdog low threshold    */
    volatile uint32_t SQR1;     /* 0x2C  regular sequence 1 (L)    */
    volatile uint32_t SQR2;     /* 0x30  regular sequence 2        */
    volatile uint32_t SQR3;     /* 0x34  regular sequence 3        */
    volatile uint32_t JSQR;     /* 0x38  injected sequence         */
    volatile uint32_t JDR1;     /* 0x3C  injected data 1           */
    volatile uint32_t JDR2;     /* 0x40  injected data 2           */
    volatile uint32_t JDR3;     /* 0x44  injected data 3           */
    volatile uint32_t JDR4;     /* 0x48  injected data 4           */
    volatile uint32_t DR;       /* 0x4C  regular data register     */
} ADC_TypeDef_Mini;

#define ADC1  ((ADC_TypeDef_Mini *) ADC1_BASE_ADDR)

#endif /* _ADC_H_ */