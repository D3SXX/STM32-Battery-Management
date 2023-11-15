#ifndef ADC_SETTINGS_H
#define ADC_SETTINGS_H
/*MCU settings*/
#define GPIO_MODE_INPUT     0x00U /*!< Select input mode */
#define GPIO_MODE_OUTPUT    0x01U /*!< Select output mode */
#define GPIO_MODE_ALTERNATE 0x02U /*!< Select alternate function mode */
#define GPIO_MODE_ANALOG    0x03U /*!< Select analog mode */
#define PA0                 0U
#define PA1                 1U
#define PA4                 4U
#define PC1                 1U

/*User settings*/
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
#endif