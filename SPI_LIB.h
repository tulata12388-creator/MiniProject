#ifndef _SPI_H_
#define _SPI_H_

#include "c_stdint_type_confirm.h"
/*============================================================
 *  SPI_LIB.h – SPI1 Slave Driver (STM32F411)
 *
 *  SPI1 is configured as slave, Mode 0 (CPOL=0, CPHA=0),
 *  8-bit, hardware NSS, TXE interrupt-driven.
 *
 *  The Raspberry Pi (SPI master) clocks 16 bytes of 0x00.
 *  For each byte clocked in, the STM32 returns one byte
 *  from the TX frame buffer (built by greenhouse.c).
 *
 *  Frame alignment: g_idx wraps at g_len (16).  No EXTI on NSS.
 *  First valid frame starts at byte 0; Pi-side resync scans for
 *  the AA 55 ... 0D pattern and validates the XOR checksum.
 *
 *  Double-buffer mechanism:
 *    greenhouse.c builds a new 16-byte frame in a back-buffer,
 *    then calls SPI1_Slave_SwapBuffer() to atomically switch
 *    the active pointer — preventing the Pi from reading a
 *    partially-updated frame.
 *
 *  See board.h Section 7 for the full SPI protocol spec.
 *============================================================*/
 
 
 
/*******************  Bit definition for SPI_CR1 register  ********************/

#define SPI_CR1_CPHA_Pos            (0U)                                       
#define SPI_CR1_CPHA_Msk            (0x1UL << SPI_CR1_CPHA_Pos)                 /*!< 0x00000001 */
#define SPI_CR1_CPHA                SPI_CR1_CPHA_Msk                           /*!<Clock Phase      */
#define SPI_CR1_CPOL_Pos            (1U)                                       
#define SPI_CR1_CPOL_Msk            (0x1UL << SPI_CR1_CPOL_Pos)                 /*!< 0x00000002 */
#define SPI_CR1_CPOL                SPI_CR1_CPOL_Msk                           /*!<Clock Polarity   */
#define SPI_CR1_MSTR_Pos            (2U)                                       
#define SPI_CR1_MSTR_Msk            (0x1UL << SPI_CR1_MSTR_Pos)                 /*!< 0x00000004 */
#define SPI_CR1_MSTR                SPI_CR1_MSTR_Msk                           /*!<Master Selection */

#define SPI_CR1_BR_Pos              (3U)                                       
#define SPI_CR1_BR_Msk              (0x7UL << SPI_CR1_BR_Pos)                   /*!< 0x00000038 */
#define SPI_CR1_BR                  SPI_CR1_BR_Msk                             /*!<BR[2:0] bits (Baud Rate Control) */
#define SPI_CR1_BR_0                (0x1UL << SPI_CR1_BR_Pos)                   /*!< 0x00000008 */
#define SPI_CR1_BR_1                (0x2UL << SPI_CR1_BR_Pos)                   /*!< 0x00000010 */
#define SPI_CR1_BR_2                (0x4UL << SPI_CR1_BR_Pos)                   /*!< 0x00000020 */

