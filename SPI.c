#include "SPI.h"
#include "RCC_STM32.h"
#include "NVIC.h"
/* Buffer de gui du lieu cho Raspberry Pi */
static unsigned char *spi_tx_buffer = 0;
static unsigned short spi_tx_length = 0;
static unsigned short spi_tx_index = 0;
void SPI1_Init(void)
{	
	CLK_CONTROL->APB2ENR |= (1U << 12); 		/* CLOCK SPI1 */
	/* Disable CR1 */
	SPI1_CONTROL->CR1 	 &= ~(1U << 6); 		/* SPI_CR1_disabled 	*/
	/* Clear 
	   Slave mode
     CPOL = 0
     CPHA = 0
     8-bit data
     hardware NSS */
	SPI1_CONTROL->CR1 = 0;
	/* Enable RX interrupt */
  SPI1_CONTROL->CR2 |= (1 << 6);
	/* Enable interrupt trong NVIC */
	NVIC_ClearPending	(35);
	NVIC_SetPriority	(35, 1);
	NVIC_EnableIRQn		(35); 		/* SPI1 : IRQn = 35) */
	/* SPI_CR1_enable 		*/
	SPI1_CONTROL->CR1		 |= (1U << 6);			
}
/* Gán buffer du lieu se gui */
void SPI1_SetTxBuffer(unsigned char *data, unsigned short length)
{
    spi_tx_buffer = data;
    spi_tx_length = length;
    spi_tx_index = 0;
}
/* Reset vi trí buffer */
void SPI1_ResetBuffer(void)
{
    spi_tx_index = 0;
}

void SPI1_IRQHandler(void) /* Auto xu ly khi co du lieu khong can CPU polling */
{
		// Check OVR 
    if (SPI1_CONTROL->SR & (1 << 6)) /* Receive buffer not empty */
    {
        volatile unsigned char temp;
        temp = SPI1_CONTROL->DR;
        temp = SPI1_CONTROL->SR;
        return;
    }
    /* neu SPI nhan du lieu */
   if (SPI1_CONTROL->SR & (1 << 0))
    {
        /* doc du lieu de clear RXNE */
        unsigned char data = SPI1_CONTROL->DR;
				/* Check */ 
				if(spi_tx_buffer && spi_tx_length)
				{
					SPI1_CONTROL->DR = spi_tx_buffer[spi_tx_index];

					spi_tx_index++;
						if (spi_tx_index >= spi_tx_length)
						{
								spi_tx_index = 0;
						}
				}
				else
				{
					SPI1_CONTROL->DR = 0x00;
				}
    }
}
/* Test 
void SPI1_Init_Master(void)
{
    CLK_CONTROL->APB2ENR |= (1<<12);

    SPI1_CONTROL->CR1 = 0;

    SPI1_CONTROL->CR1 |= (1<<2); // Master
    SPI1_CONTROL->CR1 |= (1<<9); // SSM
    SPI1_CONTROL->CR1 |= (1<<8); // SSI

    SPI1_CONTROL->CR1 |= (3<<3); // baudrate

    SPI1_CONTROL->CR1 |= (1<<6); // enable SPI
}
unsigned char SPI1_Transfer(unsigned char data)
{
    while(!(SPI1_CONTROL->SR & (1<<1)));

    SPI1_CONTROL->DR = data;

    while(!(SPI1_CONTROL->SR & (1<<0)));

    return SPI1_CONTROL->DR;
}*/
