#include "RCC_STM32.h" 
void RCC_Configuration(void)
{
	// enable HSE on and wait for ready 
    CLK_CONTROL->CR |= (1<<16);

    while(!((CLK_CONTROL->CR >> 17) & 0x1))
    {
        // wait
    }
	// 2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
		CLK_CONTROL->APB1ENR |= (1<<28);
	// 3. Configure the FLASH PREFETCH and the LATENCY Related Setting
		FLASH_CONTROL->CR = 0;
  // 4. Configure the Prescalarts HCLK, PCLK1, PCLK2
		CLK_CONTROL->CFGR &= ~(0xF << 4); 
		CLK_CONTROL->CFGR &= ~(0x7 << 10); 
		CLK_CONTROL->CFGR	&= ~(0x7 << 13);
	// HSE 
		CLK_CONTROL->CFGR &= ~(0x3 << 0);
		CLK_CONTROL->CFGR |=  (0x1 << 0);
	// Wait until HSE ON 
		while(!((CLK_CONTROL->CFGR >> 2) & 0x1))
		{
			//wait
		}
}

