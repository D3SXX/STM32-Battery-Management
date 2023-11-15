#include <unity.h>

#include "../unity_config.h"
#include "stm32l1xx.h"
#include "usart_config.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_mosfet_control_set_pb8(void) {
    // mostfet_control_set_single_pin(3u);
    // HAL_Delay(1000);
    TEST_PASS();
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
    LED2_init();

    UNITY_BEGIN();
    RUN_TEST(test_mosfet_control_set_pb8);
    GPIOA->ODR ^= 0x20;
    HAL_Delay(500);

    UNITY_END();

    while (1) {
    }
}

void SysTick_Handler(void) { HAL_IncTick(); }
