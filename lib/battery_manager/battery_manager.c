#include "battery_manager.h"

BATTERY_STATUS battery_status = BATT_STAT_GOOD;

/**
 * \brief Evaluate the battery status by giving cell voltage.
 * \param[in] voltage Cell voltage value, positive only.
 * \return BATTERY_STATUS self-explain.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
BATTERY_STATUS battery_manager_cell_voltage_check(const uint16_t voltage) {
    BATTERY_STATUS result = BATT_STAT_GOOD;
    if (voltage <= BATT_CELL_VOLT_MIN) {
        result = (BATT_STAT_ERROR | BATT_STAT_VOLT_EMPTY);
    } else if (voltage <= BATT_CELL_VOLT_LOW) {
        result = (BATT_STAT_GOOD | BATT_STAT_VOLT_LOW);
    } else if (voltage < BATT_CELL_VOLT_HIGH) {
        result = (BATT_STAT_GOOD | BATT_STAT_VOLT_OPTIM);
    } else if (voltage < BATT_CELL_VOLT_MAX_RESTING) {
        result = (BATT_STAT_GOOD | BATT_STAT_VOLT_HIGH);
    } else if (voltage == BATT_CELL_VOLT_MAX_RESTING) {
        if (battery_status & BATT_STAT_CHARGING) {
            result = (BATT_STAT_GOOD | BATT_STAT_VOLT_HIGH);
        } else {
            result = (BATT_STAT_GOOD | BATT_STAT_VOLT_FULL);
        }
    } else if (voltage < BATT_CELL_VOLT_MAX_CHARGING) {
        if (battery_status & BATT_STAT_CHARGING) {
            result = (BATT_STAT_GOOD | BATT_STAT_VOLT_HIGH);
        } else {
            result = (BATT_STAT_ERROR | BATT_STAT_VOLT_FULL);
        }
    } else if (voltage == BATT_CELL_VOLT_MAX_CHARGING) {
        if (battery_status & BATT_STAT_CHARGING) {
            result = (BATT_STAT_GOOD | BATT_STAT_VOLT_FULL);
        } else {
            result = (BATT_STAT_ERROR | BATT_STAT_VOLT_FULL);
        }
    } else {
        result = (BATT_STAT_ERROR | BATT_STAT_VOLT_FULL);
    }
    battery_manager_status_overwrite(result, BATT_STAT_VOLT_MASK);
    return result;
}

/**
 * \brief Evaluate the battery status by giving current.
 * \param[in] current Batter temperature, sensor range -55C - 150C.
 * \return BATTERY_STATUS self-explain.
 * \author siyuan xu, e2101066@edu.vamk.fi, Nov.2023
 */
BATTERY_STATUS battery_manager_current_check(int32_t current) {
    BATTERY_STATUS result = BATT_STAT_GOOD;
    if (current == BATT_CURRENT_MIN) {
        result = (BATT_STAT_GOOD | BATT_STAT_REST);
    } else {
        /* Check for charging/discharging. */
        if (current < BATT_CURRENT_MIN) {
            // Discharging
            current = 0 - current;  // flip the sign for easier
            result |= BATT_STAT_DISCHARGING;
        } else {
            // Charing
            result |= BATT_STAT_CHARGING;
        }
        /* Check for low, high, mthresh, optim */
        if (current < BATT_CURRENT_MTHRESH - BATT_CURRENT_SAFETY_OFFSET) {
            result |= (BATT_STAT_GOOD | BATT_STAT_CURRENT_LOW);
        } else if (current < BATT_CURRENT_OPTIM) {
            result |= (BATT_STAT_GOOD | BATT_STAT_CURRENT_LOW | BATT_STAT_CURRENT_MTHRESH);
        } else if (current < BATT_CURRENT_MAX - BATT_CURRENT_SAFETY_OFFSET) {
            result |= (BATT_STAT_GOOD | BATT_STAT_CURRENT_OPTIM | BATT_STAT_CURRENT_MTHRESH);
        } else if (current < BATT_CURRENT_MAX) {
            result |= (BATT_STAT_GOOD | BATT_STAT_CURRENT_HIGH | BATT_STAT_CURRENT_MTHRESH);
        } else {
            result |= (BATT_STAT_ERROR | BATT_STAT_CURRENT_HIGH | BATT_STAT_CURRENT_MTHRESH);
        }
    }
    battery_manager_status_overwrite(result, BATT_STAT_CURRENT_MASK);
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
        result = (BATT_STAT_ERROR | BATT_STAT_TEMP_LOW);
    } else if (temperature <= BATT_TEMPERATURE_LOW) {
        result = (BATT_STAT_GOOD | BATT_STAT_TEMP_LOW);
    } else if (temperature < BATT_TEMPERATURE_HIGH) {
        result = (BATT_STAT_GOOD | BATT_STAT_TEMP_OPTIM);
    } else if (temperature < BATT_TEMPERATURE_MAX) {
        result = (BATT_STAT_GOOD | BATT_STAT_TEMP_HIGH);
    } else {
        result = (BATT_STAT_ERROR | BATT_STAT_TEMP_HIGH);
    }
    battery_manager_status_overwrite(result, BATT_STAT_TEMPERATURE_MASK);
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