#define SPI_CR1_SPE_Pos             (6U)                                       
#define SPI_CR1_SPE_Msk             (0x1UL << SPI_CR1_SPE_Pos)                  /*!< 0x00000040 */
#define SPI_CR1_SPE                 SPI_CR1_SPE_Msk                            /*!<SPI Enable                          */
#define SPI_CR1_LSBFIRST_Pos        (7U)                                       
#define SPI_CR1_LSBFIRST_Msk        (0x1UL << SPI_CR1_LSBFIRST_Pos)             /*!< 0x00000080 */
#define SPI_CR1_LSBFIRST            SPI_CR1_LSBFIRST_Msk                       /*!<Frame Format                        */
#define SPI_CR1_SSI_Pos             (8U)                                       
#define SPI_CR1_SSI_Msk             (0x1UL << SPI_CR1_SSI_Pos)                  /*!< 0x00000100 */
#define SPI_CR1_SSI                 SPI_CR1_SSI_Msk                            /*!<Internal slave select               */
#define SPI_CR1_SSM_Pos             (9U)                                       
#define SPI_CR1_SSM_Msk             (0x1UL << SPI_CR1_SSM_Pos)                  /*!< 0x00000200 */
#define SPI_CR1_SSM                 SPI_CR1_SSM_Msk                            /*!<Software slave management           */
#define SPI_CR1_RXONLY_Pos          (10U)                                      
#define SPI_CR1_RXONLY_Msk          (0x1UL << SPI_CR1_RXONLY_Pos)               /*!< 0x00000400 */
#define SPI_CR1_RXONLY              SPI_CR1_RXONLY_Msk                         /*!<Receive only                        */
#define SPI_CR1_DFF_Pos             (11U)                                      
#define SPI_CR1_DFF_Msk             (0x1UL << SPI_CR1_DFF_Pos)                  /*!< 0x00000800 */
#define SPI_CR1_DFF                 SPI_CR1_DFF_Msk                            /*!<Data Frame Format                   */
#define SPI_CR1_CRCNEXT_Pos         (12U)                                      
#define SPI_CR1_CRCNEXT_Msk         (0x1UL << SPI_CR1_CRCNEXT_Pos)              /*!< 0x00001000 */
#define SPI_CR1_CRCNEXT             SPI_CR1_CRCNEXT_Msk                        /*!<Transmit CRC next                   */
#define SPI_CR1_CRCEN_Pos           (13U)                                      
#define SPI_CR1_CRCEN_Msk           (0x1UL << SPI_CR1_CRCEN_Pos)                /*!< 0x00002000 */
#define SPI_CR1_CRCEN               SPI_CR1_CRCEN_Msk                          /*!<Hardware CRC calculation enable     */
#define SPI_CR1_BIDIOE_Pos          (14U)                                      
#define SPI_CR1_BIDIOE_Msk          (0x1UL << SPI_CR1_BIDIOE_Pos)               /*!< 0x00004000 */
#define SPI_CR1_BIDIOE              SPI_CR1_BIDIOE_Msk                         /*!<Output enable in bidirectional mode */
#define SPI_CR1_BIDIMODE_Pos        (15U)                                      
#define SPI_CR1_BIDIMODE_Msk        (0x1UL << SPI_CR1_BIDIMODE_Pos)             /*!< 0x00008000 */
#define SPI_CR1_BIDIMODE            SPI_CR1_BIDIMODE_Msk                       /*!<Bidirectional data mode enable      */
/* SPI base addresses (APB2 for SPI1, APB1 for SPI2/3) */
#define SPI1_BASE_ADDR   (0x40013000UL)
#define SPI2_BASE_ADDR   (0x40003800UL)
#define SPI3_BASE_ADDR   (0x40003C00UL)

/*---------------------------------------------------------
 * SPI register map  (RM0383 §28.5)
 *
 * Offset  Register   Key bits for slave mode
 * ------  ---------  ----------------------------------
 * 0x00    CR1        SPE [6], MSTR [2]=0 (slave),
 *                    CPOL [1]=0, CPHA [0]=0 (Mode 0)
 *                    SSM [9]=0 (hardware NSS)
 * 0x04    CR2        RXNEIE [6] (RX not empty IRQ)
 * 0x08    SR         RXNE [0], TXE [1], OVR [6]
 * 0x0C    DR         Data register (8-bit access)
 *---------------------------------------------------------*/
typedef struct
{
    volatile uint32_t CR1;      /* 0x00  control register 1       */
    volatile uint32_t CR2;      /* 0x04  control register 2       */
    volatile uint32_t SR;       /* 0x08  status register          */
    volatile uint32_t DR;       /* 0x0C  data register            */
    volatile uint32_t CRCPR;    /* 0x10  CRC polynomial           */
    volatile uint32_t RXCRCR;   /* 0x14  RX CRC                   */
    volatile uint32_t TXCRCR;   /* 0x18  TX CRC                   */
    volatile uint32_t I2SCFGR;  /* 0x1C  I2S configuration        */
    volatile uint32_t I2SPR;    /* 0x20  I2S prescaler            */
} SPI_TypeDef_Mini;

#define SPI1  ((SPI_TypeDef_Mini *) SPI1_BASE_ADDR)
#define SPI2  ((SPI_TypeDef_Mini *) SPI2_BASE_ADDR)
#define SPI3  ((SPI_TypeDef_Mini *) SPI3_BASE_ADDR)

/* ----------- API ----------- */

/* Initialize SPI1 as slave, Mode 0, 8-bit, RXNE IRQ */
void SPI1_Slave_Init(void);

/* Set the active TX buffer (called once during init) */
void SPI1_Slave_SetTxBuffer(volatile uint8_t *buf, uint16_t len);

/* Queue a new TX buffer for the NEXT frame boundary.
 * The actual pointer swap happens inside SPI1_IRQHandler
 * when g_idx wraps to 0, ensuring the Pi always receives
 * a complete, consistent frame (no mid-frame XOR mismatch). */
void SPI1_Slave_SwapBuffer(volatile uint8_t *new_buf, uint16_t len);

/* Returns 1 if a SwapBuffer is still pending (SPI mid-frame).
 * greenhouse.c checks this to avoid writing into the buffer
 * that SPI is currently reading. */
int  SPI1_Slave_IsSwapPending(void);

/* Reset TX byte index to 0 and pre-fill DR with byte[0] */
void SPI1_Slave_ResetIndex(void);
#endif /* _SPI_H_ */