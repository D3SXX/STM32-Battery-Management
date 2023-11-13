#include "mux.h"
#include "utils.h"

void mux_init() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;             // Enable GPIOB
    GPIOB->MODER |= (GPIO_MODE_OUTPUT << MODER_MUX_A) |  // Set PB3 to ANALOG mode
                    (GPIO_MODE_OUTPUT << MODER_MUX_B) |  // Set PB5 to ANALOG mode
                    (GPIO_MODE_OUTPUT << MODER_MUX_C);   // Set PB4 to ANALOG mode

    GPIOB->PUPDR |= (0x02 << GPIO_PUPDR_PUPDR3_Pos) |  // Set PB3 internal resister to PULL-DOWN
                    (0x02 << GPIO_PUPDR_PUPDR5_Pos) |  // Set PB5 internal resister to PULL-DOWN
                    (0x02 << GPIO_PUPDR_PUPDR4_Pos);   // Set PB4 internal resister to PULL-DOWN
}

void mux_set(const uint16_t mux_channel) {
    GPIOB->ODR = mux_channel;
    delay_ms(1);
}