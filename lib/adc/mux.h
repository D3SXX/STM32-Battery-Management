#ifndef MUX_H
#define MUX_H
#include <stdint.h>

#include "stm32l1xx.h"

#define GPIO_MODE_INPUT     0x00U /*!< Select input mode */
#define GPIO_MODE_OUTPUT    0x01U /*!< Select output mode */
#define GPIO_MODE_ALTERNATE 0x02U /*!< Select alternate function mode */
#define GPIO_MODE_ANALOG    0x03U /*!< Select analog mode */

#define MODER_MUX_A         GPIO_MODER_MODER3_Pos  // PB3 (D3)
#define MODER_MUX_B         GPIO_MODER_MODER5_Pos  // PB5 (D4)
#define MODER_MUX_C         GPIO_MODER_MODER4_Pos  // PB4 (D5)

#define ODR_MUX_A           GPIO_ODR_ODR_3  // PB3 (D3)
#define ODR_MUX_B           GPIO_ODR_ODR_5  // PB5 (D4)
#define ODR_MUX_C           GPIO_ODR_ODR_4  // PB4 (D5)

#define MUX_SEL_CH0         0x00U
#define MUX_SEL_CH1         (ODR_MUX_A)
#define MUX_SEL_CH2         (ODR_MUX_B)
#define MUX_SEL_CH3         (ODR_MUX_A | ODR_MUX_B)
#define MUX_SEL_CH4         (ODR_MUX_C)
#define MUX_SEL_CH5         (ODR_MUX_A | ODR_MUX_C)
#define MUX_SEL_CH6         (ODR_MUX_B | ODR_MUX_C)
#define MUX_SEL_CH7         (ODR_MUX_A | ODR_MUX_B | ODR_MUX_C)

void mux_init(void);
void mux_set(const uint16_t mux_channel);

#endif