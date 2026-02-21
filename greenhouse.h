#ifndef _GREENHOUSE_H_
#define _GREENHOUSE_H_

#include <stdint.h>
#include "board.h"

extern volatile uint8_t g_spi_packet[PACKET_LEN];

void Greenhouse_InitPacket(void);
void Greenhouse_OnAdcReady(void);

#endif