#include "RCC_STM32.h"
#include "GPIO.h"
#include "SPI.h"
#include "DMA.h"
#include "ADC.h"
#include "Define.h"
#include "module_actuator.h" 
#include "Fire_detection.h" 
#include "SPI_Protocol.h" 
int main(void)
{	
	RCC_Configuration();
	GPIO_Config_Analog();
  ADC1_init();
	ADC1_Start();
  uint16_t temp;
	uint16_t gas;
	while(1)
	{
    temp = ADC1_Read_DMA(ADC_CH_LM35);
		gas  = ADC1_Read_DMA(ADC_CH_GAS);
		
		small_delay(1000000);
	}
}
