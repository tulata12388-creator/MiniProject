#include "GPIO.h"
#include "RCC_STM32.h" 

/* Define basic function */
void GPIO_SetMode (GPIO_TYPE *GPIOx, uint8_t pin, uint8_t mode)
{
	GPIOx->MODER &= ~(0x3  << (pin * 2));
	GPIOx->MODER |=  (mode << (pin * 2));
}
void GPIO_SetOutput (GPIO_TYPE *GPIOx, uint8_t pin, uint8_t type)
{
	GPIOx->OTYPER &= ~(1    << pin);
	GPIOx->OTYPER |=  (type << pin);
}
void GPIO_SPEED 		(GPIO_TYPE *GPIOx, uint8_t pin, uint8_t speed)
{
	GPIOx->OSPEEDR &= ~(0x3   << (pin * 2));
	GPIOx->OSPEEDR |=  (speed << (pin * 2));
}
void GPIO_SetPull 	(GPIO_TYPE *GPIOx, uint8_t pin, uint8_t pull)
{
	GPIOx->PUPDR  &= ~(0x3   << (pin * 2));
	GPIOx->PUPDR  |=  (pull  << (pin * 2));
}
void GPIO_Set_AFRH 	(GPIO_TYPE *GPIOx, uint8_t pin, uint8_t afrh)
{
	GPIOx->AFRH  &= ~(0xF   << (pin * 4));
	GPIOx->AFRH  |=  (afrh  << (pin * 4));
}
void GPIO_Set_AFRL 	(GPIO_TYPE *GPIOx, uint8_t pin, uint8_t afrl)
{
	GPIOx->AFRL  &= ~(0xF   << (pin * 4));
	GPIOx->AFRL  |=  (afrl  << (pin * 4));
}
/* Config main function */
void GPIO_Config_Analog(void)
{
	CLK_CONTROL->AHB1ENR |= (1 << 0);
	GPIO_SetMode (GPIO_A_CONTROL, 0, GPIO_MODE_ANALOG); 
	GPIO_SetMode (GPIO_A_CONTROL, 1, GPIO_MODE_ANALOG);
	GPIO_SetMode (GPIO_A_CONTROL, 2, GPIO_MODE_ANALOG);
	GPIO_SetMode (GPIO_A_CONTROL, 3, GPIO_MODE_ANALOG);
}

void GPIO_Config_Buzzer_Output(void)
{
	CLK_CONTROL->AHB1ENR |= (1 << 1); 
	GPIO_SetMode 		(GPIO_B_CONTROL, 0, GPIO_MODE_OUTPUT);
	GPIO_SetOutput	(GPIO_B_CONTROL, 0, GPIO_OUTPUT_PUSHPULL);
	GPIO_SPEED			(GPIO_B_CONTROL, 0, GPIO_SPEED_LOW);
	GPIO_SetPull		(GPIO_B_CONTROL, 0, GPIO_NO_UPDOWN);
	
	GPIO_B_CONTROL->BSRR = (1 << (0+16)); 
}
void GPIO_Config_Relay_Output(void)
{
	CLK_CONTROL->AHB1ENR |= (1 << 1);
	GPIO_SetMode 		(GPIO_B_CONTROL, 2, GPIO_MODE_OUTPUT);
	GPIO_SetOutput	(GPIO_B_CONTROL, 2, GPIO_OUTPUT_PUSHPULL);
	GPIO_SPEED			(GPIO_B_CONTROL, 2, GPIO_SPEED_LOW);
	GPIO_SetPull		(GPIO_B_CONTROL, 2, GPIO_NO_UPDOWN);
	
	GPIO_B_CONTROL->BSRR = (1 << (2+16)); 
}
void GPIO_Config_SPI1(void)
{
	/* Clock Enable */
	CLK_CONTROL->AHB1ENR |= (1 << 0);
	CLK_CONTROL->APB2ENR |= (1 << 12);
	/* Mode */
	GPIO_SetMode 		(GPIO_A_CONTROL, 4, GPIO_MODE_OUTPUT); // PA4 NSS
	GPIO_SetMode 		(GPIO_A_CONTROL, 5, GPIO_MODE_AF); // PA5 SCK
	GPIO_SetMode 		(GPIO_A_CONTROL, 6, GPIO_MODE_AF); // PA6 MISO
	GPIO_SetMode 		(GPIO_A_CONTROL, 7, GPIO_MODE_AF); // PA7 MOSI
	/* AF */
	GPIO_Set_AFRL		(GPIO_A_CONTROL, 5, 5);
	GPIO_Set_AFRL		(GPIO_A_CONTROL, 6, 5);
	GPIO_Set_AFRL		(GPIO_A_CONTROL, 7, 5);
	/* Output Mode */
	GPIO_SetOutput	(GPIO_A_CONTROL, 5, GPIO_OUTPUT_PUSHPULL);
	GPIO_SetOutput	(GPIO_A_CONTROL, 7, GPIO_OUTPUT_PUSHPULL);
	/* Speed */
	GPIO_SPEED			(GPIO_A_CONTROL, 4, GPIO_SPEED_HIGH);
	GPIO_SPEED			(GPIO_A_CONTROL, 5, GPIO_SPEED_HIGH);
	GPIO_SPEED			(GPIO_A_CONTROL, 6, GPIO_SPEED_HIGH);
	GPIO_SPEED			(GPIO_A_CONTROL, 7, GPIO_SPEED_HIGH);
	/* Pull */
	GPIO_SetPull		(GPIO_A_CONTROL, 4, GPIO_PULL_UP);
	GPIO_SetPull		(GPIO_A_CONTROL, 5, GPIO_PULL_DOWN);
	GPIO_SetPull		(GPIO_A_CONTROL, 6, GPIO_NO_UPDOWN);
	GPIO_SetPull		(GPIO_A_CONTROL, 7, GPIO_NO_UPDOWN);
}
void SPI1_Select(void)
{
    GPIO_A_CONTROL->BSRR = (1<<(4+16)); // NSS LOW
}

void SPI1_Unselect(void)
{
    GPIO_A_CONTROL->BSRR = (1<<4); // NSS HIGH
}


