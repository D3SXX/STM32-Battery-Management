#include "sysclock_config.h"

#include "ll_include.h"

/*
 * sysclock_config.c
 *
 *  Created on: Sep, 2023
 *      Author: Siyuan Xu
 */

/**
 * \author siyuan xu, e2101066@edu.vamk.fi, Sep.2023
 * \brief Configures the HSE as System clock frequency 32MHz, AHB/APBx prescalers and Flash
 *         settings.
 */
void SystemClock_Config(void) {
    LL_FLASH_Enable64bitAccess();

    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1) {}
    // LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1); Not avaible for this board?
    LL_RCC_HSE_EnableBypass();
    LL_RCC_HSE_Enable();

    /* Wait till HSE is ready */
    while (LL_RCC_HSE_IsReady() != 1) {}
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLL_MUL_8,
                                LL_RCC_PLL_DIV_2);  // 8Mhz(base clock) * 8 / 2 = 32MHz
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1) {}
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {}

    //   LL_Init1msTick(32000000);  //This will disable SysTick interrupt

    LL_SetSystemCoreClock(32000000);
}

/**
 * \brief Set clock frequency for SysTick.
 * \param[in] frequency_hz SysTick frequecy in Hz.
 * \author siyuan xu, e2101066@edu.vamk.fi, Sep.2023
 */
void systick_init(const uint32_t frequency_hz) {
    SysTick->LOAD =
        F_CPU / frequency_hz - 1;  // 32 000 000 - 1 = 1s so 32000 - 1 = ms, 32 - 1= 1 us
    SysTick->VAL = 0;
    // enable counter, use processor clock, M3 Generic User Guide p. 159
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |  // Set SysTick->CTRL = 0b111;
                    SysTick_CTRL_TICKINT_Msk | 
                    SysTick_CTRL_ENABLE_Msk;
}