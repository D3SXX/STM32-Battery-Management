/*
 * utils.c
 *
 *  Created on: Feb 24, 2023
 *      Author: Siyuan Xu
 */
#include "utils.h"
#include "usart_config.h"
#include "stm32l1xx.h"

void delay_ms(const unsigned long delay) {
    unsigned long i = 0;

    while (i < delay) {
        // CTRL register bit 16  returns 1 if timer counted to 0 since last time this was read.
        while (!((SysTick->CTRL) & 0x10000)) {
        }  // M3 Generic User Guide p. 159
        i++;
    }
}

void init_delay_ms(void){
    SysTick->LOAD   = 32000 - 1;  // 32 000 000 - 1 = 1s so 32000 - 1 = ms, 32 - 1= 1 us
    SysTick->VAL    = 0;
    // enable counter, use processor clock, M3 Generic User Guide p. 159
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |     //Set SysTick->CTRL = 0b111;
                   SysTick_CTRL_TICKINT_Msk|
                   SysTick_CTRL_ENABLE_Msk;    
}

/**
 * \brief Send debug information
 * \param[in] message - Debug message
 * \author siyuan xu, e2101066@edu.vamk.fi, Mar.2023
 */
void debug_console(const char *message) { USART2_send_string(message); }
