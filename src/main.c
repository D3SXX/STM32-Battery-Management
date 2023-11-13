#include "STM32L1xx_LL_Driver.h"
#include "adc.h"
#include "iwdg.h"
#include "modbus_rtu.h"
#include "modbus_rtu_func_4.h"
#include "mux.h"
#include "stdbool.h"
#include "sysclock_config.h"
#include "timer.h"
#include "usart_config.h"
#include "utils.h"

#if (DEBUG_CONSOLE_EN > 0u)
#include "stdio.h"
#endif

/* Private flags  */
#define MODBUS_REQUEST       0x01U
#define ADC_READ_CURRENT     0x02U
#define ADC_READ_TEMPERATURE 0x04U
#define ADC_READ_VOLTS       0x08U
#define ADC_READ_ALL         0x0EU
#define DMA1_ERROR           0x8000U

uint16_t subroutine_flag  = 0x00;
bool     dma1_ch5_tc_flag = false;  // DMA1 ch5 Transmite complete flag

/* Private macro */
#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

/* Private variables */
uint8_t usart1_rx_dma_buffer[USART1_RX_DMA_BUFFER_SIZE];
#if (DEBUG_CONSOLE_EN > 0u)
uint8_t usart2_rx_dma_buffer[USART2_RX_DMA_BUFFER_SIZE];
#endif

/* Private function prototypes  */
static inline void LED2_init(void) {
    RCC->AHBENR |= 1;       // GPIOA ABH bus clock ON. p154
    GPIOA->MODER |= 0x400;  // GPIOA pin 5 to output. p184
}

void Configure_DebugPin(uint32_t debug_pin) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(GPIOA, debug_pin, LL_GPIO_MODE_OUTPUT);
}

void modbus_routine();
void read_current_routine(void);
void read_temperature_routine(void);
void read_cell_voltage_routine(void);
void TIM2_IRQ_handler(void);
void usart1_rx_dma_frame_oversize_check();


/* Main routine */
int main(void) {
    __disable_irq();
    SystemClock_Config();
    systick_init(1000);  // set SysTick frequency to 1000Hz
    USART1_dma_init();
#if (DEBUG_CONSOLE_EN > 0u)
    USART2_dma_init();
#endif
    IWDG_init();
    adc_init();
    mux_init();
    LED2_init();
    timer2_init();
    timer2_start(1000, 1000);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);  // config a debug pin on GPIO
#if (DEBUG_CONSOLE_EN > 0u)
    debug_console("App started...\n\r");
#endif
    __enable_irq();

    while (1) {
        IWDG_feed();

        if (subroutine_flag & ADC_READ_CURRENT) {
            read_current_routine();
        }
        if (subroutine_flag & ADC_READ_TEMPERATURE) {
            read_temperature_routine();
        }
        if (subroutine_flag & ADC_READ_VOLTS) {
            read_cell_voltage_routine();
        }
        if (subroutine_flag & MODBUS_REQUEST) {
            modbus_routine();
            usart1_rx_dma_frame_oversize_check();
            subroutine_flag &= ~MODBUS_REQUEST;
        }
        if (subroutine_flag & DMA1_ERROR) {
            USART1_RX_DMA_Buffer_Reset();
            DMA1_Channel15_Reload();
            subroutine_flag &= ~DMA1_ERROR;  // Reset DMA1_ERROR
        }
        // delay_ms(1000);
        // LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5);
    }
}

/**
 * \brief   DMA1 channel5 interrupt handler for USART1 RX
 * \author  Siyuan Xu,
 * \details If modbus request message length is great than 8 bytes
 */
void usart1_rx_dma_frame_oversize_check(void) {
    if (!dma1_ch5_tc_flag && usart1_rx_dma_buffer[0] != 0) {
        subroutine_flag |= DMA1_ERROR;
    }
}

/* Interrupt handlers here */
void SysTick_Handler(void) {
    // Debug PIN PA8
    // LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_8);
}

void TIM2_IRQHandler(void) {
    // Handle a timer 'update' interrupt event
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~(TIM_SR_UIF);
        if (!(subroutine_flag & MODBUS_REQUEST)) {  // prevent reading during modbus request.
            subroutine_flag |= ADC_READ_ALL;
        }
    }
    LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5);
}

void modbus_routine(void) {
    if (MODBUS_RTU_ERR_SUCCESS !=
        modbusRtu_AddressValidation(usart1_rx_dma_buffer[SLAVE_ADDRESS])) {
#if (DEBUG_CONSOLE_EN > 0u)
        char debug_msg[DBUG_MSG_LEN];
        snprintf(debug_msg, DBUG_MSG_LEN, "SlaveAddr:%#x\n\r", usart1_rx_dma_buffer[SLAVE_ADDRESS]);
        debug_console("Not my address, discard the frame!\r\n");
#endif
    } else {
        modbusRtu_ParseRequest(usart1_rx_dma_buffer);
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("My address, Run Modbus_ParseRequest()!\r\n");
#endif
    }
    // TBD - start usart1_watchdog(USART1_RX_Buffer_Reset)
    subroutine_flag &= ~MODBUS_REQUEST;  // disable the flag
    dma1_ch5_tc_flag = false;
    USART1_RX_DMA_Buffer_Reset();
    DMA1_Channel15_Reload();
}

