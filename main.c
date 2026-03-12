#include "RCC_STM32.h"
#include "GPIO.h"
#include "SPI.h"
#include "DMA.h"
#include "ADC.h"
#include "Define.h"
int main(void)
{	
	RCC_Configuration();
	GPIO_Config_Analog();
	small_delay (1000);
  ADC1_init();
  uint16_t adc_test[4];
	while(1)
	{
    ADC1_Read_Polling(adc_test);
	}
}
