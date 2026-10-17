#include "irq_rm0383.h"
#include "i2c_header.h"
#include "DMA_LIB.h"  /* Your custom DMA header */
#include "board.h"

/* The global variable greenhouse.c will pack into the SPI frame */
volatile uint16_t g_i2c_pir_data = 0;

/* Internal 8-bit buffer for DMA to write into */
static volatile uint8_t i2c_rx_buf[2];

/*============================================================
 *  I2C1_DMA1_Stream0_Init
 *
 *  Hardware path:
 *    PB8 (SCL), PB9 (SDA) -> I2C1 (Master) 
 *    -> DMA1 Stream 0 Channel 1 -> i2c_rx_buf[2]
 *============================================================*/
void I2C1_DMA1_Stream0_Init(void)
{
   
    /* 2. Configure PB8 (SCL) and PB9 (SDA) for I2C1 (AF4) */
    
   


    /* 3. Configure DMA1 Stream 0 for I2C1_RX */
    
    /* EN (Stream Enable) is bit 0 of CR */
    DMA1->S0.CR &= ~(1U << 0);
    while(DMA1->S0.CR & (1U << 0)) { /* wait for disable */ }

    DMA1->S0.PAR  = (uint32_t)&I2C1->DR;       /* Read from I2C Data Reg */
    DMA1->S0.M0AR = (uint32_t)i2c_rx_buf;      /* Write to memory array */
    DMA1->S0.NDTR = 2;                         /* 2 bytes per transfer */

    /* 
     * Configure CR register bits:
     * CHSEL [27:25] : Channel 1 = bit 25
     * PL    [17:16] : Priority Medium = bit 16
     * MINC  [10]    : Memory increment mode = bit 10
     * CIRC  [8]     : Circular mode = bit 8
     * TCIE  [4]     : Transfer complete interrupt enable = bit 4
     */
    DMA1->S0.CR = (1U << 25) | 
                  (1U << 16) | 
                  (1U << 10) | 
                  (1U << 8)  | 
                  (1U << 4);

    /* Enable DMA Stream 0 Interrupt in NVIC */
    NVIC_SetPriority(DMA1_Stream0_IRQn, 2); 
    NVIC_EnableIRQ(DMA1_Stream0_IRQn);

    /* Enable DMA Stream -> EN is bit 0 */
    DMA1->S0.CR |= (1U << 0);


    /* 4. Configure I2C1 (Master, 100kHz Standard Mode @ 16MHz APB1) */
    
    /* PE (Peripheral Enable) is bit 0 of CR1. Disable for config */
    I2C1->CR1 &= ~(1U << 0);                       

    /* FREQ is bits 5:0 of CR2 -> 16 MHz */
    I2C1->CR2  = 16U;                               
    
    /* CCR is bits 11:0 -> 80 */
    I2C1->CCR  = 80U;                               
    
    /* TRISE is bits 5:0 -> 17 */
    I2C1->TRISE = 17U;                              

    /* DMAEN (DMA Requests Enable) is bit 11 of CR2 */
    I2C1->CR2 |= (1U << 11);                     
    
    /* Enable I2C peripheral -> PE is bit 0 of CR1 */
    I2C1->CR1 |= (1U << 0);                        
}

/*============================================================
 *  DMA1 Stream 0 ISR
 *============================================================*/
void DMA1_Stream0_IRQHandler(void)
{
    /* TCIF0 (Transfer Complete Interrupt Flag for Stream 0) is bit 5 of LISR */
    if (DMA1->LISR & (1U << 5))
    {
        /* CTCIF0 (Clear Transfer Complete flag for Stream 0) is bit 5 of LIFCR */
        DMA1->LIFCR = (1U << 5);
        
        /* Recombine the two 8-bit I2C reads into the 16-bit variable for SPI */
        g_i2c_pir_data = (uint16_t)((i2c_rx_buf[0] << 8) | i2c_rx_buf[1]);
    }
}