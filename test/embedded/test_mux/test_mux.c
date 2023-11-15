#include <unity.h>

#include "../unity_config.h"
#include "mux.h"
#include "usart_config.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_mux_set_pb3(void) {
    mux_set_single_pin(3u);
    // HAL_Delay(1000);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(0x08, gpiob_status);
    mux_reset_single_pin(3u);
    // HAL_Delay(1000);
}

void test_mux_set_pb4(void) {
    mux_set_single_pin(4u);
    // HAL_Delay(1000);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(0x10, gpiob_status);
    mux_reset_single_pin(4u);
    // HAL_Delay(1000);
}

void test_mux_set_pb5(void) {
    mux_set_single_pin(5u);
    // HAL_Delay(1000);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(0x20, gpiob_status);
    mux_reset_single_pin(5u);
    // HAL_Delay(1000);
}

void test_mux_set_all(void) {
    mux_set(0x38);
    // HAL_Delay(1000);
    uint32_t gpiob_status = GPIOB->IDR;
    mux_reset();
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(0x38, gpiob_status);
    // HAL_Delay(1000);
}

void test_mux_set_ch0(void) {
    mux_set(MUX_SEL_CH0);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(MUX_SEL_CH0, gpiob_status);
}

void test_mux_set_ch1(void) {
    mux_set(MUX_SEL_CH1);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(MUX_SEL_CH1, gpiob_status);
}

void test_mux_set_ch2(void) {
    mux_set(MUX_SEL_CH2);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(MUX_SEL_CH2, gpiob_status);
}

void test_mux_set_ch3(void) {
    mux_set(MUX_SEL_CH3);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(MUX_SEL_CH3, gpiob_status);
}

void test_mux_set_ch4(void) {
    mux_set(MUX_SEL_CH4);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(MUX_SEL_CH4, gpiob_status);
}

void test_mux_set_ch5(void) {
    mux_set(MUX_SEL_CH5);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(MUX_SEL_CH5, gpiob_status);
}

void test_mux_set_ch6(void) {
    mux_set(MUX_SEL_CH6);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(MUX_SEL_CH6, gpiob_status);
}

void test_mux_set_ch7(void) {
    mux_set(MUX_SEL_CH7);
    uint32_t gpiob_status = GPIOB->IDR;
    gpiob_status &= MUX_IDR_ODR_MASK;
    TEST_ASSERT_EQUAL(MUX_SEL_CH7, gpiob_status);
}

static inline void LED2_init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;              // GPIOA ABH bus clock ON. p154
    GPIOA->MODER |= 0x01 << GPIO_MODER_MODER5_Pos;  // GPIOA pin 5 to output. p184
    GPIOA->ODR |= 0x20;
}

int main(int argc, char **argv) {
    HAL_Init();       // initialize the HAL library
    HAL_Delay(2000);  // service delay
    mux_init();
    LED2_init();

    UNITY_BEGIN();
    RUN_TEST(test_mux_set_all);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_pb3);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_pb4);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_pb5);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_ch0);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_ch1);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_ch2);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_ch3);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_ch4);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_ch5);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_ch6);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);
    RUN_TEST(test_mux_set_ch7);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);

    UNITY_END();

    while (1) {
    }
}

void SysTick_Handler(void) { HAL_IncTick(); }
