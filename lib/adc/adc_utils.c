#include "adc_utils.h"
/**
 * \brief Convert adc reading to cell coltage.
 * \param[in] adc_value The adc readings.
 * \return The cell voltage value in mV.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 * \details This function converts 12 bit ADC value into 0 - 4125mV.
 */
uint16_t adc_convert_cell_voltage(const uint16_t adc_value) {
    double cell_volt = adc_value * 4.125 / 4095;  // Cell Voltage range 0 - 4.125V
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
    double  shunt_current_double = adc_value * 41.25 / 4095;  // current sensing range 0 - 41.25A
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
    double temp_celcius = (adc_value) * 3.3 * 100 / 4095;  // 10mV per Celcius
    return (uint16_t)(temp_celcius * 10);                  // accuracy in 0.1 Celcius
}

/**
 * \brief Find median value of the given array.
 * \param[in] arr The address of the data array.
 * \param[in] len The length of the data array.
 * \return The median value of the data.
 * \author Vladislav R., Nov.2023
 * \details Comments by Siyuan Xu:
 * This function is expensive. Time cost O(N)=N^2. For small data set only.
 */
int find_median(int arr[], int len) {
    int apu;
    /*Sort array and get medium value*/
    for (int k = 0; k < len; k++) {
        for (int i = 0; i < len; i++) {
            if (arr[i] > arr[i + 1]) {
                apu        = arr[i];
                arr[i]     = arr[i + 1];
                arr[i + 1] = apu;
            }
        }
    }
    if (len % 2 == 0) {
        /*If the length is even, return the average of the two middle values*/
        return (arr[len / 2 - 1] + arr[len / 2]) / 2;
    } else {
        /*If the length is odd, return the middle value*/
        return arr[len / 2];
    }
}