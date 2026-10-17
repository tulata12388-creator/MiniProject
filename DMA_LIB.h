#ifndef _DMA_H_
#define _DMA_H_

#include "c_stdint_type_confirm.h"
#include "board.h"   /* ADC_NUM_CHANNELS */

/*============================================================
 *  DMA_LIB.h – DMA Register Map (STM32F411)
 *
 *  This project uses DMA2 Stream 0 Channel 0 to transfer
 *  ADC1 scan results into g_adc_buf[] in circular mode.
 *
 *  DMA2 Stream 0 configuration (set in ADC_DMA_LIB.c):
 *    Channel    : 0  (ADC1 request on DMA2 Stream0 Ch0)
 *    Direction  : Peripheral ? Memory
 *    Data width : 16-bit (half-word)
 *    MINC       : Memory increment ON
 *    CIRC       : Circular mode ON (auto-restart)
 *    TCIE       : Transfer-complete interrupt ? callback
 *============================================================*/

/* DMA base addresses (AHB1, RM0383 §2.3) */
#define DMA1_BASE_ADDR   (0x40026000UL)
#define DMA2_BASE_ADDR   (0x40026400UL)

/*---------------------------------------------------------
 * DMA Stream register block  (24 bytes per stream)
 *
 * Offset  Register  Key bits
 * ------  --------  ----------------------------------
 * 0x00    CR        EN, TCIE, CIRC, DIR, MINC, MSIZE,
 *                   PSIZE, PL, CHSEL[27:25]
 * 0x04    NDTR      Number of data items to transfer
 * 0x08    PAR       Peripheral address (ADC1->DR)
 * 0x0C    M0AR      Memory 0 address   (g_adc_buf)
 * 0x10    M1AR      Memory 1 address   (double-buffer)
 * 0x14    FCR       FIFO control (disabled = direct mode)
 *---------------------------------------------------------*/
typedef struct
{
    volatile uint32_t CR;     /* 0x00  stream config             */
    volatile uint32_t NDTR;   /* 0x04  number of data items      */
    volatile uint32_t PAR;    /* 0x08  peripheral address         */
    volatile uint32_t M0AR;   /* 0x0C  memory 0 address           */
    volatile uint32_t M1AR;   /* 0x10  memory 1 address           */
    volatile uint32_t FCR;    /* 0x14  FIFO control               */
} DMA_Stream_TypeDef_Mini;

/*---------------------------------------------------------
 * DMA Controller register block
 *
 * LISR/HISR : status flags for streams 0-3 / 4-7
 * LIFCR/HIFCR : write-1-to-clear flag registers
 * S0..S7 : stream register blocks (24 bytes each)
 *---------------------------------------------------------*/
typedef struct
{
    volatile uint32_t LISR;     /* 0x00  low interrupt status      */
    volatile uint32_t HISR;     /* 0x04  high interrupt status     */
    volatile uint32_t LIFCR;    /* 0x08  low interrupt flag clear  */
    volatile uint32_t HIFCR;    /* 0x0C  high interrupt flag clear */
    DMA_Stream_TypeDef_Mini S0; /* 0x10  Stream 0 ? (ADC1)        */
    DMA_Stream_TypeDef_Mini S1; /* 0x28  Stream 1                  */
    DMA_Stream_TypeDef_Mini S2; /* 0x40  Stream 2                  */
    DMA_Stream_TypeDef_Mini S3; /* 0x58  Stream 3                  */
    DMA_Stream_TypeDef_Mini S4; /* 0x70  Stream 4                  */
    DMA_Stream_TypeDef_Mini S5; /* 0x88  Stream 5                  */
    DMA_Stream_TypeDef_Mini S6; /* 0xA0  Stream 6                  */
    DMA_Stream_TypeDef_Mini S7; /* 0xB8  Stream 7                  */
} DMA_TypeDef_Mini;

#define DMA1   ((DMA_TypeDef_Mini *) DMA1_BASE_ADDR)
#define DMA2_REG   ((DMA_TypeDef_Mini *) DMA2_BASE_ADDR)

/* Stream shortcut pointers */
#define DMA2_STREAM0   (&(DMA2_REG->S0))

/* ----------- Shared Data -----------
 * g_adc_buf[4] — DMA destination, written by hardware.
 * Read by Greenhouse_OnAdcReady() in DMA TC IRQ.
 */
extern volatile uint16_t g_adc_buf[ADC_NUM_CHANNELS];

/* Initialize ADC1 scan + DMA2 circular and start conversion */
void ADC1_DMA2_Stream0_InitStart(void);

/* Check if new ADC data is ready (DMA TC flag, polled from main loop) */
uint8_t ADC_DMA_CheckReady(void);

#endif /* _DMA_H_ */