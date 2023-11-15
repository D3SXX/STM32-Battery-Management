#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H
#include <stdbool.h>
#include <stdint.h>

#define BATT_STAT_GOOD             0x00UL
#define BATT_STAT_ERROR            0x01UL
#define BATT_STAT_VOLT_LOW         0x02UL
#define BATT_STAT_VOLT_HIGH        0x04UL
#define BATT_STAT_VOLT_EMPTY       0x08UL
#define BATT_STAT_VOLT_FULL        0x10UL
#define BATT_STAT_TEMPERATURE_LOW  0x20UL
#define BATT_STAT_TEMPERATURE_HIGH 0x40UL
#define BATT_STAT_CURRENT_LOW      0x80UL
#define BATT_STAT_CURRENT_HIGH     0x100UL
#define BATT_STAT_CHARGING         0x200ul
#define BATT_STAT_DISCHARGING      0x400UL
#define BATT_STAT_REST             0x800UL  // Neither charging nor discharging.

#define BATT_STAT_VOLT_MASK \
    (BATT_STAT_VOLT_LOW | BATT_STAT_VOLT_HIGH | BATT_STAT_VOLT_EMPTY | BATT_STAT_VOLT_FULL)
#define BATT_STAT_CURRENT_MASK                                                                     \
    (BATT_STAT_CURRENT_LOW | BATT_STAT_CURRENT_HIGH | BATT_STAT_CHARGING | BATT_STAT_DISCHARGING | \
     BATT_STAT_REST)
#define BATT_STAT_TEMPERATURE_MASK  (BATT_STAT_TEMPERATURE_LOW | BATT_STAT_TEMPERATURE_LOW)

#define BATTERY_STATUS              uint32_t
#define BATTERY_STATUS_MASK         uint32_t

#define BATT_CELL_VOLT_MIN          (uint16_t)2500  // 0%
#define BATT_CELL_VOLT_MAX_CHARGING (uint16_t)3650  // 100% charging
#define BATT_CELL_VOLT_MAX_RESTING  (uint16_t)3400  // 100% resting
#define BATT_CELL_VOLT_LOW          (uint16_t)3130  // 14%
#define BATT_CELL_VOLT_HIGH         (uint16_t)3330  // 90%

#define BATT_CURRENT_MIN            (uint16_t)2500  // 0%
#define BATT_CELL_VOLT_MAX_CHARGING (uint16_t)3650  // 100% charging
#define BATT_CELL_VOLT_MAX_RESTING  (uint16_t)3400  // 100% resting
#define BATT_CELL_VOLT_LOW          (uint16_t)3130  // 14%
#define BATT_CELL_VOLT_HIGH         (uint16_t)3330  // 90%

#define BATT_TEMPERATURE_MIN        (float)-20.0
#define BATT_TEMPERATURE_MAX        (float)60.0
#define BATT_TEMPERATURE_LOW        (float)0.0
#define BATT_TEMPERATURE_HIGH       (float)45.0

#define BATT_CURRENT_LOW            (uint16t)500
#define BATT_CURRENT_HIGH           (uint16t)1300
#define BATT_CURRENT_MIN            (uint16t)0
#define BATT_CURRENT_MAX            (uint16t)37600

BATTERY_STATUS battery_manager_cell_voltage_check(const uint16_t voltage);
BATTERY_STATUS battery_manager_voltage_check(const uint16_t voltage);
BATTERY_STATUS battery_manager_current_check(const uint16_t current);
BATTERY_STATUS battery_manager_temperature_check(const double temperature);
BATTERY_STATUS battery_manager_status_get(void);
BATTERY_STATUS battery_manager_status_set(const BATTERY_STATUS      status,
                                          const BATTERY_STATUS_MASK mask);
void           battery_manager_management_routine(void);
#endif