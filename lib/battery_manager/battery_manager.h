#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H
#include <stdbool.h>
#include <stdint.h>

/* BATT_STAT */
/* Bit0 indicates GOOD or ERROR condition of the battery. */
#define BATT_STAT_GOOD  0x00UL
#define BATT_STAT_ERROR 0x01UL
/* BATT_STAT_VOLT*/
/* Bit1-5 indicates the voltage level of the battery. */
#define BATT_STAT_VOLT_LOW   0x02UL
#define BATT_STAT_VOLT_HIGH  0x04UL
#define BATT_STAT_VOLT_EMPTY 0x08UL
#define BATT_STAT_VOLT_FULL  0x10UL
#define BATT_STAT_VOLT_OPTIM 0x20UL
/* BATT_STAT_TEMPERATURE*/
/* Bit6-8 indicates the temperature level of the battery. */
#define BATT_STAT_TEMP_LOW   0x20UL
#define BATT_STAT_TEMP_HIGH  0x40UL
#define BATT_STAT_TEMP_OPTIM 0x40UL
/* BATT_STAT_CURRENT*/
/* Bit9-14 indicates the current level and the charging status of the battery. */
#define BATT_STAT_CURRENT_LOW     0x80UL
#define BATT_STAT_CURRENT_HIGH    0x100UL
#define BATT_STAT_CURRENT_OPTIM   0x100UL
#define BATT_STAT_CURRENT_MTHRESH 0x200UL
#define BATT_STAT_CHARGING        0x400ul
#define BATT_STAT_DISCHARGING     0x800UL
#define BATT_STAT_REST            0x1000UL  // Neither charging nor discharging.

/* Masks */
#define BATT_STAT_HEALTH_MASK (BATT_STAT_GOOD | BATT_STAT_ERROR)
#define BATT_STAT_VOLT_MASK                                                                    \
    (BATT_STAT_HEALTH_MASK | BATT_STAT_VOLT_LOW | BATT_STAT_VOLT_HIGH | BATT_STAT_VOLT_EMPTY | \
     BATT_STAT_VOLT_FULL | BATT_STAT_VOLT_OPTIM)
#define BATT_STAT_CURRENT_MASK                                                                     \
    (BATT_STAT_HEALTH_MASK | BATT_STAT_CURRENT_LOW | BATT_STAT_CURRENT_HIGH | BATT_STAT_CHARGING | \
     BATT_STAT_DISCHARGING | BATT_STAT_REST | BATT_STAT_CURRENT_OPTIM | BATT_STAT_CURRENT_MTHRESH)
#define BATT_STAT_TEMPERATURE_MASK \
    (BATT_STAT_HEALTH_MASK | BATT_STAT_TEMP_LOW | BATT_STAT_TEMP_HIGH | BATT_STAT_TEMP_OPTIM)

#define BATTERY_STATUS      uint32_t
#define BATTERY_STATUS_MASK uint32_t
/* Voltages ratings in mV */
#define BATT_CELL_VOLT_MIN          (uint16_t)2500  // 0%
#define BATT_CELL_VOLT_MAX_CHARGING (uint16_t)3650  // 100% charging
#define BATT_CELL_VOLT_MAX_RESTING  (uint16_t)3400  // 100% resting
#define BATT_CELL_VOLT_LOW          (uint16_t)3130  // 14%
#define BATT_CELL_VOLT_HIGH         (uint16_t)3330  // 90%
/* Temperature ratings in Celcius Degrees */
#define BATT_TEMPERATURE_MIN  (float)-20.0
#define BATT_TEMPERATURE_MAX  (float)60.0
#define BATT_TEMPERATURE_LOW  (float)0.0
#define BATT_TEMPERATURE_HIGH (float)45.0
/* Current ratings in mA */
#define BATT_CURRENT_SAFETY_OFFSET (uint16_t)300  // Safety offset from absolute ratings.
#define BATT_CURRENT_LOW           (uint16_t)500
#define BATT_CURRENT_MTHRESH       (uint16_t)1300  // Max Continuous Drain–Source Diode Forward Current.
#define BATT_CURRENT_OPTIM         (uint16_t)5000
#define BATT_CURRENT_MIN           (uint16_t)0
#define BATT_CURRENT_MAX           (uint16_t)37600

BATTERY_STATUS battery_manager_cell_voltage_check(const uint16_t voltage);
BATTERY_STATUS battery_manager_voltage_check(const uint16_t voltage);
BATTERY_STATUS battery_manager_current_check(int32_t current);
BATTERY_STATUS battery_manager_temperature_check(const double temperature);
BATTERY_STATUS battery_manager_status_get(void);
BATTERY_STATUS battery_manager_status_set(const BATTERY_STATUS status_result);
BATTERY_STATUS battery_manager_status_overwrite(const BATTERY_STATUS      status,
                                                const BATTERY_STATUS_MASK mask);
void           battery_manager_management_routine(void);
#endif