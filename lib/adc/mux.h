#ifndef MUX_H
#define MUX_H
#include <stdint.h>

#include "stm32l1xx.h"

#define GPIO_MODE_INPUT     0x00UL /*!< Select input mode */
#define GPIO_MODE_OUTPUT    0x01UL /*!< Select output mode */
#define GPIO_MODE_ALTERNATE 0x02UL /*!< Select alternate function mode */
#define GPIO_MODE_ANALOG    0x03UL /*!< Select analog mode */

#define MUX_MODER_A_Pos     GPIO_MODER_MODER3_Pos  // PB3 (D3)
#define MUX_MODER_B_Pos     GPIO_MODER_MODER5_Pos  // PB5 (D4)
#define MUX_MODER_C_Pos     GPIO_MODER_MODER4_Pos  // PB4 (D5)

#define MUX_OTYPER_A        GPIO_OTYPER_OT_3
#define MUX_OTYPER_B        GPIO_OTYPER_OT_5
#define MUX_OTYPER_C        GPIO_OTYPER_OT_4

#define MUX_PUPDR_A_Pos     GPIO_PUPDR_PUPDR3_Pos
#define MUX_PUPDR_B_Pos     GPIO_PUPDR_PUPDR5_Pos
#define MUX_PUPDR_C_Pos     GPIO_PUPDR_PUPDR4_Pos

#define MUX_OSPEEDR_A_Pos   GPIO_OSPEEDER_OSPEEDR3_Pos
#define MUX_OSPEEDR_B_Pos   GPIO_OSPEEDER_OSPEEDR5_Pos
#define MUX_OSPEEDR_C_Pos   GPIO_OSPEEDER_OSPEEDR4_Pos

#define MUX_IDR_ODR_A       GPIO_ODR_ODR_3  // PB3 (D3)
#define MUX_IDR_ODR_B       GPIO_ODR_ODR_5  // PB5 (D4)
#define MUX_IDR_ODR_C       GPIO_ODR_ODR_4  // PB4 (D5)

#define MUX_MODER_MASK \
    ((0x3 << MUX_MODER_A_Pos) | (0x3 << MUX_MODER_B_Pos) | (0x3 << MUX_MODER_C_Pos))

#define MUX_PUPDR_MASK \
    ((0x3 << MUX_PUPDR_A_Pos) | (0x3 << MUX_PUPDR_B_Pos) | (0x3 << MUX_PUPDR_C_Pos))

#define MUX_OSPEEDR_MASK \
    ((0x3 << MUX_OSPEEDR_A_Pos) | (0x3 << MUX_OSPEEDR_B_Pos) | (0x3 << MUX_OSPEEDR_C_Pos))

#define MUX_OTYPER_MASK  (MUX_OTYPER_A | MUX_OTYPER_B | MUX_OTYPER_C)
#define MUX_IDR_ODR_MASK (MUX_IDR_ODR_A | MUX_IDR_ODR_B | MUX_IDR_ODR_C)

#define MUX_SEL_CH0      0x00U
#define MUX_SEL_CH1      (MUX_IDR_ODR_A)
#define MUX_SEL_CH2      (MUX_IDR_ODR_B)
#define MUX_SEL_CH3      (MUX_IDR_ODR_A | MUX_IDR_ODR_B)
#define MUX_SEL_CH4      (MUX_IDR_ODR_C)
#define MUX_SEL_CH5      (MUX_IDR_ODR_A | MUX_IDR_ODR_C)
#define MUX_SEL_CH6      (MUX_IDR_ODR_B | MUX_IDR_ODR_C)
#define MUX_SEL_CH7      (MUX_IDR_ODR_A | MUX_IDR_ODR_B | MUX_IDR_ODR_C)

void mux_init(void);
void mux_set(const uint32_t mux_channel);
void mux_reset(void);
void mux_set_single_pin(const uint32_t gpio_pin);
void mux_reset_single_pin(const uint32_t gpio_pin);
#endif