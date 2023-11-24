#include "battery_manager.h"

#include "mosfet_control.h"

#if (DEBUG_CONSOLE_EN > 0u)
#include "stdio.h"
#include "utils.h"
char message[128] = {0};
#endif

BATTERY_STATUS battery_status = BATT_STAT_GOOD;

/**
 * \brief Evaluate the battery status by giving cell voltage.
 * \param[in] voltage Cell voltage value, positive only.
 * \return BATTERY_STATUS self-explain.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
BATTERY_STATUS battery_manager_cell_voltage_check(const uint16_t cell_index,
                                                  const uint16_t voltage) {
    BATTERY_STATUS result = BATT_STAT_GOOD;
    if (voltage <= BATT_CELL_VOLT_MIN) {
#ifndef TEST_NATIVE
        if (!(BATT_STAT_ERROR & battery_status)) {
            mosfet_control_set(MOSFET_CONTROL_CHARGE);
        }
#endif
        result = (BATT_STAT_ERROR | BATT_STAT_VOLT_EMPTY);
#if (DEBUG_CONSOLE_EN > 0u)
        sprintf(message, "Error! Cell %u voltage critical low.\r\n", cell_index);
        debug_console(message);
#endif
    } else if (voltage <= BATT_CELL_VOLT_LOW) {
        result = (BATT_STAT_GOOD | BATT_STAT_VOLT_LOW);
#if (DEBUG_CONSOLE_EN > 0u)
        sprintf(message, "Debug! Cell %u voltage low.\r\n", cell_index);
        debug_console(message);
#endif
    } else if (voltage < BATT_CELL_VOLT_HIGH) {
        result = (BATT_STAT_GOOD | BATT_STAT_VOLT_OPTIM);
#if (DEBUG_CONSOLE_EN > 0u)
        sprintf(message, "Debug! Cell %u voltage optim.\r\n", cell_index);
        debug_console(message);
#endif
    } else if (voltage < BATT_CELL_VOLT_MAX_RESTING) {
#if (DEBUG_CONSOLE_EN > 0u)
        sprintf(message, "Debug! Cell %u voltage high.\r\n", cell_index);
        debug_console(message);
#endif
        result = (BATT_STAT_GOOD | BATT_STAT_VOLT_HIGH);
    } else if (voltage == BATT_CELL_VOLT_MAX_RESTING) {
        if (battery_status & BATT_STAT_CHARGING) {
            result = (BATT_STAT_GOOD | BATT_STAT_VOLT_HIGH);
#if (DEBUG_CONSOLE_EN > 0u)
            sprintf(message, "Debug! Cell %u voltage high.\r\n", cell_index);
            debug_console(message);
#endif
        } else {
            result = (BATT_STAT_GOOD | BATT_STAT_VOLT_FULL);
#if (DEBUG_CONSOLE_EN > 0u)
            sprintf(message, "Debug! Cell %u none-charging voltage full.\r\n", cell_index);
            debug_console(message);
#endif
        }
    } else if (voltage < BATT_CELL_VOLT_MAX_CHARGING) {
        if (battery_status & BATT_STAT_CHARGING) {
            result = (BATT_STAT_GOOD | BATT_STAT_VOLT_HIGH);
#if (DEBUG_CONSOLE_EN > 0u)
            sprintf(message, "Debug! Cell %u charging voltage full.\r\n", cell_index);
            debug_console(message);
#endif
        } else {
#ifndef TEST_NATIVE
            if (!(BATT_STAT_ERROR & battery_status)) {
                mosfet_control_set(MOSFET_CONTROL_DISCHARGE);
            }
#endif
            result = (BATT_STAT_ERROR | BATT_STAT_VOLT_FULL);
#if (DEBUG_CONSOLE_EN > 0u)
            sprintf(message, "Error! Cell %u none-charging voltage too high.\r\n", cell_index);
            debug_console(message);
#endif
        }
    } else if (voltage == BATT_CELL_VOLT_MAX_CHARGING) {
        if (battery_status & BATT_STAT_CHARGING) {
            result = (BATT_STAT_GOOD | BATT_STAT_VOLT_FULL);
#if (DEBUG_CONSOLE_EN > 0u)
            sprintf(message, "Debug! Cell %u charging voltage high.\r\n", cell_index);
            debug_console(message);
#endif
        } else {
#ifndef TEST_NATIVE
            if (!(BATT_STAT_ERROR & battery_status)) {
                mosfet_control_set(MOSFET_CONTROL_DISCHARGE);
            }
#endif
            result = (BATT_STAT_ERROR | BATT_STAT_VOLT_FULL);
#if (DEBUG_CONSOLE_EN > 0u)
            sprintf(message, "Error! Cell %u none-charging voltage too high.\r\n", cell_index);
            debug_console(message);
#endif
        }
    } else {
#ifndef TEST_NATIVE
        if (!(BATT_STAT_ERROR & battery_status)) {
            mosfet_control_set(MOSFET_CONTROL_DISCHARGE);
        }
#endif
        result = (BATT_STAT_ERROR | BATT_STAT_VOLT_FULL);
#if (DEBUG_CONSOLE_EN > 0u)
        sprintf(message, "Error! Cell %u voltage too high.\r\n", cell_index);
        debug_console(message);
#endif
    }
    battery_manager_status_overwrite(result, BATT_STAT_VOLT_MASK);
#if (DEBUG_CONSOLE_EN > 0u)
    sprintf(message, "Debug! Cell %u Voltage: %umV Battery status: 0x%lx\r\n", cell_index, voltage,
            battery_status);
    debug_console(message);
#endif
    return result;
}

/**
 * \brief Evaluate the battery status by giving current.
 * \param[in] current Batter temperature, sensor range -55C - 150C.
 * \return BATTERY_STATUS self-explain.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
BATTERY_STATUS battery_manager_current_check(int16_t current) {
    BATTERY_STATUS result = BATT_STAT_GOOD;
    if (current == BATT_CURRENT_MIN) {
        result = (BATT_STAT_GOOD | BATT_STAT_REST);
    } else {
        /* Check for charging/discharging. */
        if (current < BATT_CURRENT_MIN) {
            // Discharging
            current = 0 - current;  // flip the sign for easier
                                    // No Discharging circuit yet. Have to fully open mosfets.
#ifndef TEST_NATIVE
            if (!(BATT_STAT_ERROR & battery_status)) {
                mosfet_control_set(MOSFET_CONTROL_OPEN);
            }
#endif
            result |= BATT_STAT_DISCHARGING;
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("Warning! Battery is at Rest or Discharging. Set Mosfet fully on.\r\n");
#endif
        } else {
            // Charing
            result |= BATT_STAT_CHARGING;
            /* Check for low, high, mthresh, optim */
            if (current < BATT_CURRENT_MTHRESH - BATT_CURRENT_SAFETY_OFFSET) {
#ifndef TEST_NATIVE
                if (!(BATT_STAT_ERROR & battery_status)) {
                    mosfet_control_set(MOSFET_CONTROL_CHARGE);
                }
#endif
                result |= (BATT_STAT_GOOD | BATT_STAT_CURRENT_LOW);
#if (DEBUG_CONSOLE_EN > 0u)
                debug_console("Debug! Battery current is low.\r\n");
#endif
            } else if (current < BATT_CURRENT_OPTIM) {
#ifndef TEST_NATIVE
                if (!(BATT_STAT_ERROR & battery_status)) {
                    mosfet_control_set(MOSFET_CONTROL_OPEN);
                }
#endif
                result |= (BATT_STAT_GOOD | BATT_STAT_CURRENT_LOW | BATT_STAT_CURRENT_MTHRESH);
#if (DEBUG_CONSOLE_EN > 0u)
                debug_console(
                    "Debug! Battery charging current is low. Set battery charging only.\r\n");
#endif
            } else if (current < BATT_CURRENT_MAX - BATT_CURRENT_SAFETY_OFFSET) {
#ifndef TEST_NATIVE
                if (!(BATT_STAT_ERROR & battery_status)) {
                    mosfet_control_set(MOSFET_CONTROL_OPEN);
                }
#endif
                result |= (BATT_STAT_GOOD | BATT_STAT_CURRENT_OPTIM | BATT_STAT_CURRENT_MTHRESH);
#if (DEBUG_CONSOLE_EN > 0u)
                debug_console("Debug! Battery current is optimum. Mosfet fully on.\r\n");
#endif
            } else if (current < BATT_CURRENT_MAX) {
#ifndef TEST_NATIVE
                if (!(BATT_STAT_ERROR & battery_status)) {
                    mosfet_control_set(MOSFET_CONTROL_OPEN);
                }
#endif
                result |= (BATT_STAT_GOOD | BATT_STAT_CURRENT_HIGH | BATT_STAT_CURRENT_MTHRESH);
#if (DEBUG_CONSOLE_EN > 0u)
                debug_console("Debug! Battery current is high. Mosfet fully on.\r\n");
#endif
            } else {
#ifndef TEST_NATIVE
                mosfet_control_set(MOSFET_CONTROL_CLOSED);
#endif
                result |= (BATT_STAT_ERROR | BATT_STAT_CURRENT_HIGH | BATT_STAT_CURRENT_MTHRESH);
#if (DEBUG_CONSOLE_EN > 0u)
                debug_console("Error! Battery current is too high. Battery disabled.\r\n");
#endif
            }
        }
    }
    battery_manager_status_overwrite(result, BATT_STAT_CURRENT_MASK);
