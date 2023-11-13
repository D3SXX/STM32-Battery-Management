#ifndef MOC_ADC_READ_H
#define MOC_ADC_READ_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

uint16_t moc_adc_read_cell_voltage(const int cell_index __attribute__((unused)));
uint16_t moc_adc_read_batt_current(void);
uint16_t moc_adc_read_batt_temp(void);
void moc_adc_set(const uint16_t);
uint16_t moc_adc_read(uint8_t);

#endif