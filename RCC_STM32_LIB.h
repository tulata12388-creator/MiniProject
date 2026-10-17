#ifndef _RCC_STM32_LIB_H_
#define _RCC_STM32_LIB_H_

#include "c_stdint_type_confirm.h"
/*============================================================
 *  RCC_STM32_LIB.h – Reset & Clock Control (STM32F411)
 *
 *  This project enables:
 *    AHB1 : GPIOA, GPIOB, DMA2
 *    APB2 : ADC1, SPI1
 *
 *  All other peripherals remain clock-gated (low power).
 *============================================================*/
 


/* RCC peripheral base address (RM0383 §6.3.22) */
#define RCC_BASE_ADDR     0x40023800UL

/*====== Bit definition for RCC ==============================*/ 
/********************  Bit definition for RCC_AHB1ENR register  **************/
#define RCC_AHB1ENR_GPIOAEN_Pos           (0U)
#define RCC_AHB1ENR_GPIOAEN_Msk           (0x1UL << RCC_AHB1ENR_GPIOAEN_Pos)   /*!< 0x00000001 */
#define RCC_AHB1ENR_GPIOAEN               RCC_AHB1ENR_GPIOAEN_Msk

#define RCC_AHB1ENR_GPIOBEN_Pos           (1U)
#define RCC_AHB1ENR_GPIOBEN_Msk           (0x1UL << RCC_AHB1ENR_GPIOBEN_Pos)   /*!< 0x00000002 */
#define RCC_AHB1ENR_GPIOBEN               RCC_AHB1ENR_GPIOBEN_Msk

#define RCC_AHB1ENR_DMA2EN_Pos            (22U)
#define RCC_AHB1ENR_DMA2EN_Msk            (0x1UL << RCC_AHB1ENR_DMA2EN_Pos)    /*!< 0x00400000 */
#define RCC_AHB1ENR_DMA2EN                RCC_AHB1ENR_DMA2EN_Msk



/********************  Bit definition for RCC_APB2ENR register  **************/
#define RCC_APB2ENR_ADC1EN_Pos            (8U)
#define RCC_APB2ENR_ADC1EN_Msk            (0x1UL << RCC_APB2ENR_ADC1EN_Pos)    /*!< 0x00000100 */
#define RCC_APB2ENR_ADC1EN                RCC_APB2ENR_ADC1EN_Msk

#define RCC_APB2ENR_SPI1EN_Pos            (12U)
#define RCC_APB2ENR_SPI1EN_Msk            (0x1UL << RCC_APB2ENR_SPI1EN_Pos)    /*!< 0x00001000 */
#define RCC_APB2ENR_SPI1EN                RCC_APB2ENR_SPI1EN_Msk

#define RCC_APB2ENR_SYSCFGEN_Pos          (14U)
#define RCC_APB2ENR_SYSCFGEN_Msk          (0x1UL << RCC_APB2ENR_SYSCFGEN_Pos)  /*!< 0x00004000 */
#define RCC_APB2ENR_SYSCFGEN              RCC_APB2ENR_SYSCFGEN_Msk
/*---------------------------------------------------------
 * RCC register map  (RM0383 Table 13)
 * Offset  Register     This project uses?
 * ------  -----------  -------------------------
 * 0x00    CR           HSI/HSE/PLL control
 * 0x04    PLLCFGR      PLL configuration
 * 0x08    CFGR         Clock mux / prescalers
 * 0x0C    CIR          Clock interrupt register
 * 0x10    AHB1RSTR     AHB1 peripheral reset
 * 0x14    AHB2RSTR     AHB2 peripheral reset
 * 0x20    APB1RSTR     APB1 peripheral reset
 * 0x24    APB2RSTR     APB2 peripheral reset
 * 0x30    AHB1ENR    ? GPIOA, GPIOB, DMA2 enable
 * 0x34    AHB2ENR      AHB2 peripheral enable
 * 0x40    APB1ENR      APB1 peripheral enable
 * 0x44    APB2ENR    ? ADC1, SPI1 enable
 *---------------------------------------------------------*/
typedef struct
{
    volatile uint32_t CR;            /* 0x00  clock control              */
    volatile uint32_t PLLCFGR;       /* 0x04  PLL configuration          */
    volatile uint32_t CFGR;          /* 0x08  clock config               */
    volatile uint32_t CIR;           /* 0x0C  clock interrupt            */
    volatile uint32_t AHB1RSTR;      /* 0x10  AHB1 reset                 */
    volatile uint32_t AHB2RSTR;      /* 0x14  AHB2 reset                 */
    uint32_t          RESERVED0[2];  /* 0x18, 0x1C                       */
    volatile uint32_t APB1RSTR;      /* 0x20  APB1 reset                 */
    volatile uint32_t APB2RSTR;      /* 0x24  APB2 reset                 */
    uint32_t          RESERVED1[2];  /* 0x28, 0x2C                       */
    volatile uint32_t AHB1ENR;       /* 0x30  AHB1 enable  ?             */
    volatile uint32_t AHB2ENR;       /* 0x34  AHB2 enable                */
    uint32_t          RESERVED2[2];  /* 0x38, 0x3C                       */
    volatile uint32_t APB1ENR;       /* 0x40  APB1 enable                */
    volatile uint32_t APB2ENR;       /* 0x44  APB2 enable  ?             */
    uint32_t          RESERVED3[2];  /* 0x48, 0x4C                       */
    volatile uint32_t AHB1LPENR;     /* 0x50  AHB1 low-power enable      */
    volatile uint32_t AHB2LPENR;     /* 0x54  AHB2 low-power enable      */
    uint32_t          RESERVED4[2];  /* 0x58, 0x5C                       */
    volatile uint32_t APB1LPENR;     /* 0x60  APB1 low-power enable      */
    volatile uint32_t APB2LPENR;     /* 0x64  APB2 low-power enable      */
    uint32_t          RESERVED5[2];  /* 0x68, 0x6C                       */
    volatile uint32_t BDCR;          /* 0x70  backup domain control      */
    volatile uint32_t CSR;           /* 0x74  clock control & status     */
    uint32_t          RESERVED6[2];  /* 0x78, 0x7C                       */
    volatile uint32_t SSCGR;         /* 0x80  spread spectrum            */
    volatile uint32_t PLLI2SCFGR;    /* 0x84  PLLI2S configuration       */
    uint32_t          RESERVED7;     /* 0x88                             */
    volatile uint32_t DCKCFGR;       /* 0x8C  dedicated clocks config    */
} RCC_TypeDef_Mini;

#define RCC   ((RCC_TypeDef_Mini *) RCC_BASE_ADDR)

/* Enable clocks for GPIOA, GPIOB, DMA2, ADC1, SPI1 */
void RCC_Enable_For_GPIO_ADC_SPI_DMA(void);

#endif /* _RCC_STM32_LIB_H_ */
