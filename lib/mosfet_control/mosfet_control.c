#include "mosfet_control.h"

void mosfet_control_init() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  // Enable GPIOB

    GPIOB->MODER &= ~MOSFET_CONTROL_MODER_MASK;

    GPIOB->MODER |= ((GPIO_MODE_OUTPUT << MOSFET_CONTROL_MODER_G0_Pos) |  // Set PB6 to OUTPUT mode
                     (GPIO_MODE_OUTPUT << MOSFET_CONTROL_MODER_G1_Pos));  // Set PB10 to OUTPUT mode

    // GPIOB->MODER = 0x540U;

    GPIOB->OTYPER &= ~(MOSFET_CONTROL_OTYPER_MASK);  // Set PB6, PB10 to push-pull configuration

    GPIOB->OSPEEDR &= ~MOSFET_CONTROL_OSPEEDR_MASK;
    GPIOB->OSPEEDR |= (0x03 << MOSFET_CONTROL_OSPEEDR_G0_Pos) |  // Set PB3 to VH speed
                      (0x03 << MOSFET_CONTROL_OSPEEDR_G1_Pos);   // Set PB4 to VH speed

    GPIOB->PUPDR &= ~MOSFET_CONTROL_PUPDR_MASK;        // Reset PUPDR for PB3, PB4, PB5.
    GPIOB->PUPDR |= (0x02 << MOSFET_CONTROL_PUPDR_G0_Pos) |  // Set PB6 internal resister to PULL-DOWN
                    (0x02 << MOSFET_CONTROL_PUPDR_G1_Pos);   // Set PB10 internal resister to PULL-DOWN
}

void mosfet_control_set(const uint32_t mosfet_control_state) {
    GPIOB->ODR &= ~MOSFET_CONTROL_IDR_ODR_MASK;           // Always reset before set
    while ((GPIOB->IDR) & (MOSFET_CONTROL_IDR_ODR_MASK))  // waiting till reset complete
        ;

    GPIOB->ODR |= mosfet_control_state;
    while (((GPIOB->IDR) & (MOSFET_CONTROL_IDR_ODR_MASK)) != mosfet_control_state)
        ;
}

void mosfet_control_reset(void) {
    GPIOB->ODR &= ~MOSFET_CONTROL_IDR_ODR_MASK;
    while ((GPIOB->IDR) & (MOSFET_CONTROL_IDR_ODR_MASK))  // waiting till reset complete
        ;
}

void mosfet_control_set_single_pin(const uint32_t gpio_pin) { GPIOB->BSRR |= (1u << gpio_pin); }

void mosfet_control_reset_single_pin(const uint32_t gpio_pin) {
    GPIOB->BSRR |= (1 << (gpio_pin + 0x10));  // + 0x10 for reset pin
}