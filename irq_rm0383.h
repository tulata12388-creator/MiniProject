#ifndef IRQ_RM0383_H
#define IRQ_RM0383_H

#include "core_cm4_bm.h"
#include "c_stdint_type_confirm.h"   

/* ================= PROJECT IRQ PRIORITY ================= */
#define SPI1_IRQ_PRIORITY             1U
#define EXTI4_IRQ_PRIORITY            1U
#define DMA2_STREAM0_IRQ_PRIORITY     2U
#define SYSTICK_IRQ_PRIORITY          3U

/* ================= PERIPHERAL STRUCTURES ================= */
typedef struct
{
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SR;
    __IO uint32_t DR;
    __IO uint32_t CRCPR;
    __IO uint32_t RXCRCR;
    __IO uint32_t TXCRCR;
    __IO uint32_t I2SCFGR;
    __IO uint32_t I2SPR;
} SPI_TypeDef;

typedef struct
{
    __IO uint32_t IMR;
    __IO uint32_t EMR;
    __IO uint32_t RTSR;
    __IO uint32_t FTSR;
    __IO uint32_t SWIER;
    __IO uint32_t PR;
} EXTI_TypeDef;

typedef struct
{
    __IO uint32_t MEMRMP;
    __IO uint32_t PMC;
    __IO uint32_t EXTICR[4];
    uint32_t RESERVED[2];
    __IO uint32_t CMPCR;
} SYSCFG_TypeDef;

typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t AHB1RSTR;
    __IO uint32_t AHB2RSTR;
    uint32_t RESERVED0[2];
    __IO uint32_t APB1RSTR;
    __IO uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    __IO uint32_t AHB1ENR;
    __IO uint32_t AHB2ENR;
    uint32_t RESERVED2[2];
    __IO uint32_t APB1ENR;
    __IO uint32_t APB2ENR;
    uint32_t RESERVED3[2];
    __IO uint32_t AHB1LPENR;
    __IO uint32_t AHB2LPENR;
    uint32_t RESERVED4[2];
    __IO uint32_t APB1LPENR;
    __IO uint32_t APB2LPENR;
    uint32_t RESERVED5[2];
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
    uint32_t RESERVED6[2];
    __IO uint32_t SSCGR;
    __IO uint32_t PLLI2SCFGR;
    __IO uint32_t DCKCFGR;
} RCC_TypeDef;

typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t NDTR;
    __IO uint32_t PAR;
    __IO uint32_t M0AR;
    __IO uint32_t M1AR;
    __IO uint32_t FCR;
} DMA_Stream_TypeDef;

typedef struct
{
    __IO uint32_t LISR;
    __IO uint32_t HISR;
    __IO uint32_t LIFCR;
    __IO uint32_t HIFCR;
} DMA_TypeDef;

/* ================= PERIPHERAL BASE ================= */
#define PERIPH_BASE             (0x40000000UL)
#define APB1PERIPH_BASE         (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE         (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE         (PERIPH_BASE + 0x00020000UL)

#define SPI1_BASE               (APB2PERIPH_BASE + 0x3000UL)
#define SPI2_BASE               (APB1PERIPH_BASE + 0x3800UL)

#define SYSCFG_BASE             (APB2PERIPH_BASE + 0x3800UL)
#define EXTI_BASE               (APB2PERIPH_BASE + 0x3C00UL)

#define RCC_BASE                (AHB1PERIPH_BASE + 0x3800UL)
#define DMA2_BASE               (AHB1PERIPH_BASE + 0x6400UL)
#define DMA2_Stream0_BASE       (DMA2_BASE + 0x010UL)

/* ================= PERIPHERAL POINTERS ================= */
#define SPI1                    ((SPI_TypeDef *)SPI1_BASE)
#define SPI2                    ((SPI_TypeDef *)SPI2_BASE)

#define SYSCFG                  ((SYSCFG_TypeDef *)SYSCFG_BASE)
#define EXTI                    ((EXTI_TypeDef *)EXTI_BASE)
#define RCC                     ((RCC_TypeDef *)RCC_BASE)

