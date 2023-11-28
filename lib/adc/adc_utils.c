#include "adc_utils.h"
/**
 * \brief Convert adc reading to cell coltage.
 * \param[in] adc_value The adc readings.
 * \return The cell voltage value in mV.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 * \details This function converts 12 bit ADC value into 0 - 4125mV.
 */
uint16_t adc_convert_cell_voltage(const uint16_t adc_value) {
    double cell_volt = adc_value * ADC_VOLT_MAX / 4095;  // Cell Voltage range 0 - 4.125V
    return (uint16_t)(cell_volt * 1000);          // accuracy in 1 mV.
}

/**
 * \brief Convert adc reading to battery current.
 * \param[in] adc_value The adc readings.
 * \return The battery current value in mA.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 * \details This function converts 12 bit ADC value into 0 - 41250mA.
 */
uint16_t adc_convert_batt_current(uint16_t adc_value) {
    double  shunt_current_double = adc_value * ADC_CURRENT_MAX / 4095;  // current sensing range 0 - 41.25A
    int16_t shunt_current_int    = (int16_t)(shunt_current_double * 1000);  // accuracy in 1 mA.
    return (uint16_t)(shunt_current_int);
}

/**
 * \brief Convert adc value to temperature.
 * \param[in] adc_value The adc readings.
 * \return The battery temperature in 0.1 Celcius.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 * \details This function converts 12 bit ADC value into -50 - 150C.
 */
uint16_t adc_convert_batt_temp(const uint16_t adc_value) {
    double temp_celcius = (adc_value) * ADC_TEMP_MAX / 4095;  // 10mV per Celcius
    return (uint16_t)(temp_celcius * 10);                  // accuracy in 0.1 Celcius
}