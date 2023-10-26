#include "adc.h"

#include "stm32l1xx.h"

void adc_init(void) {
    // RCC->AHBENR |= (RCC_APB2ENR_ADC1EN);
    // RCC_
    // RCC->CCIPR &= ~(RCC_CCIPR_ADCSEL);
    // RCC->CCIPR |= (3 << RCC_CCIPR_ADCSEL_Pos);

    // GPIOA->MODER |= (0x3 << (1 * 2));
}

void inline adc_power_up() {}

uint16_t adc_read(const int pin) {
    // // ADC1->CR2 &= ~(ADC_CR2_ADON);    //Disable ADC
    // // Enable the ADC voltage regulator.
    // ADC1->CR2 |= (ADC_CR2_ADON);
    // // Delay for a handful of microseconds.
    // delay_cycles(100);
    // // Calibrate the ADC if necessary.
    // if (perform_calibration) {
    //     ADC1->CR |= (ADC_);
    //     while (ADC1->CR & ADC_CR_ADCAL) {
    //     };
    // }
    return 1024;
}

void adc_read_current(_Atomic uint16_t* data) { *data = adc_read(PIN_CURRENT); }
void adc_read_temperature(_Atomic uint16_t* data) { *data = adc_read(PIN_TEMP); }
void adc_read_volts(_Atomic uint16_t* data) {
    for (int i = 0; i < 4; i++) {
        // mux_set(i + 1);
        *(data + i) = adc_read(PIN_VOLT);
    }
}