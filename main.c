#include "board.h"
#include "RCC_STM32_LIB.h""
#include "GPIO.h""
#include "SPI_LIB.h""
#include "DMA_LIB.h""
#include "ADC_LIB.h"
#include "greenhouse.h"

int main(void)
{
    RCC_Enable_For_GPIO_ADC_SPI_DMA();

    GPIO_Config_ADC_PA0_PA3_Analog();
    GPIO_Config_SPI1_PA4_PA7_AF5();
    GPIO_Config_Buzzer_PB0_Output();
    GPIO_Config_Motor_PB1_Output();

    SPI1_Slave_Init();
    Greenhouse_InitPacket();

    ADC1_DMA2_Stream0_InitStart();

    while (1)
    {
        __WFI(); /* ng?, m?i th? ch?y b?ng IRQ */
    }
}