#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdint.h>
#include "stm32f4xx.h"

/* ADC channels order in adc_buf[] */
#define ADC_IDX_LM35   0
#define ADC_IDX_GAS    1
#define ADC_IDX_S3     2
#define ADC_IDX_S4     3

#define ADC_CH_LM35    0   // PA0
#define ADC_CH_GAS     1   // PA1
#define ADC_CH_S3      2   // PA2
#define ADC_CH_S4      3   // PA3

/* SPI packet */
#define PACKET_LEN     16

/* thresholds */
#define TEMP_ALARM_X10     300   // 30.0C
#define GAS_ALARM_ADC      2500  // raw ADC

#endif