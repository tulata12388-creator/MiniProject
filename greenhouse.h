#ifndef _GREENHOUSE_H_
#define _GREENHOUSE_H_

#include "c_stdint_type_confirm.h"
#include "board.h"

/*============================================================
 *  greenhouse.h – Central System Logic
 *
 *  Connects all service-layer modules:
 *    ADC data (adc_mgr) ? Alarm logic (fire_logic)
 *    ? Actuator control ? SPI packet (for Raspberry Pi)
 *
 *  Called from DMA2 TC IRQ every time 4 ADC channels complete.
 *
 *  Uses double-buffer for SPI TX:
 *    g_spi_buf_a / g_spi_buf_b — one active (SPI reads),
 *    one back-buffer (greenhouse writes).  Swapped atomically.
 *============================================================*/

/* Build initial zero-frame and set up SPI TX double-buffer */
void Greenhouse_InitPacket(void);

/* Callback from DMA2 TC IRQ ? full processing pipeline */
void Greenhouse_OnAdcReady(void);

#endif /* _GREENHOUSE_H_ */
