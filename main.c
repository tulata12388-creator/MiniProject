#include "RCC_STM32.h"
#include "GPIO.h"
#include "SPI.h"
#include "DMA.h"
#include "ADC.h"
#include "Define.h"
#include "module_actuator.h" 
#include "Fire_detection.h" 
#include "SPI_Protocol.h"
uint8_t tx = 0x55;
uint8_t rx;
int main(void)
{	
	RCC_Configuration();
  GPIO_Config_SPI1();   
	SPI1_Init_Master();
	/*GPIO_Config_Analog();
	DMA2_Stream0_Config_ADC(adc_buffer,4);
	SPI1_Init();
  ADC1_init();
	ADC1_Start();
	
  uint16_t temp;
	uint16_t gas;*/
	
	//uint8_t spi_packet[2];
	while(1)
	{
    /*temp = ADC1_Read_DMA(ADC_CH_LM35);
		gas  = ADC1_Read_DMA(ADC_CH_GAS);
		
		spi_packet[0] = temp >> 4; 
		spi_packet[1] = gas  >> 4; 
		
		SPI1_SetTxBuffer(spi_packet,2); */
		
		//SPI1_Select();

    rx = SPI1_Transfer(tx);

    //SPI1_Unselect();

    //small_delay(1000000);
	}
}
