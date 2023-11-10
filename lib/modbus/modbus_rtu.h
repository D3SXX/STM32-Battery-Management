#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H
#include <stddef.h>
#include <stdint.h>
#include "usart_config.h"

/* Modbus RTU client parameters*/
#define MODBUS_RTU_SLAVE_ADDR_THIS       (uint8_t)0x05
#define MODBUS_REGISTER_SIZE             6
#define MODBUS_REGISTER_ADDR_MIN         0x00
#define MODBUS_REGISTER_ADDR_MAX         0x05
#define MODBUS_BAUD_RATE                 USART_BAUDRATE
#define MODBUS_FRAME_SILENT_WAIT_TIME_MS (int)(3.5 * 8 / MODBUS_BAUD_RATE)
#define MODBUS_FRAME_REPLY_LENGTH        7
#define MODBUS_FRAME_ERROR_REPLY_LENGTH  5

_Atomic uint16_t modbus_registers[MODBUS_REGISTER_SIZE];

/* Modbus RTU data structures */
typedef enum {
    SLAVE_ADDRESS = 0,
    FUNCTION_CODE,
    START_ADDRESS_HI,
    START_ADDRESS_LOW,
    REG_QUANTITY_HI,
    REG_QUANTITY_LOW,
    CHECKSUM_HI,
    CHECKSUM_LOW,
    REPLY_BYTE_COUNT = 2,
    REPLY_DATA_HI,
    REPLY_DATA_LOW,
    REPLY_CHECKSUM_HI,
    REPLY_CHECKSUM_LOW,
    ERROR_REPLY_DATA = 2,
    ERROR_REPLY_CHECKSUM_HI,
    ERROR_REPLY_CHECKSUM_LOW
} MODBUS_RTU_FRAME_BIT;

typedef enum {
    READ_DO = 1,
    READ_DI,
    READ_AO,
    READ_AI,
    WRITE_ONE_DO,
    WRITE_ONE_AO
} MODBUS_FUNCTION_CODE;

#pragma pack(1) 
typedef struct modbus_rtu_type {
    uint8_t  SlaveAddress;
    uint8_t  FunctionCode;
    uint16_t StartAddress;
    uint16_t RegisterQuantity;
    uint16_t Checksum;
} modbus_rtu_t;

typedef enum {
    MODBUS_RTU_ERR_SUCCESS = 0,
    MODBUS_RTU_ERR_BAD_FUNCTION_CODE,
    MODBUS_RTU_ERR_BAD_REGISTER_ADDR,
    MODBUS_RTU_ERR_BAD_CRC,
    MODBUS_RTU_ERR_INTERNAL_ERROR,
    MODBUS_RTU_ERR_BAD_SLAVE_ADDR = 20,
    MODBUS_RTU_ERR_BAD_FRAME_OVERSIZE,
    MODBUS_RTU_ERR_BAD_FRAME_UNDERSIZE,
    MODBUS_RTU_ERR_NOT_SUPPORTED
} MODBUS_RTU_ERR;

extern void           debug_console(const char *message);  // user needs to implement
extern void           modbusRtu_SendData(const uint8_t *const data,
                                         const size_t         data_length);  // user needs to implement
extern MODBUS_RTU_ERR modbusRtu_ReadInputRegister(
    const uint8_t *const modbus_rtu_frame,  // user needed to implement
    uint8_t *reply_data, uint8_t *reply_data_len);

void           modbusRtu_ParseRequest(const uint8_t *const modbus_rtu_frame);
void           modbusRtu_ErrorReply(const uint8_t *const modbus_rtu_frame,
                                    const MODBUS_RTU_ERR modbus_exception_code);
void           modbusRtu_Reply(const uint8_t *const modbus_rtu_frame, const uint8_t *data,
                               const uint8_t data_len);
modbus_rtu_t   modbus_rtu_create(void);
MODBUS_RTU_ERR modbusRtu_AddressValidation(const uint8_t address);
MODBUS_RTU_ERR modbusRtu_FunctionCodeValidation(const uint8_t function_code);
MODBUS_RTU_ERR modbusRtu_RegisterAddressValidation(const uint16_t reg_addr);
MODBUS_RTU_ERR modbusRtu_CrcCheck(const uint8_t *const modbus_rtu_frame);
#endif
