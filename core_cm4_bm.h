#ifndef CORE_CM4_BM_H
#define CORE_CM4_BM_H

#include "c_stdint_type_confirm.h"

/* =========================================================================
 *  Cortex-M4 core support (bare-metal)
 *  Target: STM32F411
 * ========================================================================= */

#define __NVIC_PRIO_BITS    (4U)

/* =========================================================================
 *  IRQ number definition
 *
 *  Negative value  : Cortex-M system exceptions
 *  Positive value  : MCU external interrupts
 * ========================================================================= */
typedef enum
{
    /* Cortex-M4 processor exceptions */
    NonMaskableInt_IRQn      = -14,
    MemoryManagement_IRQn    = -12,
    BusFault_IRQn            = -11,
    UsageFault_IRQn          = -10,
    SVCall_IRQn              = -5,
    DebugMonitor_IRQn        = -4,
    PendSV_IRQn              = -2,
    SysTick_IRQn             = -1,

    /* STM32F411 external interrupts */
    WWDG_IRQn                = 0,
    PVD_IRQn                 = 1,
    TAMP_STAMP_IRQn          = 2,
    RTC_WKUP_IRQn            = 3,
    FLASH_IRQn               = 4,
    RCC_IRQn                 = 5,
    EXTI0_IRQn               = 6,
    EXTI1_IRQn               = 7,
    EXTI2_IRQn               = 8,
    EXTI3_IRQn               = 9,
    EXTI4_IRQn               = 10,

    DMA1_Stream0_IRQn        = 11,
    DMA1_Stream1_IRQn        = 12,
    DMA1_Stream2_IRQn        = 13,
    DMA1_Stream3_IRQn        = 14,
    DMA1_Stream4_IRQn        = 15,
    DMA1_Stream5_IRQn        = 16,
    DMA1_Stream6_IRQn        = 17,
    ADC_IRQn                 = 18,

    EXTI9_5_IRQn             = 23,
    TIM1_BRK_TIM9_IRQn       = 24,
    TIM1_UP_TIM10_IRQn       = 25,
    TIM1_TRG_COM_TIM11_IRQn  = 26,
    TIM1_CC_IRQn             = 27,
    TIM2_IRQn                = 28,
    TIM3_IRQn                = 29,
    TIM4_IRQn                = 30,
    I2C1_EV_IRQn             = 31,
    I2C1_ER_IRQn             = 32,
    I2C2_EV_IRQn             = 33,
    I2C2_ER_IRQn             = 34,
    SPI1_IRQn                = 35,
    SPI2_IRQn                = 36,
    USART1_IRQn              = 37,
    USART2_IRQn              = 38,
    EXTI15_10_IRQn           = 40,
    RTC_Alarm_IRQn           = 41,
    OTG_FS_WKUP_IRQn         = 42,
    DMA1_Stream7_IRQn        = 47,
    SDIO_IRQn                = 49,
    TIM5_IRQn                = 50,
    SPI3_IRQn                = 51,

    DMA2_Stream0_IRQn        = 56,
    DMA2_Stream1_IRQn        = 57,
    DMA2_Stream2_IRQn        = 58,
    DMA2_Stream3_IRQn        = 59,
    DMA2_Stream4_IRQn        = 60,
    OTG_FS_IRQn              = 67,
    DMA2_Stream5_IRQn        = 68,
    DMA2_Stream6_IRQn        = 69,
    DMA2_Stream7_IRQn        = 70,
    USART6_IRQn              = 71,
    I2C3_EV_IRQn             = 72,
    I2C3_ER_IRQn             = 73,
    FPU_IRQn                 = 81,
    SPI4_IRQn                = 84,
    SPI5_IRQn                = 85
} IRQn_Type;

/* =========================================================================
 *  NVIC register map
 *  Base address: 0xE000E100
 * ========================================================================= */
typedef struct
{
    __IO uint32_t ISER[8];          /* Interrupt Set-Enable Registers      */
         uint32_t RESERVED0[24];
    __IO uint32_t ICER[8];          /* Interrupt Clear-Enable Registers    */
         uint32_t RESERVED1[24];
    __IO uint32_t ISPR[8];          /* Interrupt Set-Pending Registers     */
         uint32_t RESERVED2[24];
    __IO uint32_t ICPR[8];          /* Interrupt Clear-Pending Registers   */
         uint32_t RESERVED3[24];
    __IO uint32_t IABR[8];          /* Interrupt Active Bit Registers      */
         uint32_t RESERVED4[56];
    __IO uint8_t  IP[240];          /* Interrupt Priority Registers        */
         uint32_t RESERVED5[644];
    __O  uint32_t STIR;             /* Software Trigger Interrupt Register */
} NVIC_TypeDef;

/* =========================================================================
 *  SCB register map
 *  Base address: 0xE000ED00
 * ========================================================================= */
