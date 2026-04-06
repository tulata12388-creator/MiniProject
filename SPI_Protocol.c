#include "SPI_Protocol.h" 
#include "SPI.h"
#include "Fire_detection.h"

static unsigned char spi_packet[3]; 
void SPI_Update_packet (void)
{
	spi_packet[0] = Fire_GetTemperature() >> 4;
	spi_packet[1] = Fire_GetGas() >> 4;
	spi_packet[2] = Fire_GetState();
	
	SPI1_SetTxBuffer(spi_packet,3);
}