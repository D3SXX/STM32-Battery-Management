#include <unity.h>

#include "../unity_config.h"
#include "modbus_rtu.h"
#include "stm32l1xx.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_modbus_addr_validation_positive(void) {
    uint8_t addr = 0x05;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_AddressValidation(addr));
}

void test_modbus_addr_validation_negative(void) {
    uint8_t addr = 0x04;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_BAD_SLAVE_ADDR, modbusRtu_AddressValidation(addr));
}

void test_modbus_addr_validation_edge_max(void) {
    uint8_t addr = 0xff;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_BAD_SLAVE_ADDR, modbusRtu_AddressValidation(addr));
}

void test_modbus_addr_validation_edge_min(void) {
    uint8_t addr = 0;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_BAD_SLAVE_ADDR, modbusRtu_AddressValidation(addr));
}

void test_modbus_function_code_validation_positive() {
    MODBUS_FUNCTION_CODE f_code = READ_AI;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_FunctionCodeValidation(f_code));
}

void test_modbus_function_code_validation_negative() {
    MODBUS_FUNCTION_CODE f_code = 0xff;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_BAD_FUNCTION_CODE, modbusRtu_FunctionCodeValidation(f_code));
}
void test_modbus_function_code_validation_edge_max() {
    MODBUS_FUNCTION_CODE f_code = WRITE_ONE_AO;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_FunctionCodeValidation(f_code));
}
void test_modbus_function_code_validation_edge_min() {
    uint8_t f_code = READ_DO;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_FunctionCodeValidation(f_code));
}

void test_modbus_register_addr_validation_postive() {
    uint16_t reg_addr = 0x01;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_RegisterAddressValidation(reg_addr));
}
void test_modbus_register_addr_validation_negative() {
    uint16_t reg_addr = 0xff;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_BAD_REGISTER_ADDR,
                      modbusRtu_RegisterAddressValidation(reg_addr));
}
void test_modbus_register_addr_validation_edge_max() {
    uint16_t reg_addr = MODBUS_REGISTER_ADDR_MIN;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_RegisterAddressValidation(reg_addr));
}
void test_modbus_register_addr_validation_edge_min() {
    uint16_t reg_addr = MODBUS_REGISTER_ADDR_MAX;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_RegisterAddressValidation(reg_addr));
}

void test_modbus_crc_check_positive() {
    modbus_rtu_t modbus_frame;
    modbus_frame.SlaveAddress     = 0x05;
    modbus_frame.FunctionCode     = 0x04;
    modbus_frame.StartAddress     = 0x0100;  // little endian, (uint16)1 in the memeory is 0x0100
    modbus_frame.RegisterQuantity = 0x0100;
    modbus_frame.Checksum         = 0x8e61;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_CrcCheck((uint8_t *)&modbus_frame));

    uint8_t modbus_frame_byte[] = {0x05, 0x04, 0x00, 0x01, 0x00, 0x01, 0x61, 0x8e};
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_CrcCheck(modbus_frame_byte));
}

void test_modbus_crc_check_negative() {
    modbus_rtu_t modbus_frame;
    modbus_frame.SlaveAddress     = 0x05;
    modbus_frame.FunctionCode     = 0x04;
    modbus_frame.StartAddress     = 0x0100;
    modbus_frame.RegisterQuantity = 0x0100;
    modbus_frame.Checksum         = 0x8e60;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_BAD_CRC, modbusRtu_CrcCheck((uint8_t *)&modbus_frame));
}

void test_modbus_crc_check_edge_max() {
    modbus_rtu_t modbus_frame;
    modbus_frame.SlaveAddress     = 0xff;
    modbus_frame.FunctionCode     = 0xff;
    modbus_frame.StartAddress     = 0xffff;
    modbus_frame.RegisterQuantity = 0xffff;
    modbus_frame.Checksum         = 0x9401;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_CrcCheck((uint8_t *)&modbus_frame));
}

void test_modbus_crc_check_edge_min() {
    uint8_t modbus_frame_byte[] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0xfd, 0xca};
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_CrcCheck(modbus_frame_byte));

    modbus_rtu_t modbus_frame;
    modbus_frame.SlaveAddress     = 0x01;
    modbus_frame.FunctionCode     = 0x01;
    modbus_frame.StartAddress     = 0x0000;
    modbus_frame.RegisterQuantity = 0x0100;
    modbus_frame.Checksum         = 0xcafd;
    TEST_ASSERT_EQUAL(MODBUS_RTU_ERR_SUCCESS, modbusRtu_CrcCheck((uint8_t *)&modbus_frame));
}

static inline void LED2_init(void) {
    RCC->AHBENR |= 1;       // GPIOA ABH bus clock ON. p154
    GPIOA->MODER |= 0x400;  // GPIOA pin 5 to output. p184
    GPIOA->ODR |= 0x20;
}

int main(int argc, char **argv) {
    HAL_Init();       // initialize the HAL library
    HAL_Delay(2000);  // service delay
    LED2_init();

    UNITY_BEGIN();
    RUN_TEST(test_modbus_addr_validation_positive);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_addr_validation_negative);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_addr_validation_edge_max);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_addr_validation_edge_min);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_function_code_validation_positive);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_function_code_validation_negative);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_function_code_validation_edge_max);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_function_code_validation_edge_min);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_register_addr_validation_postive);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_register_addr_validation_negative);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_register_addr_validation_edge_max);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_register_addr_validation_edge_min);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_crc_check_positive);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_crc_check_negative);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_crc_check_edge_max);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    RUN_TEST(test_modbus_crc_check_edge_min);
    HAL_Delay(500);
    GPIOA->ODR &= ~0x20;
    UNITY_END();

    while (1) {
    }
}

void SysTick_Handler(void) { HAL_IncTick(); }