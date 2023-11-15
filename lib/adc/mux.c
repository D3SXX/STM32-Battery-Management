#include "mux.h"

#include "..\utils\utils.h"

void mux_init() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  // Enable GPIOB

    GPIOB->MODER &= ~((0b11 << MODER_MUX_A) |  // Reset PB3
                      (0b11 << MODER_MUX_B) |  // Reset PB5
                      (0b11 << MODER_MUX_C));  // Reset PB4

    GPIOB->MODER |= ((GPIO_MODE_OUTPUT << MODER_MUX_A) |  // Set PB3 to OUTPUT mode
                     (GPIO_MODE_OUTPUT << MODER_MUX_B) |  // Set PB5 to OUTPUT mode
                     (GPIO_MODE_OUTPUT << MODER_MUX_C));  // Set PB4 to OUTPUT mode
    // GPIOB->MODER = 0x540U;

    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_3 | GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5);

    GPIOB->OSPEEDR |= (0X11 << GPIO_OSPEEDER_OSPEEDR3_Pos) |  // Set PB3 to VH speed
                      (0X11 << GPIO_OSPEEDER_OSPEEDR4_Pos) |  // Set PB4 to VH speed
                      (0X11 << GPIO_OSPEEDER_OSPEEDR5_Pos);   // Set PB5 to VH speed

    GPIOB->PUPDR |= (0x02 << GPIO_PUPDR_PUPDR3_Pos) |  // Set PB3 internal resister to PULL-DOWN
                    (0x02 << GPIO_PUPDR_PUPDR5_Pos) |  // Set PB5 internal resister to PULL-DOWN
                    (0x02 << GPIO_PUPDR_PUPDR4_Pos);   // Set PB4 internal resister to PULL-DOWN
}

void mux_set(const uint32_t mux_channel) {
    GPIOB->ODR = mux_channel;
}

void mux_reset(void) { GPIOB->ODR = 0u; }

void mux_set_single_pin(const uint32_t gpio_pin) { GPIOB->BSRR |= 1u << gpio_pin; }

void mux_reset_single_pin(const uint32_t gpio_pin) {
    GPIOB->BSRR |= 1 << (gpio_pin + 0x10);  // + 0x10 for reset pin
}