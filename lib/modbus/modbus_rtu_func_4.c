#include "modbus_rtu_func_4.h"

#include <stdbool.h>

#include "moc_adc_read.h"
#include "modbus_rtu.h"
#include "usart_config.h"

/**
 * \brief Local implementation for sending Modbus RTU data
 * \param[in] data - The address of the data to be sent
 * \param[in] data_length - The number of bytes
 * \author siyuan xu, e2101066@edu.vamk.fi, Mar.2023
 */
void modbusRtu_SendData(const uint8_t *const data, const size_t data_length) {
    rs485_send_data(data, data_length);
}

/**
 * \brief Local implementation for reading input register for Modbus RTU
 * \param[in] modbus_rtu_frame - Address + PDU + CRC, PDU = Function code + Data
 * \param[out] reply_data - The address of the data to be sent
 * \param[out] reply_data_len - The length of the data
 * \author siyuan xu, e2101066@edu.vamk.fi, Sep.2023
 */
MODBUS_RTU_ERR modbusRtu_ReadInputRegister(const uint8_t *const modbus_rtu_frame,
                                           uint8_t *reply_data, uint8_t *reply_data_len) {
    uint16_t register_addr = ((uint16_t)modbus_rtu_frame[START_ADDRESS_HI] << 8) |
                             (uint16_t)modbus_rtu_frame[START_ADDRESS_LOW];
    uint16_t register_quantity = ((uint16_t)modbus_rtu_frame[REG_QUANTITY_HI] << 8) |
                                 (uint16_t)modbus_rtu_frame[REG_QUANTITY_LOW];
    MODBUS_RTU_ERR err;
    bool           result = false;
    uint16_t       data;
#if (DEBUG_CONSOLE_EN > 0u)
    char debug_msg[100];
#endif
    err = modbusRtu_RegisterAddressValidation(register_addr);
    if (err != MODBUS_RTU_ERR_SUCCESS) {
        return MODBUS_RTU_ERR_BAD_REGISTER_ADDR;
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("BAD REGISTER ADDR:");
#endif
    } else {
        switch (register_addr) {
            case REG_ADDR_CELL1_VOLT:
                result = adc_read_cell_voltage(1, &data);
                if (result != true) {
                    err = MODBUS_RTU_ERR_NOT_SUPPORTED;
#if (DEBUG_CONSOLE_EN > 0u)
                    debug_console("BAD REGISTER ADDR:");
#endif
                } else {
                    reply_data[0]   = (uint8_t)(data >> 8);
                    reply_data[1]   = (uint8_t)(data & 0xff);
                    *reply_data_len = 2;
                    err             = MODBUS_RTU_ERR_SUCCESS;
                }
                break;
            case REG_ADDR_CELL2_VOLT:
                // TBD
                return MODBUS_RTU_ERR_BAD_REGISTER_ADDR;
                break;
            case REG_ADDR_CELL3_VOLT:
                // TBD
                return MODBUS_RTU_ERR_BAD_REGISTER_ADDR;
                break;
            case REG_ADDR_CELL4_VOLT:
                // TBD
                return MODBUS_RTU_ERR_BAD_REGISTER_ADDR;
                break;
            case REG_ADDR_BATT_CURRENT:
                // TBD
                result = adc_read_batt_current(&data);
                if (result != true) {
                    err = MODBUS_RTU_ERR_NOT_SUPPORTED;
#if (DEBUG_CONSOLE_EN > 0u)
                    debug_console("BAD REGISTER ADDR:");
#endif
                } else {
                    reply_data[0]   = (uint8_t)(data >> 8);
                    reply_data[1]   = (uint8_t)(data & 0xff);
                    *reply_data_len = 2;
                    err             = MODBUS_RTU_ERR_SUCCESS;
                    return err;
                }
                break;
            case REG_ADDR_BATT_TEMP:
                // TBD
                result = adc_read_batt_temp(&data);
                if (result != true) {
                    err = MODBUS_RTU_ERR_NOT_SUPPORTED;
#if (DEBUG_CONSOLE_EN > 0u)
                    debug_console("BAD REGISTER ADDR:");
#endif
                } else {
                    reply_data[0]   = (uint8_t)(data >> 8);
                    reply_data[1]   = (uint8_t)(data & 0xff);
                    *reply_data_len = 2;
                    err             = MODBUS_RTU_ERR_SUCCESS;
                    return err;
                }
                break;
            default:
                // Register is out of range
                err = MODBUS_RTU_ERR_BAD_REGISTER_ADDR;
                return err;
                break;
        }
    }
    return err;
}