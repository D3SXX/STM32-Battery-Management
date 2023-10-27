#ifndef MOC_ADC_READ_H
#define MOC_ADC_READ_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

uint16_t moc_adc_read_cell_voltage(void);
uint16_t moc_adc_read_batt_current(void);
uint16_t moc_adc_read_batt_temp(void);

#endif