typedef struct
{
    __I  uint32_t CPUID;            /* CPU ID Base Register                */
    __IO uint32_t ICSR;             /* Interrupt Control and State Reg     */
    __IO uint32_t VTOR;             /* Vector Table Offset Register        */
    __IO uint32_t AIRCR;            /* Application Interrupt/Reset Control */
    __IO uint32_t SCR;              /* System Control Register             */
    __IO uint32_t CCR;              /* Configuration and Control Register  */
    __IO uint8_t  SHP[12];          /* System Handler Priority Registers   */
    __IO uint32_t SHCSR;            /* System Handler Control/State Reg    */
    __IO uint32_t CFSR;             /* Configurable Fault Status Register  */
    __IO uint32_t HFSR;             /* HardFault Status Register           */
    __IO uint32_t DFSR;             /* Debug Fault Status Register         */
    __IO uint32_t MMFAR;            /* MemManage Fault Address Register    */
    __IO uint32_t BFAR;             /* BusFault Address Register           */
    __IO uint32_t AFSR;             /* Auxiliary Fault Status Register     */
    __IO uint32_t PFR[2];           /* Processor Feature Register          */
    __IO uint32_t DFR;              /* Debug Feature Register              */
    __IO uint32_t ADR;              /* Auxiliary Feature Register          */
    __IO uint32_t MMFR[4];          /* Memory Model Feature Register       */
    __IO uint32_t ISAR[5];          /* Instruction Set Attribute Register  */
         uint32_t RESERVED0[5];
    __IO uint32_t CPACR;            /* Coprocessor Access Control Register */
} SCB_TypeDef;

/* =========================================================================
 *  SysTick register map
 *  Base address: 0xE000E010
 * ========================================================================= */
typedef struct
{
    __IO uint32_t CTRL;             /* SysTick Control and Status Register */
    __IO uint32_t LOAD;             /* SysTick Reload Value Register       */
    __IO uint32_t VAL;              /* SysTick Current Value Register      */
    __I  uint32_t CALIB;            /* SysTick Calibration Register        */
} SysTick_TypeDef;

/* =========================================================================
 *  Core peripheral base addresses
 * ========================================================================= */
#define SCS_BASE            (0xE000E000UL)
#define SYSTICK_BASE        (SCS_BASE + 0x0010UL)
#define NVIC_BASE           (SCS_BASE + 0x0100UL)
#define SCB_BASE            (SCS_BASE + 0x0D00UL)

/* =========================================================================
 *  Core peripheral declaration
 * ========================================================================= */
#define SysTick             ((SysTick_TypeDef *) SYSTICK_BASE)
#define NVIC                ((NVIC_TypeDef *)    NVIC_BASE)
#define SCB                 ((SCB_TypeDef *)     SCB_BASE)

/* =========================================================================
 *  SysTick CTRL register bit definition
 * ========================================================================= */
#define SysTick_CTRL_ENABLE_Msk      (1UL << 0U)
#define SysTick_CTRL_TICKINT_Msk     (1UL << 1U)
#define SysTick_CTRL_CLKSOURCE_Msk   (1UL << 2U)

/* =========================================================================
 *  NVIC: enable external interrupt
 * ========================================================================= */
static inline void NVIC_EnableIRQ(IRQn_Type irqn)
{
    if ((int32_t)irqn >= 0)
    {
        NVIC->ISER[((uint32_t)irqn >> 5U)] = (1UL << ((uint32_t)irqn & 0x1FU));
    }
}

/* =========================================================================
 *  NVIC: disable external interrupt
 * ========================================================================= */
static inline void NVIC_DisableIRQ(IRQn_Type irqn)
{
    if ((int32_t)irqn >= 0)
    {
        NVIC->ICER[((uint32_t)irqn >> 5U)] = (1UL << ((uint32_t)irqn & 0x1FU));
    }
}

/* =========================================================================
 *  NVIC: clear pending external interrupt
 * ========================================================================= */
static inline void NVIC_ClearPendingIRQ(IRQn_Type irqn)
{
    if ((int32_t)irqn >= 0)
    {
        NVIC->ICPR[((uint32_t)irqn >> 5U)] = (1UL << ((uint32_t)irqn & 0x1FU));
    }
}

/* =========================================================================
 *  NVIC/SCB: set interrupt priority
 *
 *  External IRQ   -> NVIC->IP[]
 *  System handler -> SCB->SHP[]
 * ========================================================================= */
static inline void NVIC_SetPriority(IRQn_Type irqn, uint32_t priority)
{
    uint8_t prio_field;

    prio_field = (uint8_t)((priority << (8U - __NVIC_PRIO_BITS)) & 0xFFU);

    if ((int32_t)irqn >= 0)
    {
        NVIC->IP[(uint32_t)irqn] = prio_field;
    }
    else
    {
        SCB->SHP[((((uint32_t)irqn) & 0x0FU) - 4UL)] = prio_field;
    }
}

/* =========================================================================
 *  SysTick: configure timer tick
 *
 *  ticks = reload + 1
 *  Return:
 *      0 -> success
 *      1 -> reload value out of range
 * ========================================================================= */
static inline uint32_t SysTick_Config_BM(uint32_t ticks)
{
    if ((ticks == 0UL) || ((ticks - 1UL) > 0x00FFFFFFUL))
    {
        return 1UL;
    }

    SysTick->LOAD = (ticks - 1UL);  /* Set reload register              */
    SysTick->VAL  = 0UL;            /* Clear current counter value      */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;   /* Enable SysTick + interrupt */

    return 0UL;
}

#endif /* CORE_CM4_BM_H */