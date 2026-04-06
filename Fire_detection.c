#include "Fire_detection.h"
#include "ADC.h"
extern volatile unsigned short adc_buffer[4];
static unsigned char fire_state = 0;

void Fire_Update(void)
{
	unsigned short temp = adc_buffer[0];
	unsigned short gas  = adc_buffer[1]; 
	
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
unsigned char Fire_GetState(void)
{
	return fire_state;
}
unsigned short Fire_GetTemperature(void)
{
	return adc_buffer[0];
}
unsigned short Fire_GetGas(void)
{
	return adc_buffer[1];
}