#if (DEBUG_CONSOLE_EN > 0u)
    sprintf(message, "Debug! Battery Current: %umA Battery status: 0x%lx\r\n", current,
            battery_status);
    debug_console(message);
#endif
    return result;
}

/**
 * \brief Evaluate the battery status by giving temperature.
 * \param[in] temperature Batter temperature, sensor range -55C - 150C.
 * \return BATTERY_STATUS self-explain.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
BATTERY_STATUS battery_manager_temperature_check(const double temperature) {
    BATTERY_STATUS result = BATT_STAT_GOOD;
    if (temperature <= BATT_TEMPERATURE_MIN) {
#ifndef TEST_NATIVE
        if (!(BATT_STAT_ERROR & battery_status)) {
            mosfet_control_set(MOSFET_CONTROL_CLOSED);
        }
#endif
        result = (BATT_STAT_ERROR | BATT_STAT_TEMP_LOW);
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("Error! Battery temperature is too low. Battery disabled.\r\n");
#endif
    } else if (temperature <= BATT_TEMPERATURE_LOW) {
#ifndef TEST_NATIVE
        if (!(BATT_STAT_ERROR & battery_status)) {
            mosfet_control_set(MOSFET_CONTROL_OPEN);
        }
#endif
        result = (BATT_STAT_GOOD | BATT_STAT_TEMP_LOW);
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("Warning! Battery temperature is low.\r\n");
#endif
    } else if (temperature < BATT_TEMPERATURE_HIGH) {
#ifndef TEST_NATIVE
        if (!(BATT_STAT_ERROR & battery_status)) {
            mosfet_control_set(MOSFET_CONTROL_OPEN);
        }
#endif
        result = (BATT_STAT_GOOD | BATT_STAT_TEMP_OPTIM);
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("Debug! Battery temperature is optimum.\r\n");
#endif
    } else if (temperature < BATT_TEMPERATURE_MAX) {
#ifndef TEST_NATIVE
        if (!(BATT_STAT_ERROR & battery_status)) {
            mosfet_control_set(MOSFET_CONTROL_OPEN);
        }
#endif
        result = (BATT_STAT_GOOD | BATT_STAT_TEMP_HIGH);
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("Warning! Battery temperature is high.\r\n");
#endif
    } else {
#ifndef TEST_NATIVE
        if (!(BATT_STAT_ERROR & battery_status)) {
            mosfet_control_set(MOSFET_CONTROL_OPEN);
        }
#endif
        result = (BATT_STAT_ERROR | BATT_STAT_TEMP_HIGH);
#if (DEBUG_CONSOLE_EN > 0u)
        debug_console("Error! Battery temperature is too high. Battery disabled.\r\n");
#endif
    }
    battery_manager_status_overwrite(result, BATT_STAT_TEMPERATURE_MASK);
#if (DEBUG_CONSOLE_EN > 0u)
    sprintf(message, "Debug! Battery temperature: %dC Battery status: 0x%lx\r\n", (int)temperature,
            battery_status);
    debug_console(message);
#endif
    return result;
}

/**
 * \brief Overwrite local global variable battery_status.
 * \param[in] status_result The battery status returned from: cell_voltage_check(), current_check(),
 * and temperature_check().
 * \param[in] mask The mask for status bits.
 * \return BATTERY_STATUS self-explain.
 * \details There are four categories of status bits: Health, Voltage, Temperature and Current. The
 * bits should be set by corresponding masks.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
BATTERY_STATUS battery_manager_status_overwrite(const BATTERY_STATUS      status_result,
                                                const BATTERY_STATUS_MASK mask) {
    battery_status &= ~mask;
    battery_status |= status_result;
    return battery_status;
}

/**
 * \brief Set the battery status.
 * \param[in] status The status to be seted.
 * \return BATTERY_STATUS self-explain.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
BATTERY_STATUS battery_manager_status_set(const BATTERY_STATUS status) {
    battery_status |= status;
    return battery_status;
}

/**
 * \brief Reset battery status.
 * \param[in] status The status to be reseted.
 * \return BATTERY_STATUS self-explain.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
BATTERY_STATUS battery_manager_status_reset(const BATTERY_STATUS status) {
    battery_status &= ~status;
    return battery_status;
}

/**
 * \brief Return the current battery status.
 * \return BATTERY_STATUS self-explain.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
BATTERY_STATUS battery_manager_status_get(void) { return battery_status; }

/**
 * \brief Manage battery battery ON, OFF, CHARGING, DISCHARGING states
 * \return BATTERY_STATUS self-explain.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
BATTERY_STATUS battery_manager_management_routine(void) {
    uint32_t result = 0;
    switch (battery_status) {
        case (BATT_STAT_ERROR | BATT_STAT_CHARGING | BATT_STAT_CURRENT_HIGH):
            mosfet_control_set(MOSFET_CONTROL_CLOSED);
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("Error! Battery charging high current detected, Shutdown...");
#endif
            break;
        case (BATT_STAT_ERROR | BATT_STAT_CHARGING | BATT_STAT_DISCHARGING):
            mosfet_control_set(MOSFET_CONTROL_CLOSED);
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("Error! Battery discharging high current detected, Shutdown...");
#endif
            break;
        case (BATT_STAT_ERROR | BATT_STAT_VOLT_EMPTY):
            mosfet_control_set(MOSFET_CONTROL_CLOSED);
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("Error! Battery voltage too low, Shutdown...");
#endif
            break;
        case (BATT_STAT_ERROR | BATT_STAT_VOLT_HIGH):
            mosfet_control_set(MOSFET_CONTROL_CLOSED);
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("Error! Battery voltage too high, Shutdown...");
#endif
            break;
        case (BATT_STAT_ERROR | BATT_STAT_VOLT_LOW):
            mosfet_control_set(MOSFET_CONTROL_CLOSED);
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("Error! Battery voltage too low, Shutdown...");
#endif
            break;
        case (BATT_STAT_ERROR | BATT_STAT_TEMP_HIGH):
            mosfet_control_set(MOSFET_CONTROL_CLOSED);
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("Error! Battery temperature too high, Shutdown...");
#endif
            break;
        case (BATT_STAT_ERROR | BATT_STAT_TEMP_LOW):
            mosfet_control_set(MOSFET_CONTROL_CLOSED);
#if (DEBUG_CONSOLE_EN > 0u)
            debug_console("Error! Battery temperature too low, Shutdown...");
#endif
            break;
        case (BATT_STAT_GOOD | BATT_STAT_TEMP_HIGH):
        default:
            break;
    }
    return result;
}