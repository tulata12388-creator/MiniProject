#include "SPI.h"
#include "RCC_STM32.h"
void SPI1_Init(void)
{	
	CLK_CONTROL->APB2ENR |= (1U << 12); 		/* CLOCK SPI1 */
	/* Disable CR1 */
	SPI1_CONTROL->CR1 	 &= ~(1U << 6); 		/* SPI_CR1_disabled 	*/
	/* Clear */
	SPI1_CONTROL->CR1 = 0;
  SPI1_CONTROL->CR2 = 0;
	/* Config CR1 */
	SPI1_CONTROL->CR1		 |=  (1U << 2);		  /* SPI_CR1_Master 		*/
	
	SPI1_CONTROL->CR1 	 &= ~(1U << 0);			/* SPI_CR1_CPHA_First */
	SPI1_CONTROL->CR1		 &= ~(1U << 1);			/* SPI_CR1_CPOL_LOW 	*/
	
	SPI1_CONTROL->CR1		 &= ~(7U << 3); 		/* Baudrate: fPCLK /2 */
	SPI1_CONTROL->CR1		 &= ~(1U << 7); 		/* SPI_CR1_MSB_first  */
	SPI1_CONTROL->CR1		 |=  (1U << 8);			/* SPI_CR1_SSI_EN     */
	SPI1_CONTROL->CR1		 |=  (1U << 9);			/* SPI_CR1_SSM_EN 		*/
	
	SPI1_CONTROL->CR1		 |= (1U << 6);			/* SPI_CR1_enable 		*/
}
void SPI1_Enable(void)
{
	SPI1_CONTROL->CR1		 |= (1U << 6);
}
void SPI1_Disabled(void)
{
	SPI1_CONTROL->CR1		 &= ~(1U << 6); 
}	
void SPI1_Enable_DMA_TX(void)
{
	SPI1_CONTROL->CR2		 |=  (1U << 1);			/* SPI_CR2_TXDMA 			*/
}
void SPI1_Enable_DMA_RX(void)
{
  SPI1_CONTROL->CR2		 |=  (1U << 0);			/* SPI_CR2_RXDMA 			*/
}
/*
void SPI1_TransmitReceive_DMA(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t length)
{
	
}*/