#define DMA2                    ((DMA_TypeDef *)DMA2_BASE)
#define DMA2_Stream0            ((DMA_Stream_TypeDef *)DMA2_Stream0_BASE)

/* ================= RCC BIT ================= */
#define RCC_APB2ENR_SYSCFGEN    (1UL << 14)

/* ================= SPI BIT ================= */
#define SPI_CR1_CPHA            (1UL << 0)
#define SPI_CR1_CPOL            (1UL << 1)
#define SPI_CR1_MSTR            (1UL << 2)
#define SPI_CR1_SPE             (1UL << 6)
#define SPI_CR1_LSBFIRST        (1UL << 7)
#define SPI_CR1_SSI             (1UL << 8)
#define SPI_CR1_SSM             (1UL << 9)

#define SPI_CR2_ERRIE           (1UL << 5)
#define SPI_CR2_RXNEIE          (1UL << 6)
#define SPI_CR2_TXEIE           (1UL << 7)

#define SPI_SR_RXNE             (1UL << 0)
#define SPI_SR_TXE              (1UL << 1)
#define SPI_SR_OVR              (1UL << 6)

/* ================= DMA STREAM0 FLAG ================= */
#define DMA_LIFCR_CFEIF0        (1UL << 0)
#define DMA_LIFCR_CDMEIF0       (1UL << 2)
#define DMA_LIFCR_CTEIF0        (1UL << 3)
#define DMA_LIFCR_CHTIF0        (1UL << 4)
#define DMA_LIFCR_CTCIF0        (1UL << 5)

/* ================= EXTI BIT ================= */
#define EXTI_LINE4              (1UL << 4)

/* ================= GLOBAL FLAG ================= */
extern volatile uint8_t  g_adc_dma_ready;
extern volatile uint32_t g_systick_ms;
extern volatile uint8_t  g_spi_tx_byte;
extern volatile uint8_t  g_spi_rx_byte;

/* ================= IRQ INIT ================= */
static inline void SPI1_IRQ_Init(void)
{
    NVIC_ClearPendingIRQ(SPI1_IRQn);
    NVIC_SetPriority(SPI1_IRQn, SPI1_IRQ_PRIORITY);
    NVIC_EnableIRQ(SPI1_IRQn);
}

static inline void EXTI4_IRQ_Init(void)
{
    NVIC_ClearPendingIRQ(EXTI4_IRQn);
    NVIC_SetPriority(EXTI4_IRQn, EXTI4_IRQ_PRIORITY);
    NVIC_EnableIRQ(EXTI4_IRQn);
}

static inline void DMA2_Stream0_IRQ_Init(void)
{
    NVIC_ClearPendingIRQ(DMA2_Stream0_IRQn);
    NVIC_SetPriority(DMA2_Stream0_IRQn, DMA2_STREAM0_IRQ_PRIORITY);
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

static inline void SysTick_IRQ_Init(uint32_t ticks_1ms)
{
    NVIC_SetPriority(SysTick_IRQn, SYSTICK_IRQ_PRIORITY);
    (void)SysTick_Config_BM(ticks_1ms);
}

static inline void IRQ_Init_All(uint32_t ticks_1ms)
{
    SPI1_IRQ_Init();
    EXTI4_IRQ_Init();
    DMA2_Stream0_IRQ_Init();
    SysTick_IRQ_Init(ticks_1ms);
}

/* ================= ENABLE SOURCE IN PERIPHERAL ================= */
static inline void DMA2_Stream0_ClearAllFlags(void)
{
    DMA2->LIFCR = DMA_LIFCR_CFEIF0
                | DMA_LIFCR_CDMEIF0
                | DMA_LIFCR_CTEIF0
                | DMA_LIFCR_CHTIF0
                | DMA_LIFCR_CTCIF0;
}

/* ================= ISR PROTOTYPE ================= */
void SPI1_IRQHandler(void);
void EXTI4_IRQHandler(void);
void DMA2_Stream0_IRQHandler(void);
void SysTick_Handler(void);

#endif /* IRQ_RM0383_H */