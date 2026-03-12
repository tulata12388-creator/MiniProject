#ifndef _FIRE_DETECTION_
#define _FIRE_DETECTION_ 
#include <stdint.h>

void Fire_Update(void);
uint8_t Fire_GetState(void);
uint16_t Fire_GetTemperature(void);
uint16_t Fire_GetGas(void);

#endif /* Fire_detection.h */