#ifndef _ACTUATORS_H_
#define _ACTUATORS_H_

#include "stm32f4xx.h"
#include <stdint.h>

void Buzzer_Set(uint8_t on);
void Motor_Set(uint8_t on);

uint8_t Buzzer_Get(void);
uint8_t Motor_Get(void);

#endif