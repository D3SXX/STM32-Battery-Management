#include "modbus_rtu.h"

#include <stdlib.h>

#include "crc.h"
#include "utils.h"

/**
 * \brief Create an modbus_rtu_t object
 * \return modbus_rtu_t type struct
 * \author siyuan xu, e2101066@edu.vamk.fi, Jan.2023
 */
modbus_rtu_rqst_t modbus_rtu_create(void) {
    modbus_rtu_rqst_t modbus_rtu_object = {0, 0, 0, 0, 0};
    return modbus_rtu_object;
}

/**
 * \brief Run a modbus rtu request
 * \param[in] modbus_rtu_frame - Address + PDU + CRC, PDU = Function code + Data
 * \author siyuan xu, e2101066@edu.vamk.fi, Jan.2023
 */
MODBUS_RTU_ERR modbusRtu_ParseRequest(const uint8_t *const modbus_rtu_frame) {
    MODBUS_RTU_ERR err;
    uint8_t        reply_data[8]  = 0;
    uint8_t        reply_data_len = 0;

    // Validate CRC checksum
    err = modbusRtu_CrcCheck(modbus_rtu_frame);
    if (err != MODBUS_RTU_ERR_SUCCESS) {
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("BAD CRC!\n\r");
#endif
        modbusRtu_ErrorReply(modbus_rtu_frame, (uint8_t)err);
        return err;
    } else {
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("CRC SUCCESS!\n\r");
#endif
        // Validate Function Code
        err = modbusRtu_FunctionCodeValidation(modbus_rtu_frame[FUNCTION_CODE]);
        if (MODBUS_RTU_ERR_SUCCESS != err) {
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("BAD FUNCTION CODE!\n\r");
#endif
            modbusRtu_ErrorReply(modbus_rtu_frame, (uint8_t)err);
            return err;
        } else {
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("FUNCTION CODE Accepted!\n\r");
#endif
            switch (modbus_rtu_frame[FUNCTION_CODE]) {
                case READ_DO:
                    // Not supported
                    err = MODBUS_RTU_ERR_ILLEGAL_FUNCTION;
                    break;
                case READ_DI:
                    // Not supported
                    err = MODBUS_RTU_ERR_ILLEGAL_FUNCTION;
                    break;
                case READ_AO:
                    // Not supported
                    err = MODBUS_RTU_ERR_ILLEGAL_FUNCTION;
                    break;
                case READ_AI:
                    // TBD
                    err =
                        modbusRtu_ReadInputRegister(modbus_rtu_frame, reply_data, &reply_data_len);
                    break;
                case WRITE_ONE_DO:
                    // Not supported
                    err = MODBUS_RTU_ERR_ILLEGAL_FUNCTION;
                    break;
                case WRITE_ONE_AO:
                    // Not supported
                    err = MODBUS_RTU_ERR_ILLEGAL_FUNCTION;
                    break;
                default:
                    break;
            }
            if (MODBUS_RTU_ERR_SUCCESS != err) {
                modbusRtu_ErrorReply(modbus_rtu_frame, (uint8_t)err);
            } else {
                modbusRtu_Reply(modbus_rtu_frame, reply_data, reply_data_len);
#if (DEBUG_CONSOLE_EN > 0u)
                debug_console("Modbus RTU request execution successful!\n\r");
#endif
            }
        }
    }
}

/**
 * \brief Validate the slave address in request frame
 * \param[in] address - the slave address in request frame
 * \return MODBUS_RTU_SUCCESS when success, MODBUS_RTU_ERR_BAD_SLAVE_ADDR when failed
 * \author siyuan xu, e2101066@edu.vamk.fi, Jan.2023
 */
MODBUS_RTU_ERR modbusRtu_AddressValidation(const uint8_t address) {
    if (address != MODBUS_RTU_SLAVE_ADDR_THIS) {
        return MODBUS_RTU_ERR_BAD_SLAVE_ADDR;
    } else {
        return MODBUS_RTU_ERR_SUCCESS;
    }
}

/**
 * \brief Reply to Modbus RTU Master when exception occures
 * \param[in] modbus_rtu_frame - Address + PDU + CRC, PDU = Function code + Data
 * \param[in] modbus_exception_code - MODBUS_RTU_ERR code
 * \author siyuan xu, e2101066@edu.vamk.fi, Sep.2023
 */
void modbusRtu_ErrorReply(const uint8_t *const modbus_rtu_frame,
                          MODBUS_RTU_ERR       modbus_exception_code) {
    uint8_t  modbus_reply_frame[MODBUS_FRAME_ERROR_REPLY_LENGTH];
    uint16_t crc                         = 0;
    modbus_reply_frame[SLAVE_ADDRESS]    = modbus_rtu_frame[SLAVE_ADDRESS];
    modbus_reply_frame[FUNCTION_CODE]    = modbus_rtu_frame[FUNCTION_CODE] + 0x80;
    modbus_reply_frame[ERROR_REPLY_DATA] = modbus_exception_code;
    crc                                  = CRC16(modbus_reply_frame, 3);
    modbus_reply_frame[ERROR_REPLY_CHECKSUM_HI] =
        (uint8_t)(crc & 0xff);  // crc_low -> reply_frame_crc[0]
    modbus_reply_frame[ERROR_REPLY_CHECKSUM_LOW] =
        (uint8_t)(crc >> 8);  // crc_hi  -> reply_frame_crc[1]
    modbusRtu_SendData(modbus_reply_frame, MODBUS_FRAME_ERROR_REPLY_LENGTH);
}

