#ifndef MOC_ADC_READ_H
#define MOC_ADC_READ_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

bool adc_read_cell_voltage(int cell_number, uint16_t* data);
bool adc_read_batt_current(uint16_t* data);
bool adc_read_batt_temp(uint16_t* data);

#endif