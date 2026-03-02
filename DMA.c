#include "DMA.h"
#include "RCC_STM32.h" 
void DMA2_Stream3_Config_TX (void)
{
	/* Enable Clock DMA2 */ 
	CLK_CONTROL->AHB1ENR |= (1U << 22);
	/* Wait until stream disabled */ 
	DMA2_CONTROL->STREAM[3].CR &=  ~(1U << 0);
	while (DMA2_CONTROL->STREAM[3].CR & (1U << 0))
	{
		;// wait 
	}
	/* Clear ALL flags */ 
	DMA2_CONTROL->LIFCR |= (1U << 22); 	/* CFEIF3  */
	DMA2_CONTROL->LIFCR |= (1U << 24);  /* CDMEIF3 */
	DMA2_CONTROL->LIFCR |= (1U << 25); 	/* CTEIF3  */
	DMA2_CONTROL->LIFCR |= (1U << 26); 	/* CHTIF3  */
	DMA2_CONTROL->LIFCR |= (1U << 27);  /* CTCIF3  */
	/* Config CR */
	DMA2_CONTROL->STREAM[3].CR = 0;
	/* Choose Channel */ 
	DMA2_CONTROL->STREAM[3].CR |= 	(3U << 25); /* 0b011: Channel 3 */ 
	/* Data transfer direction */
	DMA2_CONTROL->STREAM[3].CR 	|= 	(1U << 6); 	/* 0b01: Memory to Peripheral ( Memory -> SPI1 ) */
	/* Peripheral increment mode */
	DMA2_CONTROL->STREAM[3].CR 	&= ~(1U << 9);
	/* Memory increment mode*/ 
	DMA2_CONTROL->STREAM[3].CR 	|=  (1U << 10); /* Doc lan luot tung byte trong buffer(Doc tung byte xong tang dia chi len ) */ 
	/* Peripheral data size */ 
	DMA2_CONTROL->STREAM[3].CR	&= ~(3U << 11); /* ~0b11: 8-bit */
	/* Memory data size			*/ 
	DMA2_CONTROL->STREAM[3].CR 	&= ~(3U << 13); /* ~0b11: 8-bit */
	/* Priority level */
	DMA2_CONTROL->STREAM[3].CR  |= 	(2U << 16); /* 0b10: High 	*/
	/* Circular mode */
	DMA2_CONTROL->STREAM[3].CR  &=  ~(1U << 8);  /* Khong can gui lien tuc */
	/* FIFO disabled */
	DMA2_CONTROL->STREAM[3].FCR = 0;
}



void DMA2_Stream0_Config_RX (void)
{
	/* Enable Clock DMA2 */ 
	CLK_CONTROL->AHB1ENR |= (1U << 22);
	/* Wait until stream disabled */ 
	DMA2_CONTROL->STREAM[0].CR &=  ~(1U << 0);
	while (DMA2_CONTROL->STREAM[0].CR & (1U << 0))
	{
		;// wait 
	}
	/* Clear ALL flags */ 
	DMA2_CONTROL->LIFCR |= (1U << 22); 	/* CFEIF3  */
	DMA2_CONTROL->LIFCR |= (1U << 24);  /* CDMEIF3 */
	DMA2_CONTROL->LIFCR |= (1U << 25); 	/* CTEIF3  */
	DMA2_CONTROL->LIFCR |= (1U << 26); 	/* CHTIF3  */
	DMA2_CONTROL->LIFCR |= (1U << 27);  /* CTCIF3  */
	/* Config CR */
	DMA2_CONTROL->STREAM[0].CR = 0;
	/* Choose Channel */ 
	DMA2_CONTROL->STREAM[0].CR |= 	(3U << 25); /* 0b011: Channel 3 */ 
	/* Data transfer direction */
	DMA2_CONTROL->STREAM[0].CR 	|= 	(0U << 6); 	/* 0b01: Peripheral to Memory ( SPI1 -> Memory ) */
	/* Peripheral increment mode */
	DMA2_CONTROL->STREAM[0].CR 	&= ~(1U << 9);
	/* Memory increment mode*/ 
	DMA2_CONTROL->STREAM[0].CR 	|=  (1U << 10); /* Doc lan luot tung byte trong buffer(Doc tung byte xong tang dia chi len ) */ 
	/* Peripheral data size */ 
	DMA2_CONTROL->STREAM[0].CR	&= ~(3U << 11); /* ~0b11: 8-bit */
	/* Memory data size			*/ 
	DMA2_CONTROL->STREAM[0].CR 	&= ~(3U << 13); /* ~0b11: 8-bit */
	/* Priority level */
	DMA2_CONTROL->STREAM[0].CR  |= 	(2U << 16); /* 0b10: High 	*/
	/* Circular mode */
	DMA2_CONTROL->STREAM[0].CR  &=  ~(1U << 8); /* Khong can gui lien tuc */
	/* FIFO disabled */
	DMA2_CONTROL->STREAM[0].FCR = 0;
}
void DMA2_Stream3_Start(uint8_t *buf, uint16_t len);
void DMA2_Stream0_Start(uint8_t *buf, uint16_t len);