void read_current_routine(void) {
    mux_set(MUX_SEL_CH4);
    uint16_t current                        = adc_convert_batt_current(adc_read);
    modbus_registers[REG_ADDR_BATT_CURRENT] = current;
    if (current == 0) {
        // There is no discharging current sensing yet in this project. Turn mosfet control to fully
        // open for discharging. But no way to shutdown by discharging current. TBD - mosfet_control
        // mosfet_control(MOSFET_BIDIRECTION);
    } else if (current < 500) {
        // Maxium Drain-Source Diode forward current is 1300mA. Using 500mA for super safety.
        // mosfet_control(MOSFET_CHARGE_ONLY);
    } else if (current < 33840) {  // 10% below mosfet maximum rating of Drain Current.

        // warning();
    } else if (current > 37600) {  // Absolute maximum value for
        // mosfet_contro(MOSFET_SHUT_DOWN);
    }
    subroutine_flag &= ~ADC_READ_CURRENT;
}

void read_temperature_routine(void) {
    mux_set(MUX_SEL_CH5);
    uint16_t temp_uint                   = adc_convert_batt_temp(adc_read);
    modbus_registers[REG_ADDR_BATT_TEMP] = temp_uint;
    double temp_double                   = (double)(int)temp_uint;
    if (temp_double > 60.0) {
        // mosfet_control(MOSFET_SHUT_DOWN);
    } else {
        // mosfet_control(MOSFET_PREVIOUS_STATE);
    }
    subroutine_flag &= ~ADC_READ_TEMPERATURE;
}

void read_cell_voltage_routine(void) {
    mux_set(MUX_SEL_CH0);
    uint16_t voltage                      = adc_convert_cell_voltage(adc_read);
    modbus_registers[REG_ADDR_CELL1_VOLT] = voltage;
    if (voltage <= 2600) {
        // stm32_sleep();
        // Wakeup by ADC, Timer?
    } 
    if (voltage >= 3330) {
        // mosfet_control(MOSFET_SHUT_DOWN);
    }

    mux_set(MUX_SEL_CH1);
    voltage                               = adc_convert_cell_voltage(adc_read);
    modbus_registers[REG_ADDR_CELL2_VOLT] = voltage;
    if (voltage <= 2600) {
        // stm32_sleep();
    }
    if (voltage >= 3330) {
        // mosfet_control(MOSFET_SHUT_DOWN);
    }

    mux_set(MUX_SEL_CH2);
    voltage                               = adc_convert_cell_voltage(adc_read);
    modbus_registers[REG_ADDR_CELL3_VOLT] = voltage;
    if (voltage <= 2600) {
        // stm32_sleep();
    }
    if (voltage >= 3330) {
        // mosfet_control(MOSFET_SHUT_DOWN);
    }

    mux_set(MUX_SEL_CH3);
    voltage                               = adc_convert_cell_voltage(adc_read);
    modbus_registers[REG_ADDR_CELL4_VOLT] = voltage;
    if (voltage <= 2600) {
        // stm32_sleep();
    }
    if (voltage >= 3330) {
        // mosfet_control(MOSFET_SHUT_DOWN);
    }
    subroutine_flag &= ~ADC_READ_VOLTS;
}

/**
 * \brief   DMA1 channel5 interrupt handler for USART1 RX
 * \author  Siyuan Xu,
 */
void DMA1_Channel5_IRQHandler(void) {
    /* Check half-transfer complete interrupt */
    // TBD-REMOVE HALF-TRANSFER INTERRUPT
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
        dma1_ch5_tc_flag = true;
        subroutine_flag |= MODBUS_REQUEST;  // enable the flag
        DMA1->IFCR |= DMA_IFCR_CTCIF5;      /*!< Channel 5 Transfer Complete clear */
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
    }
    /* Check for IDLE line interrupt */
    if (status & USART_SR_IDLE) {
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("USART1 Idle-line interrupt!\r\n");
#endif
        // TBD - Stop usart1_watchdog
        if (dma1_ch5_tc_flag == true) {
            data = USART1->DR; /* Clear IDLE line flag */
        } else {
            // Error condition when USART1 DMA transmit complete flag is false and IDLE line
            // interrupt fires, reset DMA1 and buffer
            subroutine_flag |= DMA1_ERROR;
            data = USART1->DR; /* Clear IDLE line flag */
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console(
                "Error!"
                "IDLE line detected, but dma transmition complete flag is FALSE!\n\r"
                "Received modbus frame length is lesser than expected.\n\r");
#endif
        }
    }
    /* Check Tranmission complete interrupt */
    if (status & USART_SR_TC) {
        if (!dma1_ch5_tc_flag && usart1_rx_dma_buffer[0] != 0) {
            subroutine_flag |= DMA1_ERROR;
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
    // TBD REMOVE DMA1_Channel6
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