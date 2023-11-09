/*
 * sysclock_config.h
 *
 *  Created on: Feb 24, 2023
 *      Author: siyxu
 */

#ifndef SYSCLOCK_CONFIG_H_
#define SYSCLOCK_CONFIG_H_
#include "stm32l1xx.h"
void SystemClock_Config(void);
void SystemClock_HSI_Config(void);
void systick_init(const uint32_t frequency_hz);
#endif /* SYSCLOCK_CONFIG_H_ */
