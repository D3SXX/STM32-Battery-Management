/*
 * utils.c
 *
 *  Created on: Feb 24, 2023
 *      Author: Siyuan Xu
 */
#include "utils.h"
#include "usart_config.h"
#include <stdlib.h>

#include "stm32l1xx.h"
/**
 * \brief Delay in miliseconds.
 * \param[in] delay Delay time in miliseconds.
 * \author siyuan xu, e2101066@edu.vamk.fi, Sep.2023
 */
void delay_ms(const uint32_t delay) {
    uint32_t i = 0;

    // systick_init(1000);  // set SysTick frequency to 1000Hz

    while (i < delay) {
        // CTRL register bit 16  returns 1 if timer counted to 0 since last time this was read.
        while (!((SysTick->CTRL) & 0x10000)) {
            asm("NOP");
        }  // M3 Generic User Guide p. 159
        i++;
    }
}

/**
 * \brief Random delay in miliseconds.
 * \param[in] max maximum delay range in miliseconds.
 * \author siyuan xu, e2101066@edu.vamk.fi, Oct.2023
 */
void delay_ms_random(const uint32_t max) {
    srand(SysTick->VAL);                       // Seed the random number generator
    uint32_t delay = (rand() / RAND_MAX) * max;  // random [0 - max] ms
    delay_ms(delay);
}

/**
 * \brief Send debug information
 * \param[in] message - Debug message
 * \author siyuan xu, e2101066@edu.vamk.fi, Mar.2023
 */
void debug_console(const char *message) { USART2_send_string(message); }
