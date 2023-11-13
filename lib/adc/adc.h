#ifndef ADC_H
#define ADC_H
#include <stdint.h>

#include "stm32l1xx.h"

#define GPIO_MODE_INPUT         0x00U /*!< Select input mode */
#define GPIO_MODE_OUTPUT        0x01U /*!< Select output mode */
#define GPIO_MODE_ALTERNATE     0x02U /*!< Select alternate function mode */
#define GPIO_MODE_ANALOG        0x03U /*!< Select analog mode */
#define PA0                     0U
#define PA1                     1U
#define PA4                     4U
#define PC1                     1U
#define ADC_INPUT_BATT_CURRENT  PA0
#define ADC_INPUT_BATT_TEMP     PA0
#define ADC_INPUT_CELL_VOLTAGE  PA0
#define PIN_CURRENT             1
#define PIN_TEMP                2
#define PIN_VOLT                3
#define LM35_VOLT_REF           (double)1.0
#define LM35_VOLT_MAX           (double)(LM35_VOLT_REF + 1.5)
#define LM35_VOLT_MIN           (double)(LM35_VOLT_REF - 0.55)
#define LM35_VOLT_TO_ADC(volt)  (volt * 4095 / 3.3)
#define LM35_ADC_REF            (uint16_t) LM35_VOLT_TO_ADC(LM35_VOLT_REF)
#define LM35_ADC_MAX            (uint16_t) LM35_VOLT_TO_ADC(LM35_VOLT_MAX)
#define LM35_ADC_MIN            (uint16_t) LM35_VOLT_TO_ADC(LM35_VOLT_MIN)
#define LM35_VOLT_TO_TEMP(volt) ((volt - LM35_VOLT_REF) / 0.01)

#define SHUNT_RESISTANCE        (double)0.01
void     adc_init(void);
uint16_t adc_read(const uint8_t adc_ch);
uint16_t adc_convert_cell_voltage(uint16_t (*adc_read)(const uint8_t adc_channel));
uint16_t adc_convert_batt_current(const uint16_t adc_value);
uint16_t adc_convert_batt_temp(uint16_t (*adc_read)(const uint8_t adc_channel));
void inline adc_power_up() { ADC1->CR2 &= ~(ADC_CR2_ADON); }
void inline adc_power_down() { ADC1->CR2 |= (ADC_CR2_ADON); }
#endif