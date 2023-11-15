#include <unity.h>

#include "../unity_config.h"
#include "mosfet_control.h"
#include "usart_config.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_mosfet_control_set_pb6(void) {
    mosfet_control_set_single_pin(6u);
    // HAL_Delay(1000);
    uint32_t gpiob_status = ((GPIOB->IDR) & (MOSFET_CONTROL_IDR_ODR_MASK));
    TEST_ASSERT_EQUAL(0x40, gpiob_status);
    // TEST_FAIL_MESSAGE("Not Implemented.");
    mosfet_control_reset_single_pin(6u);
    // HAL_Delay(1000);
}

void test_mosfet_control_set_pb10(void) {
    mosfet_control_set_single_pin(10u);
    // HAL_Delay(1000);
    uint32_t gpiob_status = ((GPIOB->IDR) & (MOSFET_CONTROL_IDR_ODR_MASK));
    TEST_ASSERT_EQUAL(0x400, gpiob_status);
    // TEST_FAIL_MESSAGE("Not Implemented.");
    mosfet_control_reset_single_pin(10u);
    // HAL_Delay(1000);
}

void test_mosfet_control_set_closed(void) {
    mosfet_control_set(MOSFET_CONTROL_CLOSED);
    // HAL_Delay(1000);
    uint32_t gpiob_status = ((GPIOB->IDR) & (MOSFET_CONTROL_IDR_ODR_MASK));
    TEST_ASSERT_EQUAL(0x0, gpiob_status);
    // TEST_FAIL_MESSAGE("Not Implemented.");
    mosfet_control_reset();
    // HAL_Delay(1000);
}

void test_mosfet_control_set_open(void) {
    mosfet_control_set(MOSFET_CONTROL_OPEN);
    // HAL_Delay(1000);
    uint32_t gpiob_status = ((GPIOB->IDR) & (MOSFET_CONTROL_IDR_ODR_MASK));
    TEST_ASSERT_EQUAL(0x440, gpiob_status);
    // TEST_FAIL_MESSAGE("Not Implemented.");
    mosfet_control_reset();
    // HAL_Delay(1000);
}

void test_mosfet_control_set_charge(void) {
    mosfet_control_set(MOSFET_CONTROL_CHARGE);
    // HAL_Delay(1000);
    uint32_t gpiob_status = ((GPIOB->IDR) & (MOSFET_CONTROL_IDR_ODR_MASK));
    TEST_ASSERT_EQUAL(0x400, gpiob_status);
    // TEST_FAIL_MESSAGE("Not Implemented.");
    mosfet_control_reset();
    // HAL_Delay(1000);
}

void test_mosfet_control_set_discharge(void) {
    mosfet_control_set(MOSFET_CONTROL_DISCHARGE);
    // HAL_Delay(1000);
    uint32_t gpiob_status = ((GPIOB->IDR) & (MOSFET_CONTROL_IDR_ODR_MASK));
    TEST_ASSERT_EQUAL(0x40, gpiob_status);
    // TEST_FAIL_MESSAGE("Not Implemented.");
    mosfet_control_reset();
    // HAL_Delay(1000);
}

static inline void LED2_init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;              // GPIOA ABH bus clock ON. p154
    GPIOA->MODER |= 0x01 << GPIO_MODER_MODER5_Pos;  // GPIOA pin 5 to output. p184
    GPIOA->ODR |= 0x20;
}

int main(int argc, char **argv) {
    HAL_Init();       // initialize the HAL library
    HAL_Delay(2000);  // service delay
    mosfet_control_init();
    LED2_init();

    UNITY_BEGIN();
    RUN_TEST(test_mosfet_control_set_pb6);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mosfet_control_set_pb10);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mosfet_control_set_closed);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mosfet_control_set_open);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mosfet_control_set_charge);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mosfet_control_set_discharge);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);

    UNITY_END();

    while (1) {
    }
}

void SysTick_Handler(void) { HAL_IncTick(); }
