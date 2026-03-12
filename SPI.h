#ifndef _SPI_H
#define _SPI_H
#include "RCC_STM32.h"
/* Define base addressed */
#define SPI1_BASE_ADDRESSED 0x40013000UL
#define SPI2_BASE_ADDRESSED 0x40003800UL
#define SPI3_BASE_ADDRESSED 0x40003C00UL
#define SPI4_BASE_ADDRESSED 0x40013400UL
#define SPI5_BASE_ADDRESSED 0x40015000UL
typedef struct 
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;
	volatile uint32_t I2SCFGR;
	volatile uint32_t I2SPR;
}SPI_TYPE;

#define SPI1_CONTROL ((SPI_TYPE*) SPI1_BASE_ADDRESSED)
#define SPI2_CONTROL ((SPI_TYPE*) SPI2_BASE_ADDRESSED)
#define SPI3_CONTROL ((SPI_TYPE*) SPI3_BASE_ADDRESSED)
#define SPI4_CONTROL ((SPI_TYPE*) SPI4_BASE_ADDRESSED)
#define SPI5_CONTROL ((SPI_TYPE*) SPI5_BASE_ADDRESSED)
/* Define control Register 1 */
#define SPI_CR1_CPHA_First 		(0U << 0) /* Clock phase (CPHA = 0) */
#define SPI_CR1_CPHA_Second		(1U << 0)	/* Clock phase (CPHA = 1) */

#define SPI_CR1_CPOL_LOW			(0U << 1)
#define SPI_CR1_CPOL_HIGH			(1U << 1)
/* Mode |  CPHA  | CPOL | Meaning
		1		|		0		 | 	0		|	First  & Rising  EDGE 
		2		|		0		 | 	1		|	First  & Falling EDGE 
		3		|		1		 | 	0		|	Second & Falling EDGE 
		4		|		1		 | 	1		|	Second & Rising  EDGE 
*/
#define SPI_CR1_Slave					(0U << 2)
#define SPI_CR1_Master				(1U << 2)	
#define SPI_CR1_disabled			(0U << 6)
#define SPI_CR1_enable				(1U << 6)
#define SPI_CR1_MSB_first			(0U << 7)
#define SPI_CR1_LSB_first			(1U << 7)
#define SPI_CR1_SSI_EN				(1U << 8)
#define SPI_CR1_SSM_EN				(1U << 9)
/* Define basis function */ 
void SPI1_Init(void);
void SPI1_SetTxBuffer(uint8_t *data, uint16_t length);
void SPI1_ResetBuffer(void);
/* Polling */ 
uint8_t SPI1_Transfer(uint8_t data);

#endif /* _SPI_H */ 
