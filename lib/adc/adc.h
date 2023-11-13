#ifndef ADC_H
#define ADC_H
#include <stdint.h>

#include "stm32l1xx.h"

#define GPIO_MODE_INPUT        0x00U /*!< Select input mode */
#define GPIO_MODE_OUTPUT       0x01U /*!< Select output mode */
#define GPIO_MODE_ALTERNATE    0x02U /*!< Select alternate function mode */
#define GPIO_MODE_ANALOG       0x03U /*!< Select analog mode */
#define PA0                    0U
#define PA1                    1U
#define PA4                    4U
#define PC1                    1U
#define ADC_INPUT_BATT_CURRENT PA0
#define ADC_INPUT_BATT_TEMP    PA0
#define ADC_INPUT_CELL_VOLTAGE PA0
#define PIN_CURRENT            1
#define PIN_TEMP               2
#define PIN_VOLT               3

void     adc_init(void);
uint16_t adc_read(const uint8_t adc_ch);
uint16_t adc_convert_cell_voltage(uint16_t (*adc_read)(const uint8_t adc_channel));
uint16_t adc_convert_batt_current(uint16_t (*adc_read)(const uint8_t adc_channel));
uint16_t adc_convert_batt_temp(uint16_t (*adc_read)(const uint8_t adc_channel));
void inline adc_power_up() { ADC1->CR2 &= ~(ADC_CR2_ADON); }
void inline adc_power_down() { ADC1->CR2 |= (ADC_CR2_ADON); }
#endif