/**
 * \brief Free the dynamic allocated memory
 * \param[in] modbus_rtu_rqst The address of modbus request object.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
uint8_t *modbusRtu_CreateReplyFrame(const modbus_rtu_rqst_t *const modbus_rtu_rqst) {
    // addr(1B) + f_code(1B) + byte_cnt(1B) + CRC(2B) = 5 bytes
    // the rest = reg_qty(2B) * 2
    int reply_frame_len = 5 + modbus_rtu_rqst->RegisterQuantity * 2;
    uint8_t reply_frame = (uint8_t*)calloc(reply_frame_len, sizeof(uint8_t));
}

/**
 * \brief Free the dynamic allocated memory
 * \param[in] modbus_rtu_reply The address of modbus replay or error reply frame.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
void modbusRtu_FreeReplyFrame(uint8_t *modbus_rtu_reply) { free(modbus_rtu_reply); }

/**
 * \brief Normal reply to Modbus RTU Master
 * \param[in] modbus_rtu_frame - Address + PDU + CRC, PDU = Function code + Data
 * \param[in] data - The address of the requested data (coil, discrete input, register...)
 * \param[in] data_len - The number of bytes of the data
 * \author siyuan xu, e2101066@edu.vamk.fi, Sep.2023
 */
void modbusRtu_Reply(const uint8_t *const modbus_rtu_frame, const uint8_t *data,
                     const uint8_t data_len) {
    uint8_t  index = 0;
    uint8_t  modbus_reply_frame[MODBUS_FRAME_REPLY_LENGTH];
    uint16_t crc                         = 0;
    modbus_reply_frame[SLAVE_ADDRESS]    = modbus_rtu_frame[SLAVE_ADDRESS];
    modbus_reply_frame[FUNCTION_CODE]    = modbus_rtu_frame[FUNCTION_CODE];
    modbus_reply_frame[REPLY_BYTE_COUNT] = data_len;
    index                                = REPLY_BYTE_COUNT;
    for (uint8_t n = 0; n < data_len; n++) {
        modbus_reply_frame[++index] = data[n];
    }
    crc                         = CRC16(modbus_reply_frame, index + 1);
    modbus_reply_frame[++index] = (uint8_t)(crc & 0xff);  // crc_low -> reply_frame_crc[0]
    modbus_reply_frame[++index] = (uint8_t)(crc >> 8);    // crc_hi  -> reply_frame_crc[1]

    modbusRtu_SendData(modbus_reply_frame, MODBUS_FRAME_REPLY_LENGTH);
}

/**
 * \brief CRC-16 error check for the Modbus RTU reqeust frame
 * \param[in] modbus_rtu_frame - Address + PDU + CRC, PDU = Function code + Data
 * \return MODBUS_RTU_SUCCESS when success, MODBUS_RTU_ERR_BAD_CRC when failed
 * \author siyuan xu, e2101066@edu.vamk.fi, Sep.2023
 */
MODBUS_RTU_ERR modbusRtu_CrcCheck(const uint8_t *const modbus_rtu_frame) {
    uint16_t crc_checksum = 0;
    uint16_t crc_calcuate = 0;
    char     crc_str[10];
    crc_calcuate = CRC16(modbus_rtu_frame, 6);
#if (DEBUG_CONSOLE_EN > 0u)
    debug_console("CRC_CAL=");
    debug_console(itoa(crc_calcuate, crc_str, 10));
#endif

    crc_checksum =
        // This Wired Big Endian CRC for the course, removed
        // ((uint16_t)modbus_rtu_frame[CHECKSUM_HI] << 8) |
        // (uint16_t)modbus_rtu_frame[CHECKSUM_LOW];
        ((uint16_t)modbus_rtu_frame[CHECKSUM_LOW] << 8) | (uint16_t)modbus_rtu_frame[CHECKSUM_HI];

    if (crc_calcuate != crc_checksum) {
        return MODBUS_RTU_ERR_BAD_CRC;
    }

    return MODBUS_RTU_ERR_SUCCESS;
}

/**
 * \brief Validate the function code from the Modbus RTU reqeust frame
 * \param[in] function_code - The function code
 * \return MODBUS_RTU_SUCCESS when success, MODBUS_RTU_ERR_BAD_FUNCTION_CODE when failed
 * \author siyuan xu, e2101066@edu.vamk.fi, Jan.2023
 */
MODBUS_RTU_ERR modbusRtu_FunctionCodeValidation(const uint8_t function_code) {
    if ((function_code < READ_DO) || (function_code > WRITE_ONE_AO)) {
        return MODBUS_RTU_ERR_ILLEGAL_FUNCTION;
    } else {
        return MODBUS_RTU_ERR_SUCCESS;
    }
}

/**
 * \brief Validate the register address from the Modbus RTU reqeust frame
 * \param[in] reg_addr - The register address
 * \return MODBUS_RTU_SUCCESS when success, MODBUS_RTU_ERR_BAD_REGISTER_ADDR when failed
 * \author siyuan xu, e2101066@edu.vamk.fi, Jan.2023
 */
MODBUS_RTU_ERR modbusRtu_RegisterAddressValidation(const uint16_t reg_addr,
                                                   const uint16_t reg_qty) {
    if ((reg_addr < MODBUS_REGISTER_ADDR_MIN) || (reg_addr > MODBUS_REGISTER_ADDR_MAX)) {
        return MODBUS_RTU_ERR_ILLEGAL_DATA_ADDR;
    } else {
        if (reg_addr + (reg_qty - 1) * 2 > MODBUS_REGISTER_ADDR_MAX) {
            return MODBUS_RTU_ERR_ILLEGAL_DATA_VALUE;
        } else {
            return MODBUS_RTU_ERR_SUCCESS;
        }
    }
}