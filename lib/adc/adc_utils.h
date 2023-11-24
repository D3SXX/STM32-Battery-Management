#ifndef ADC_UTILS_H
#define ADC_UTILS_H
#include <stdint.h>
#include "adc_settings.h"
uint16_t adc_convert_cell_voltage(const uint16_t adc_value);
uint16_t adc_convert_batt_current(const uint16_t adc_value);
uint16_t adc_convert_batt_temp(const uint16_t adc_value);
#endif