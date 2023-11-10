#ifndef MODBUS_RTU_TYPES
#define MODBUS_RTU_TYPES

#include <stddef.h>
#include <stdint.h>

/* Modbus RTU parameters user settings */
#define MODBUS_RTU_SLAVE_ADDR_THIS       (uint8_t)0x05
#define MODBUS_REGISTER_SIZE             6
#define MODBUS_REGISTER_ADDR_MIN         0x0000  // modbus specification 0x0000 to 0xffff
#define MODBUS_REGISTER_ADDR_MAX         0x0005
#define MODBUS_REGISTER_QTY_MIN          0x0001  // modbus specification 0x0001 to 0x007d
#define MODBUS_REGISTER_QTY_MAX          0x007D
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

#pragma pack(push, 1)
/* Modbus specifies 16 bit CRC in little endian, and all other 16 bit data in big endian. STM32 CPU
 * is little endian, therefore only CRC will be interpreted corretly by default. All other 16 bit
 * values need to to manually decoded. */
typedef struct modbus_rtu_request_type {
    uint8_t  SlaveAddress;
    uint8_t  FunctionCode;
    uint16_t StartAddress;
    uint16_t RegisterQuantity;
    uint16_t Checksum;
} modbus_rtu_rqst_t;

typedef struct modbus_rtu_reply_type {
    uint8_t  SlaveAddress;
    uint8_t  FunctionCode;
    uint8_t  DataLength;
    uint8_t *Data;
    uint16_t Checksum;
} modbus_rtu_rply_t;

typedef struct modbus_rtu_error_reply_type {
    uint8_t  SlaveAddress;
    uint8_t  FunctionCode;
    uint8_t  ErrorCode;
    uint16_t Checksum;
} modbus_rtu_err_t;

#pragma pack(pop)

typedef enum {
    MODBUS_RTU_ERR_SUCCESS = 0,
    MODBUS_RTU_ERR_ILLEGAL_FUNCTION,
    MODBUS_RTU_ERR_ILLEGAL_DATA_ADDR,
    MODBUS_RTU_ERR_ILLEGAL_DATA_VALUE,
    MODBUS_RTU_ERR_SERVER_FAILURE,
    MODBUS_RTU_ERR_ACK,
    MODBUS_RTU_ERR_SERVER_BUSY,
    MODBUS_RTU_ERR_BAD_MEMORY_PARITY = 0x08,
    MODBUS_RTU_ERR_GATEWAY_PATH_UNAVAILABLE = 0x0A,
    MODBUS_RTU_ERR_GATEWAY_TARGET_DEVICE_NO_RESPONSE,
    MODBUS_RTU_ERR_BAD_SLAVE_ADDR = 20,     // User defined error codes
    MODBUS_RTU_ERR_BAD_CRC,
    MODBUS_RTU_ERR_BAD_FRAME_OVERSIZE,      // Received modbus request frame length is greater than 8 bytes.
    MODBUS_RTU_ERR_BAD_FRAME_UNDERSIZE,     // Received modbus request frame length is lesser than 8 bytes.
    MODBUS_RTU_ERR_NOT_SUPPORTED            // Not supported modbus functionalities in this implementation.
} MODBUS_RTU_ERR;

#endif