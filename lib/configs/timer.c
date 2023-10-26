#include "timer.h"

void timer2_init() {
    // Enable the TIM2 clock.
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // Enable the NVIC interrupt for TIM2.
    NVIC_SetPriority(TIM2_IRQn, 0x03);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void timer2_start(uint16_t ms) {
    TIM2->CR1 &= ~(TIM_CR1_CEN);

    RCC->APB1RSTR |= (RCC_APB1RSTR_TIM2RST);
    RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM2RST);
    // Set the timer prescaler/autoreload timing registers.
    // (These are 16-bit timers, so this won't work with 32MHz.)
    TIM2->PSC = F_CPU / 1000 - 1;
    TIM2->ARR = ms - 1;
    // Send an update event to reset the timer and apply settings.
    TIM2->EGR |= TIM_EGR_UG;
    // Enable the hardware interrupt.
    TIM2->DIER |= TIM_DIER_UIE;
    // Enable the timer.
    TIM2->CR1 |= TIM_CR1_CEN;
}

void timer_stop(TIM_TypeDef *TIMx) {
    // Turn off the timer.
    TIMx->CR1 &= ~(TIM_CR1_CEN);
    // Clear the 'pending update interrupt' flag, just in case.
    TIMx->SR &= ~(TIM_SR_UIF);
}