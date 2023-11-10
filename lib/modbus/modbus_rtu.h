#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include "modbus_rtu_func_4.h"
#include "modbus_rtu_types.h"
#include "usart_config.h"

extern void debug_console(const char *message);  // user needs to implement

MODBUS_RTU_ERR modbusRtu_ParseRequest(const uint8_t *const modbus_rtu_frame);
MODBUS_RTU_ERR modbusRtu_AddressValidation(const uint8_t address);
MODBUS_RTU_ERR modbusRtu_FunctionCodeValidation(const uint8_t function_code);
MODBUS_RTU_ERR modbusRtu_RegisterAddressValidation(const uint16_t reg_addr, const uint16_t reg_qty);
MODBUS_RTU_ERR modbusRtu_CrcCheck(const uint8_t *const modbus_rtu_frame);
void modbusRtu_ErrorReply(const uint8_t *const modbus_rtu_frame,
                                    const MODBUS_RTU_ERR modbus_exception_code);
void modbusRtu_Reply(const uint8_t *const modbus_rtu_frame, const uint8_t *data,
                               const uint8_t data_len);
modbus_rtu_rqst_t modbus_rtu_create(void);
#endif
