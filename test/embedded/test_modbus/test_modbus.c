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
    UNITY_END();

    while (1) {
    }
}

void SysTick_Handler(void) { HAL_IncTick(); }