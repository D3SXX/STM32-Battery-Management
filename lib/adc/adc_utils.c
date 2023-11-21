#include "adc_utils.h"
/**
 * \brief Set return value for moc_adc_read().
 * \param[in] adc_read The function for reading ADC values.
 * \return The cell voltage value in mV.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 * \details This function converts 12 bit ADC value into 0 - 4125mV.
 */
uint16_t adc_convert_cell_voltage(uint16_t (*adc_read)(const uint8_t adc_channel)) {
    uint16_t cell_adc  = (*adc_read)(ADC_INPUT_CELL_VOLTAGE);
    double   cell_volt = (double)cell_adc * 4.125 / 4095;  // Cell Voltage range 0 - 4.125V
    return (uint16_t)(cell_volt * 1000);                   // accuracy in 1 mV.
}

/**
 * \brief Set return value for moc_adc_read().
 * \param[in] adc_read The function for reading ADC values.
 * \return The battery current value in mA.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 * \details This function converts 12 bit ADC value into 0 - 41250mA.
 */
uint16_t adc_convert_batt_current(uint16_t adc_value) {
    double shunt_volt = (double)adc_value * 4.125 / 4095;  // current sensing range 0 - 4.125V
    double shunt_current_double = shunt_volt * 10;
    int    shunt_current_int    = (int)(shunt_current_double * 1000);  // accuracy in 1 mA.
    return (uint16_t)(shunt_current_int);
}

/**
 * \brief Set return value for moc_adc_read().
 * \param[in] adc_read The function for reading ADC values.
 * \return The battery temperature in 0.1 Celcius.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 * \details This function converts 12 bit ADC value into -50 - 150C.
 */
uint16_t adc_convert_batt_temp(uint16_t (*adc_read)(const uint8_t adc_channel)) {
    uint16_t temp_adc = (*adc_read)(ADC_INPUT_BATT_TEMP);
    uint16_t reference_adc =
        LM35_ADC_REF;  // 1V reference voltage on LM35, need to be measured still.
    double   temp_celcius = (temp_adc - reference_adc) * 3.3 * 100 / 4095;  //
    int      temp_int     = (int)(temp_celcius * 10);  // accuracy in 0.1 Celcius
    uint16_t encoded_temp = (uint16_t)temp_int;  // encode temp to uint16_t for modbus register
    return encoded_temp;
}