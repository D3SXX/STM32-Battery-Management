/*
 * utils.h
 *
 *  Created on: Sep, 2023
 *      Author: Siyuan Xu
 */

#ifndef UTILS_H_
#define UTILS_H_

#define DEBUG_CONSOLE_EN 1
#define DBUG_MSG_LEN     128

void init_delay_ms(void);
void delay_ms(const unsigned long delay);
void debug_console(const char *message);

#endif /* UTILS_H_ */