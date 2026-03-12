#include "module_actuator.h" 
#include "GPIO.h" 

void Buzzer_On(void)
{
	GPIO_B_CONTROL->BSRR = (1<<0);
}
void Buzzer_Off(void)
{
	GPIO_B_CONTROL->BSRR = (1<<(0+16));
}	

void Fan_On(void)
{
	GPIO_B_CONTROL->BSRR = (1<<2);
}

void Fan_Off(void)
{
	GPIO_B_CONTROL->BSRR = (1<<(2+16));
}