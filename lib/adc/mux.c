#include "mux.h"

#include "utils.h"

void mux_init() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  // Enable GPIOB

    GPIOB->MODER &= ~MUX_MODER_MASK;

    GPIOB->MODER |= ((GPIO_MODE_OUTPUT << MUX_MODER_A_Pos) |  // Set PB3 to OUTPUT mode
                     (GPIO_MODE_OUTPUT << MUX_MODER_B_Pos) |  // Set PB5 to OUTPUT mode
                     (GPIO_MODE_OUTPUT << MUX_MODER_C_Pos));  // Set PB4 to OUTPUT mode
    // GPIOB->MODER = 0x540U;

    GPIOB->OTYPER &= ~(MUX_OTYPER_MASK);  // Set PB3, PB4, PB5 to push-pull configuration

    GPIOB->OSPEEDR &= ~MUX_OSPEEDR_MASK;
    GPIOB->OSPEEDR |= (0x03 << MUX_OSPEEDR_A_Pos) |  // Set PB3 to VH speed
                      (0x03 << MUX_OSPEEDR_B_Pos) |  // Set PB4 to VH speed
                      (0x03 << MUX_OSPEEDR_C_Pos);   // Set PB5 to VH speed

    GPIOB->PUPDR &= ~MUX_PUPDR_MASK;                   // Reset PUPDR for PB3, PB4, PB5.
    GPIOB->PUPDR |= (0x02 << GPIO_PUPDR_PUPDR3_Pos) |  // Set PB3 internal resister to PULL-DOWN
                    (0x02 << GPIO_PUPDR_PUPDR5_Pos) |  // Set PB5 internal resister to PULL-DOWN
                    (0x02 << GPIO_PUPDR_PUPDR4_Pos);   // Set PB4 internal resister to PULL-DOWN
}

void mux_set(const uint32_t mux_channel) {
    GPIOB->ODR &= ~MUX_IDR_ODR_MASK;           // Always reset before set
    while ((GPIOB->IDR) & (MUX_IDR_ODR_MASK))  // waiting till reset complete
        ;
        
    GPIOB->ODR |= mux_channel;
    while (((GPIOB->IDR) & (MUX_IDR_ODR_MASK)) != mux_channel)
        ;
}

void mux_reset(void) {
    GPIOB->ODR &= ~MUX_IDR_ODR_MASK;
    while ((GPIOB->IDR) & (MUX_IDR_ODR_MASK))  // waiting till reset complete
        ;
}

void mux_set_single_pin(const uint32_t gpio_pin) { GPIOB->BSRR |= (1u << gpio_pin); }

void mux_reset_single_pin(const uint32_t gpio_pin) {
    GPIOB->BSRR |= (1 << (gpio_pin + 0x10));  // + 0x10 for reset pin
}