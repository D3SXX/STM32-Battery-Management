#include "iwdg.h"
#include "ll_include.h"
#include "modbus_rtu.h"
#include "modbus_rtu_func_4.h"
#include "stdbool.h"
#include "sysclock_config.h"
#include "usart_config.h"
#include "utils.h"

#define DEBUG_CONSOLE_EN 1

#ifdef DEBUG_CONSOLE_EN> 0
#include "stdio.h"
#endif

/* Private define  */
bool u1tcFlag = false;  // USART1 Transmite complete flag
bool u1tsFlag = false;  // USART1 Transmite started flag

/* Private macro */
#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

/* Private variables */
_Atomic uint8_t usart1_rx_dma_buffer[USART1_RX_DMA_BUFFER_SIZE];
_Atomic uint8_t usart2_rx_dma_buffer[USART2_RX_DMA_BUFFER_SIZE];

/* Private define  */
static inline void LED2_init(void) {
    RCC->AHBENR |= 1;       // GPIOA ABH bus clock ON. p154
    GPIOA->MODER |= 0x400;  // GPIOA pin 5 to output. p184
}

void Configure_DebugPin(uint32_t debug_pin) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(GPIOA, debug_pin, LL_GPIO_MODE_OUTPUT);
}

/* Main routine */
int main(void) {
    __disable_irq();
    SystemClock_Config();
    init_delay_ms();
    USART1_dma_init();
    USART2_dma_init();
    IWDG_init();
    LED2_init();
    debug_console("App started...\n\r");
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);  // config a debug pin on GPIO
    __enable_irq();
    while (1) {
        IWDG_feed();
        // debug_console("app running...\n\r");
        delay_ms(1000);
        LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5);
    }
}

/* Interrupt handlers here */
void SysTick_Handler(void) {
    // Debug PIN PA10/pwm/D3
    LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_8);
}

/**
 * \brief   DMA1 channel5 interrupt handler for USART1 RX
 * \author  Siyuan Xu,
 */
void DMA1_Channel5_IRQHandler(void) {
    /* Check half-transfer complete interrupt */
    if (DMA1->ISR & DMA_ISR_HTIF5) {
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("USART1 DMA half-transfer interrupt!\r\n");
#endif
        DMA1->IFCR |= DMA_IFCR_CHTIF5; /*!< Channel 5 Half Transfer clear */
    }

    /* Check transfer-complete interrupt */
    if (DMA1->ISR & DMA_ISR_TCIF5) {
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("USART1 DMA transfer-complete interrupt!\r\n");
#endif
        u1tcFlag = true;
        DMA1->IFCR |= DMA_IFCR_CTCIF5; /*!< Channel 5 Transfer Complete clear */
        if (MODBUS_RTU_ERR_SUCCESS !=
            modbusRtu_AddressValidation(usart1_rx_dma_buffer[SLAVE_ADDRESS])) {
#if (DEBUG_CONSOLE_EN > 0u)
            char debug_msg[DBUG_MSG_LEN];
            snprintf(debug_msg, DBUG_MSG_LEN, "SlaveAddr:%#x\n\r",
                     usart1_rx_dma_buffer[SLAVE_ADDRESS]);
            debug_console("Not my address, discard the frame!\r\n");
#endif
        } else {
            modbusRtu_ParseRequest(usart1_rx_dma_buffer);
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("My address, Run Modbus_ParseRequest()!\r\n");
#endif
        }
        // TBD - start usart1_watchdog(USART1_RX_Buffer_Reset)
        USART1_RX_Buffer_Reset();
        DMA1_Channel15_Reload();
        // u1tcFlag = false;
    }
}

/**
 * \brief   USART1 global interrupt handler
 * \author  Siyuan xu, e2101066@edu.vamk.fi, Feb.2023
 */
void USART1_IRQHandler(void) {
    uint32_t status = USART1->SR;
    uint8_t  data __attribute__((unused));

    /* Check for Read Data Register Not Empty interrupt */
    if (status & USART_SR_RXNE) {
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("USART1 Read Data Register Not Empty!\r\n");
#endif
        u1tsFlag = true;
    }
    /* Check for IDLE line interrupt */
    if (status & USART_SR_IDLE) {
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("USART1 Idle-line interrupt!\r\n");
#endif
        // TBD - Stop usart1_watchdog
        if (u1tcFlag == true) {
            u1tcFlag = false;
            USART1_RX_Buffer_Reset();
            DMA1_Channel15_Reload();
            data = USART1->DR; /* Clear IDLE line flag */
        } else {
            // Error condition when USART1 DMA transmit complete flag is false and IDLE line
            // interrupt fires, reset DMA1 and buffer
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console(
                "Error!"
                "IDLE line detected, but transmition complete flag is FALSE!\n\r"
                "Received modbus frame length is lesser than expected.\n\r");
#endif
            USART1_RX_Buffer_Reset();
            DMA1_Channel15_Reload();
            data = USART1->DR; /* Clear IDLE line flag */
        }
    }
    /* Check Tranmission complete intrrupt */
    if (status & USART_SR_TC) {
        if (usart1_rx_dma_buffer[0] != NULL) {
            USART1_RX_Buffer_Reset();
            DMA1_Channel15_Reload();
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console(
                "ERROR! Transmition complete. But USART1_RX buffer is not empty." 
                "Received modbus frame length is greater than expected.\r\n");
#endif
        }
    }
}

/**
 * \brief   DMA1 channel6 interrupt handler for USART2 RX
 * \author  Siyuan xu, e2101066@edu.vamk.fi, Feb.2023
 */
void DMA1_Channel6_IRQHandler(void) {
    /* Check half-transfer complete interrupt */
    if (DMA1->ISR & DMA_ISR_HTIF6) {
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("USART2 DMA half-transfer interrupt!\r\n");
#endif
        DMA1->IFCR |= DMA_IFCR_CHTIF6; /*!< Channel 6 Half Transfer clear */

        USART2_send_data(usart2_rx_dma_buffer, USART2_RX_DMA_BUFFER_SIZE);
    }

    /* Check transfer-complete interrupt */
    if (DMA1->ISR & DMA_ISR_TCIF6) {
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("USART2 DMA transfer-complete interrupt!\r\n");
#endif
        DMA1->IFCR |= DMA_IFCR_CTCIF6; /*!< Channel 6 Transfer Complete clear */
        USART2_send_data(usart2_rx_dma_buffer, USART2_RX_DMA_BUFFER_SIZE);
    }
}

/**
 * \brief   USART2 global interrupt handler
 * \author  Siyuan xu, e2101066@edu.vamk.fi, Feb.2023
 */
void USART2_IRQHandler(void) {
    uint32_t status = USART2->SR;
    uint8_t  data __attribute__((unused));
    /* Check for IDLE line interrupt */
    if (status & USART_SR_IDLE) {
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("USART2 Idle-line interrupt!\r\n");
#endif
        data = USART2->DR; /* Clear IDLE line flag */
        USART2_send_data(usart2_rx_dma_buffer, USART2_RX_DMA_BUFFER_SIZE);
        DMA1_Channel16_Reload();
    }
}