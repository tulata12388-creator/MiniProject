#include "SPI.h"
#include "RCC_STM32.h"
/* Buffer de gui du lieu cho Raspberry Pi */
static uint8_t *spi_tx_buffer = 0;
static uint16_t spi_tx_length = 0;
static uint16_t spi_tx_index = 0;
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
  SPI1->CR2 |= (1 << 6);
	/* Enable interrupt trong NVIC */
  NVIC_EnableIRQ(SPI1_IRQn);
	
	SPI1_CONTROL->CR1		 |= (1U << 6);			/* SPI_CR1_enable 		*/
}
/* Gán buffer du lieu se gui */
void SPI1_SetTxBuffer(uint8_t *data, uint16_t length)
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
    /* neu SPI nhan du lieu */
    if (SPI1->SR & (1 << 0))
    {
        /* doc du lieu de clear RXNE */
        uint8_t data = SPI1->DR;

        /* GUI BYTE tiep theo */
        SPI1->DR = spi_tx_buffer[spi_tx_index];

        /* chuyen sang byte tiep theo */
        spi_tx_index++;

        /* neu het buffer thì quay lai dau */
        if (spi_tx_index >= spi_tx_length)
        {
            spi_tx_index = 0;
        }
    }
}


