#ifndef ADC_H
#define ADC_H
#include <stdatomic.h>
#include <stdint.h>

#define PIN_CURRENT 1
#define PIN_TEMP    2
#define PIN_VOLT    3

void     adc_read_current(_Atomic uint16_t* data);
void     adc_read_temperature(_Atomic uint16_t* data);
void     adc_read_volts(_Atomic uint16_t* data);
uint16_t adc_read(const int pin);
void     adc_init(void);
#endif