#include "moc_adc_read.h"

#include "stm32l1xx.h"

uint16_t moc_adc_value = 0;

uint16_t moc_adc_read_cell_voltage(const int cell_index __attribute__((unused))) {
    uint16_t cell_adc  = moc_adc_read();
    double   cell_volt = (double)cell_adc / 4095 * 4.125;  // Cell Voltage range 0 - 4.125V
    return (uint16_t)(cell_volt * 1000);                  // accuracy in 1 mV.
}

uint16_t moc_adc_read_batt_current(void) {
    uint16_t current_adc  = moc_adc_read();
    double   cell_current = (double)current_adc / 4095 * 41.25;  // current sensing range 0 - 41.25A
    return (uint16_t)(cell_current * 1000);                 // accuracy in 1 mA.
}

uint16_t moc_adc_read_batt_temp(void) {
    uint16_t temp_adc = moc_adc_read();
    uint16_t reference_adc = 1240;      // moc 1V reference voltage on LM35
    double temp_celcius = (temp_adc - reference_adc) / 4095 * 3.3;  //
    int16_t  temp_int     = (int16_t)(temp_celcius * 10);       // accuracy in 0.1 Celcius
    uint16_t encoded_temp = (uint16_t)temp_int;         // encode temp to uint16_t for modbus register
    return encoded_temp;
}

/**
 * \brief Set return value for moc_adc_read(). 
 * \param[in] moc_value - The address of the data to be sent
 * \param[in] data_length - The number of bytes
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 * \details Define USE_MOC_ADC_SET 1U to enable.
 */
void moc_adc_set(const uint16_t moc_value) {
#if (USE_MOC_ADC_SET > 0U)
    moc_adc_value = moc_value;
#else
    srand(SysTick->VAL);
    moc_adc_value = (rand() / RAND_MAX) * 4095;
#endif
}

uint16_t moc_adc_read() { return moc_adc_value; }