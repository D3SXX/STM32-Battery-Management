#include "sysclock_config.h"

#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_gpio.h"
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
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1) {
    }
    // LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1); Not avaible for this board?
    LL_RCC_HSE_EnableBypass();
    LL_RCC_HSE_Enable();

    /* Wait till HSE is ready */
    while (LL_RCC_HSE_IsReady() != 1) {
    }
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLL_MUL_8,
                                LL_RCC_PLL_DIV_2);  // 8Mhz(base clock) * 8 / 2 = 32MHz
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1) {
    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
    }

    //   LL_Init1msTick(32000000);  //This will disable SysTick interrupt

    LL_SetSystemCoreClock(32000000);
}

/**
 * @brief  Configures the HSI as System clock frequency, AHB/APBx prescalers and Flash
 *         settings.
 * @note   This function should be called only once the RCC clock configuration
 *         is reset to the default reset state (done in SystemInit() function).
 * @param  None
 * @retval None
 */
#define HSI_STARTUP_TIMEOUT ((uint16_t)0x0500) /* Time out for HSI start up */

void SystemClock_HSI_Config(void) {
    __IO uint32_t StartUpCounter = 0, HSIStatus = 0;

    /* Enable HSI */
    RCC->CR |= ((uint32_t)RCC_CR_HSION);

    /* Wait till HSI is ready and if Time out is reached exit */
    do {
        HSIStatus = RCC->CR & RCC_CR_HSIRDY;
    } while ((HSIStatus == 0) && (StartUpCounter != HSI_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSIRDY) != RESET) {
        HSIStatus = (uint32_t)0x01;
    } else {
        HSIStatus = (uint32_t)0x00;
    }

    if (HSIStatus == (uint32_t)0x01) {
        /*  PLL configuration: PLLCLK = (HSI * 6)/3 = 32 MHz */
        RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL | RCC_CFGR_PLLDIV));
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI | RCC_CFGR_PLLMUL4 | RCC_CFGR_PLLDIV2);
    }

    else {
        /* If HSI fails to start-up, the application will have wrong clock
        configuration. User can add here some code to deal with this error */
    }

    /* Enable 64-bit access */
    FLASH->ACR |= FLASH_ACR_ACC64;

    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTEN;

    /* Flash 1 wait state */
    FLASH->ACR |= FLASH_ACR_LATENCY;

    /* Power enable */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    /* Select the Voltage Range 1 (1.8 V) */
    PWR->CR = PWR_CR_VOS_0;

    /* Wait Until the Voltage Regulator is ready */
    while ((PWR->CSR & PWR_CSR_VOSF) != RESET) {
    }

    /* HCLK = SYSCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    /* PCLK1 = HCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0) {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL) {
    }
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
                    SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}