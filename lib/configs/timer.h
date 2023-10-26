#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#include "stm32l1xx.h"
void timer2_init();
void timer2_start(uint16_t ms);
void timer_stop(TIM_TypeDef *TIMx);
#endif