/*
 * utils.h
 *
 *  Created on: Sep, 2023
 *      Author: Siyuan Xu
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <stdint.h>

#define DEBUG_CONSOLE_EN 1
#define DBUG_MSG_LEN     128

void delay_ms(const uint32_t delay);
void debug_console(const char *message);

#endif /* UTILS_H_ */