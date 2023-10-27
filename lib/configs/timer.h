/*
 * sysclock_config.h
 *
 *  Created on: Oct 27, 2023
 *      Author: siyxu
 */

#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#include "stm32l1xx.h"
void timer2_init();
void timer2_start(const uint16_t frequency_hz, const uint16_t interval_ms);
void timer_stop(TIM_TypeDef *TIMx);
#endif