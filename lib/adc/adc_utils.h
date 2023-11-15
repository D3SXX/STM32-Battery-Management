#ifndef ADC_UTILS_H
#define ADC_UTILS_H
#include <stdint.h>
#include "adc_settings.h"
uint16_t adc_convert_cell_voltage(uint16_t (*adc_read)(const uint8_t adc_channel));
uint16_t adc_convert_batt_current(const uint16_t adc_value);
uint16_t adc_convert_batt_temp(uint16_t (*adc_read)(const uint8_t adc_channel));
#endif