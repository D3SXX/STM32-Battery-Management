#ifndef MODBUS_RTU_FUNC_4_H
#define MODBUS_RTU_FUNC_4_H
#include "modbus_rtu.h"

#define MODBUS_REGISTER_NUMBER 10

typedef enum {
    REG_ADDR_CELL1_VOLT = 0,
    REG_ADDR_CELL2_VOLT,
    REG_ADDR_CELL3_VOLT,
    REG_ADDR_CELL4_VOLT,
    REG_ADDR_BATT_CURRENT,
    REG_ADDR_BATT_TEMP,
} MODBUS_REGISTER_ADDRESS;

_Atomic uint16_t modbus_registers[MODBUS_REGISTER_NUMBER];

void modbusRtu_SendData(const uint8_t *const data, const size_t data_length);
MODBUS_RTU_ERR modbusRtu_ReadInputRegister(const uint8_t *const modbus_rtu_frame,
                                           uint8_t *reply_data, uint8_t *reply_data_len);

#endif