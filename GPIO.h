#ifndef _GPIO_H
#define _GPIO_H

#include <stdint.h>
#include "stm32f4xx.h"

// DEFINE BASE ADDRESSED 
#define GPIO_A_BASE 0x40020000UL
#define GPIO_B_BASE 0x40020400UL
#define GPIO_C_BASE 0x40020800UL
#define GPIO_D_BASE 0x40020C00UL
#define GPIO_E_BASE 0x40021000UL
#define GPIO_H_BASE 0x40021C00UL

typedef struct
{
	volatile uint32_t MODER; 		//0X00
	volatile uint32_t OTYPER;		//0X04
	volatile uint32_t OSPEEDR;	//0X08
	volatile uint32_t PUPDR;		//0X0C
	volatile uint32_t IDR;			//0X10
	volatile uint32_t ODR;			//0X14
	volatile uint32_t BSRR;			//0X18
	volatile uint32_t LCKR;			//0X1C
	volatile uint32_t AFRL;			//0X20
	volatile uint32_t AFRH;			//0X24
}GPIO_TYPE;

#define GPIO_A_CONTROL ((GPIO_TYPE*) GPIO_A_BASE)
#define GPIO_B_CONTROL ((GPIO_TYPE*) GPIO_B_BASE)
#define GPIO_C_CONTROL ((GPIO_TYPE*) GPIO_C_BASE)
#define GPIO_D_CONTROL ((GPIO_TYPE*) GPIO_D_BASE)
#define GPIO_E_CONTROL ((GPIO_TYPE*) GPIO_E_BASE)
#define GPIO_H_CONTROL ((GPIO_TYPE*) GPIO_H_BASE)
// Define MODE 
#define GPIO_MODE_INPUT 			0x0
#define GPIO_MODE_OUTPUT 			0x1
#define GPIO_MODE_AF 					0x2
#define GPIO_MODE_ANALOG 			0x3
// Define OTYPER
#define GPIO_OUTPUT_PUSHPULL  0x0
#define GPIO_OUTPUT_OPENDRAIN 0x1
// Define OUTPUT_SPEED
#define GPIO_SPEED_LOW 				0x0
#define GPIO_SPEED_MEDIUM 		0x1
#define GPIO_SPEED_FAST				0x2
#define GPIO_SPEED_HIGH				0x3
// Define Pull-Up and Pull-Down 
#define GPIO_NO_UPDOWN 				0x0
#define GPIO_PULL_UP					0x1
#define GPIO_PULL_DOWN				0x2
#define GPIO_RESERVED					0x3
// FUNCTION PROTOTYPES
void GPIO_SetMode		(GPIO_TYPE *GPIOx, uint8_t pin, uint8_t mode);
void GPIO_SetOutput (GPIO_TYPE *GPIOx, uint8_t pin, uint8_t type); 
void GPIO_SPEED 		(GPIO_TYPE *GPIOx, uint8_t pin, uint8_t speed); 
void GPIO_SetPull 	(GPIO_TYPE *GPIOx, uint8_t pin, uint8_t pull);
void GPIO_Set_AFRH 	(GPIO_TYPE *GPIOx, uint8_t pin, uint8_t afrh); 
void GPIO_Set_AFRL	(GPIO_TYPE *GPIOx, uint8_t pin, uint8_t afrl);

// main function 
void GPIO_Config_SPI1(void); 
void GPIO_Config_Analog(void); 
void GPIO_Config_Buzzer_Output(void);
void GPIO_Config_Relay_Output(void);
// Chip select 
void SPI1_Select(void);
void SPI1_Unselect(void);
#endif //_GPIO_H