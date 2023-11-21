#ifndef MOSFET_CONTROL_H
#define MOSFET_CONTROL_H
#include <stdint.h>

#include "stm32l1xx.h"

#define GPIO_MODE_INPUT               0x00UL /*!< Select input mode */
#define GPIO_MODE_OUTPUT              0x01UL /*!< Select output mode */
#define GPIO_MODE_G0LTERNATE          0x02UL /*!< Select alternate function mode */
#define GPIO_MODE_G0NALOG             0x03UL /*!< Select analog mode */

#define MOSFET_CONTROL_MODER_G0_Pos   GPIO_MODER_MODER6_Pos  // PB6 (D10)
#define MOSFET_CONTROL_MODER_G1_Pos   GPIO_MODER_MODER10_Pos  // PB10 (D6)

#define MOSFET_CONTROL_OTYPER_G0      GPIO_OTYPER_OT_6
#define MOSFET_CONTROL_OTYPER_G1      GPIO_OTYPER_OT_10

#define MOSFET_CONTROL_PUPDR_G0_Pos   GPIO_PUPDR_PUPDR6_Pos
#define MOSFET_CONTROL_PUPDR_G1_Pos   GPIO_PUPDR_PUPDR10_Pos

#define MOSFET_CONTROL_OSPEEDR_G0_Pos GPIO_OSPEEDER_OSPEEDR6_Pos
#define MOSFET_CONTROL_OSPEEDR_G1_Pos GPIO_OSPEEDER_OSPEEDR10_Pos

#define MOSFET_CONTROL_IDR_ODR_G0     GPIO_ODR_ODR_6  // PB6 (D10)
#define MOSFET_CONTROL_IDR_ODR_G1     GPIO_ODR_ODR_10  // PB10 (D6)

#define MOSFET_CONTROL_MODER_MASK \
    ((0x3 << MOSFET_CONTROL_MODER_G0_Pos) | (0x3 << MOSFET_CONTROL_MODER_G1_Pos))

#define MOSFET_CONTROL_PUPDR_MASK \
    ((0x3 << MOSFET_CONTROL_PUPDR_G0_Pos) | (0x3 << MOSFET_CONTROL_PUPDR_G1_Pos))

#define MOSFET_CONTROL_OSPEEDR_MASK \
    ((0x3 << MOSFET_CONTROL_OSPEEDR_G0_Pos) | (0x3 << MOSFET_CONTROL_OSPEEDR_G1_Pos))

#define MOSFET_CONTROL_OTYPER_MASK  (MOSFET_CONTROL_OTYPER_G0 | MOSFET_CONTROL_OTYPER_G1)
#define MOSFET_CONTROL_IDR_ODR_MASK (MOSFET_CONTROL_IDR_ODR_G0 | MOSFET_CONTROL_IDR_ODR_G1)

#define MOSFET_CONTROL_CLOSED       0x00U
#define MOSFET_CONTROL_OPEN         (MOSFET_CONTROL_IDR_ODR_G0 | MOSFET_CONTROL_IDR_ODR_G1)
#define MOSFET_CONTROL_CHARGE       (MOSFET_CONTROL_IDR_ODR_G0)
#define MOSFET_CONTROL_DISCHARGE    (MOSFET_CONTROL_IDR_ODR_G1)

void mosfet_control_init(void);
void mosfet_control_set(const uint32_t mosfet_control_state);
void mosfet_control_reset(void);
void mosfet_control_set_single_pin(const uint32_t gpio_pin);
void mosfet_control_reset_single_pin(const uint32_t gpio_pin);
#endif