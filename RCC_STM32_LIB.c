#include "stm32f411xe.h"

/*============================================================
 *  RCC_Enable_For_GPIO_ADC_SPI_DMA
 *
 *  Enable peripheral clocks used by this project.
 *  Must be called FIRST in main(), before any GPIO/ADC/SPI
 *  register access — otherwise Hard Fault.
 *
 *  AHB1ENR (offset 0x30):
 *    Bit  0 : GPIOAEN  – PA0-PA7 (ADC + SPI pins)
 *    Bit  1 : GPIOBEN  – PB0-PB1 (Buzzer + Motor)
 *    Bit 22 : DMA2EN   – DMA2 for ADC1 circular transfer
 *
 *  APB2ENR (offset 0x44):
 *    Bit  8 : ADC1EN   – ADC1 (4-channel scan)
 *    Bit 12 : SPI1EN   – SPI1 slave (data ? Raspberry Pi)
 *    Bit 14 : SYSCFGEN – SYSCFG (required for EXTI mux on PA4/NSS)
 *============================================================*/
void RCC_Enable_For_GPIO_ADC_SPI_DMA(void)
{
    /* AHB1: GPIOA (PA0-PA7), GPIOB (PB0-PB1), DMA2 */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN
                  | RCC_AHB1ENR_GPIOBEN
                  | RCC_AHB1ENR_DMA2EN;
	  
    /* DMA1EN is bit 21 of AHB1ENR */
    RCC->AHB1ENR |= (1U << 21);
    
    /* I2C1EN is bit 21 of APB1ENR */
    RCC->APB1ENR |= (1U << 21);

    /* APB2: ADC1 + SPI1 + SYSCFG (needed for EXTI on PA4/NSS) */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN
                  | RCC_APB2ENR_SPI1EN
                  | RCC_APB2ENR_SYSCFGEN;
}