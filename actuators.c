#include "actuators.h"

void Buzzer_Set(uint8_t on)
{
    if (on) GPIOB->ODR |=  (1U<<0);
    else    GPIOB->ODR &= ~(1U<<0);
}

void Motor_Set(uint8_t on)
{
    if (on) GPIOB->ODR |=  (1U<<1);
    else    GPIOB->ODR &= ~(1U<<1);
}

uint8_t Buzzer_Get(void){ return (GPIOB->ODR >> 0) & 1U; }
uint8_t Motor_Get(void) { return (GPIOB->ODR >> 1) & 1U; }