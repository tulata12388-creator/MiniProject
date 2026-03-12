#ifndef _DEFINE_H
#define _DEFINE_H

#include <stdint.h>

void small_delay(volatile uint32_t t);
/* Define */ 
#define ADC_CH_LM35    0   // PA0
#define ADC_CH_GAS     1   // PA1
#define ADC_CH_S3      2   // PA2
#define ADC_CH_S4      3   // PA3

#endif