#include "timer.h"

/**
 * \author siyuan xu, e2101066@edu.vamk.fi, Oct.2023
 * \brief  Initialize TIM2 and enable interrupt.
 */
void timer2_init() {
    // Enable the TIM2 clock.
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // Enable the NVIC interrupt for TIM2.
    NVIC_SetPriority(TIM2_IRQn, 0x03);
    NVIC_EnableIRQ(TIM2_IRQn);
}

/**
 * \author siyuan xu, e2101066@edu.vamk.fi, Oct.2023
 * \brief  Initialize TIM2 and enable interrupt.
 * \param[in] frequency_hz The target frequency in Hz.
 * \param[in] interval_ms  The time inverval in miliseconds when timer is up.
 * \example timers_start(1000, 500); would start timer2 at 1000Hz and 500ms interval.
 */
void timer2_start(const uint16_t frequency_hz, const uint16_t interval_ms) {
    // disable the timer, just in case
    TIM2->CR1 &= ~(TIM_CR1_CEN);

    // Reset the timer
    RCC->APB1RSTR |= (RCC_APB1RSTR_TIM2RST);
    RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM2RST);
    // Set the timer prescaler/autoreload timing registers.
    // (These are 16-bit timers, so this won't work with 32MHz.)
    TIM2->PSC = F_CPU / frequency_hz - 1;
    TIM2->ARR = interval_ms - 1;
    // Send an update event to reset the timer and apply settings.
    TIM2->EGR |= TIM_EGR_UG;
    // Enable the hardware interrupt.
    TIM2->DIER |= TIM_DIER_UIE;
    // Enable the timer.
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
 * \author siyuan xu, e2101066@edu.vamk.fi, Oct.2023
 * \brief  Stop a timer.
 * \param[in] TIMx The name of the timer.
 * \example timer_stop(TIM3); stops timer3.
 */
void timer_stop(TIM_TypeDef *TIMx) {
    // Turn off the timer.
    TIMx->CR1 &= ~(TIM_CR1_CEN);
    // Clear the 'pending update interrupt' flag, just in case.
    TIMx->SR &= ~(TIM_SR_UIF);
}