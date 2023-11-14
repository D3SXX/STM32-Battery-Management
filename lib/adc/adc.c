#include "adc.h"

#define HSI_STARTUP_TIMEOUT   ((uint16_t)0x0500) /* Time out for HSI start up */

/**
 * \brief Initialise ADC settings.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 * \details ADC channel 0, PA0 (A0) in single conversion mode.
 */
void adc_init(void) {
    __IO uint32_t StartUpCounter = 0, HSIStatus = 0;

    /* Enable HSI */
    RCC->CR |= ((uint32_t)RCC_CR_HSION);

    /* Wait till HSI is ready and if Time out is reached exit */
    do {
        HSIStatus = RCC->CR & RCC_CR_HSIRDY;
    } while ((HSIStatus == 0) && (StartUpCounter != HSI_STARTUP_TIMEOUT));
    /* PA0 (A0) -- MUX (CH0)
       PA1 (A1) -- Temp_P ?
       PA4 (A2) -- Temp_N ?
       PC1 (A4) -- Current ?
    */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;              // Enable GPIOA
    GPIOA->MODER |= (GPIO_MODE_ANALOG << PA0 * 2);  // Set PA0 to ANALOG mode
    // GPIOA->MODER |= (GPIO_MODE_ANALOG << PA1 * 2);
    // GPIOA->MODER |= (GPIO_MODE_ANALOG << PA4 * 2);

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC
    GPIOB->MODER |= (GPIO_MODE_ANALOG << PC1 * 2);

    /* ADC configurations */
    RCC->APB2ENR |= (RCC_APB2ENR_ADC1EN);         // Enable ADC1 clock.
    ADC1->CR2 &= ~ADC_CR2_CONT;                   // Set Single Conversion.
    ADC1->SMPR3 |= (0x07 << ADC_SMPR3_SMP0_Pos);  // Set ADC->CH0 sample time to max 384 cycles
    // ADC1->SMPR3 |= (0x07 << ADC_SMPR3_SMP1_Pos) |   //Set ADC->CH1 and CH2 as well..
    //                 (0x07 << ADC_SMPR3_SMP2_Pos)
    ADC1->CR1 &= 0x03 << ADC_CR1_RES_Pos;  // Set resolution to 12 bit
}

/**
 * \brief Read single conversion result from selected ADC channel.
 * \param[in] adc_channel The selected channel for conversion.
 * \return 12 bit ADC value.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 * \details The selected channel should be already enabled.
 */
uint16_t adc_read(const uint8_t adc_channel) {
    int result = 0;
    ADC1->CR2 &= ~(ADC_CR2_ADON);  // Make sure ADC is diabled.
    // Enable the ADC voltage regulator.
    ADC1->SQR5 = adc_channel;      // Set conversion sequence 1 to ch0.
    ADC1->CR2 |= ADC_CR2_ADON;     // Set bit 0, ADC on/off (1=on, 0=off).
    ADC1->CR2 |= ADC_CR2_SWSTART;  // start conversion.
    while (!(ADC1->SR & ADC_SR_EOC)) {
    }                   // wait for conversion complete
    result = ADC1->DR;  // read conversion result
    return result;
}

/**
 * \brief Set return value for moc_adc_read().
 * \param[in] adc_read The function for reading ADC values.
 * \return The cell voltage value.
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
 * \return The battery charging current value.
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
 * \return The cell voltage value.
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