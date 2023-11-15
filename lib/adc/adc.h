#ifndef ADC_H
#define ADC_H
#include <stdint.h>

#include "adc_settings.h"
#include "adc_utils.h"
#include "stm32l1xx.h"
static inline void adc_power_up(void) { ADC1->CR2 &= ~(ADC_CR2_ADON); }
static inline void adc_power_down(void) { ADC1->CR2 |= (ADC_CR2_ADON); }

void     adc_init(void);
uint16_t adc_read(const uint8_t adc_ch);
#endif