#include "Fire_detection.h"
#include "ADC.h"
extern volatile uint16_t adc_buffer[4];
static uint8_t fire_state = 0;

void Fire_Update(void)
{
	uint16_t temp = adc_buffer[0];
	uint16_t gas  = adc_buffer[1]; 
	
	/* Voltage = (ADC_value × Vref) / 4095
	   ADC_value: 0 -> 4095.
		 LM35 có dac diem:
		 10mV = 1°C
	*/
	if ( temp > 745 || gas > 1500 ) 
	{
		fire_state = 1; 
	}
	else 
	{
		fire_state = 0; 
	}
}
uint8_t Fire_GetState(void)
{
	return fire_state;
}
uint16_t Fire_GetTemperature(void)
{
	return adc_buffer[0];
}
uint16_t Fire_GetGas(void)
{
	return adc_buffer[1];
}