#include "battery_manager.h"

BATTERY_STATUS battery_status = BATT_STAT_GOOD;

BATTERY_STATUS battery_manager_cell_voltage_check(const uint16_t voltage) {
    BATTERY_STATUS result = BATT_STAT_GOOD;
    if (voltage <= BATT_CELL_VOLT_MIN) {
        result = (BATT_STAT_ERROR | BATT_STAT_VOLT_EMPTY);
    } else if (voltage <= BATT_CELL_VOLT_LOW) {
        result = (BATT_STAT_GOOD | BATT_STAT_VOLT_LOW);
    } else if (voltage < BATT_CELL_VOLT_HIGH) {
        result = (BATT_STAT_GOOD);
    } else if (voltage < BATT_CELL_VOLT_MAX_RESTING) {
        result = BATT_STAT_VOLT_HIGH;
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
    return result;
}

BATTERY_STATUS battery_manager_status_set(const BATTERY_STATUS      status,
                                          const BATTERY_STATUS_MASK mask) {
    battery_status &= ~mask;
    battery_status |= status;
    return battery_status;
}

BATTERY_STATUS battery_manager_status_get(void) { return battery_status; }