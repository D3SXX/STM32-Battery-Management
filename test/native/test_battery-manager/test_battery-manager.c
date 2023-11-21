#include <unity.h>

#include "battery_manager.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_battery_manager_cell_voltage_check_error_empty(void) {
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_MIN);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_VOLT_EMPTY);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_cell_voltage_check_low(void) {
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_LOW);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_VOLT_LOW);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_cell_voltage_check_good_l(void) {
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_LOW + 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_VOLT_OPTIM);
    TEST_ASSERT_EQUAL(expect, result);
}
void test_battery_manager_cell_voltage_check_good_h(void) {
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_HIGH - 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_VOLT_OPTIM);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_cell_voltage_check_high_1(void) {
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_HIGH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_VOLT_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_cell_voltage_check_high_2(void) {
    battery_manager_status_overwrite(BATT_STAT_CHARGING, BATT_STAT_CURRENT_MASK);
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_MAX_RESTING);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_VOLT_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
    battery_manager_status_overwrite(BATT_STAT_GOOD, BATT_STAT_CURRENT_MASK);
}

void test_battery_manager_cell_voltage_check_high_3(void) {
    battery_manager_status_overwrite(BATT_STAT_CHARGING, BATT_STAT_CURRENT_MASK);
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_MAX_CHARGING - 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_VOLT_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
    battery_manager_status_overwrite(BATT_STAT_GOOD, BATT_STAT_CURRENT_MASK);
}

void test_battery_manager_cell_voltage_check_full_rest(void) {
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_MAX_RESTING);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_VOLT_FULL);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_cell_voltage_check_full_charging(void) {
    battery_manager_status_overwrite(BATT_STAT_CHARGING, BATT_STAT_CURRENT_MASK);
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_MAX_CHARGING);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_VOLT_FULL);
    TEST_ASSERT_EQUAL(expect, result);
    battery_manager_status_overwrite(BATT_STAT_GOOD, BATT_STAT_CURRENT_MASK);
}

void test_battery_manager_cell_voltage_check_error_full_1(void) {
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_MAX_RESTING + 1);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_VOLT_FULL);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_cell_voltage_check_error_full_2(void) {
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_MAX_CHARGING);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_VOLT_FULL);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_cell_voltage_check_error_full_3(void) {
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_MAX_CHARGING + 1);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_VOLT_FULL);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_cell_voltage_check_error_full_4(void) {
    battery_manager_status_overwrite(BATT_STAT_CHARGING, BATT_STAT_CURRENT_MASK);
    BATTERY_STATUS result = battery_manager_cell_voltage_check(0, BATT_CELL_VOLT_MAX_CHARGING + 1);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_VOLT_FULL);
    TEST_ASSERT_EQUAL(expect, result);
    battery_manager_status_overwrite(BATT_STAT_GOOD, BATT_STAT_CURRENT_MASK);
}

void test_battery_manager_temperature_check_error_low_1(void) {
    BATTERY_STATUS result = battery_manager_temperature_check(BATT_TEMPERATURE_MIN - 1);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_TEMP_LOW);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_temperature_check_error_low_2(void) {
    BATTERY_STATUS result = battery_manager_temperature_check(BATT_TEMPERATURE_MIN);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_TEMP_LOW);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_temperature_check_low_1(void) {
    BATTERY_STATUS result = battery_manager_temperature_check(BATT_TEMPERATURE_MIN + 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_TEMP_LOW);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_temperature_check_low_2(void) {
    BATTERY_STATUS result = battery_manager_temperature_check(BATT_TEMPERATURE_LOW);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_TEMP_LOW);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_temperature_check_good_1(void) {
    BATTERY_STATUS result = battery_manager_temperature_check(BATT_TEMPERATURE_LOW + 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_TEMP_OPTIM);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_temperature_check_good_2(void) {
    BATTERY_STATUS result = battery_manager_temperature_check(BATT_TEMPERATURE_HIGH - 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_TEMP_OPTIM);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_temperature_check_high_1(void) {
    BATTERY_STATUS result = battery_manager_temperature_check(BATT_TEMPERATURE_HIGH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_TEMP_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_temperature_check_high_2(void) {
    BATTERY_STATUS result = battery_manager_temperature_check(BATT_TEMPERATURE_MAX - 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_TEMP_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_temperature_check_error_high_1(void) {
    BATTERY_STATUS result = battery_manager_temperature_check(BATT_TEMPERATURE_MAX);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_TEMP_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_temperature_check_error_high_2(void) {
    BATTERY_STATUS result = battery_manager_temperature_check(BATT_TEMPERATURE_MAX + 1);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_TEMP_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_rest(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MIN);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_REST);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_low_1(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MIN + 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_LOW);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_low_2(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_LOW);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_LOW);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_low_3(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_LOW + 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_LOW);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_low_4(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MTHRESH - BATT_CURRENT_SAFETY_OFFSET - 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_LOW);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_low_thresh_1(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MTHRESH - BATT_CURRENT_SAFETY_OFFSET);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_LOW | BATT_STAT_CURRENT_MTHRESH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_low_thresh_2(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MTHRESH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_LOW | BATT_STAT_CURRENT_MTHRESH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_low_thresh_3(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_OPTIM - 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_LOW | BATT_STAT_CURRENT_MTHRESH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_optim_1(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_OPTIM);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_OPTIM);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_optim_2(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_OPTIM + 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_OPTIM);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_optim_3(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MAX - BATT_CURRENT_SAFETY_OFFSET - 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_OPTIM);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_high_1(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MAX - BATT_CURRENT_SAFETY_OFFSET);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_high_2(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MAX - BATT_CURRENT_SAFETY_OFFSET + 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_high_3(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MAX - 1);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_CHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_error_1(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MAX);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_CHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_charge_error_2(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MAX + 1);
    BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_CHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_HIGH);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_low_1(void) {
    BATTERY_STATUS result = battery_manager_current_check(BATT_CURRENT_MIN - 1);
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_LOW);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_low_2(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - BATT_CURRENT_LOW);
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_LOW);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_low_3(void) {
    BATTERY_STATUS result = battery_manager_current_check(0- (BATT_CURRENT_LOW + 1));
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_LOW);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_low_4(void) {
    BATTERY_STATUS result = battery_manager_current_check(0- (BATT_CURRENT_MTHRESH - BATT_CURRENT_SAFETY_OFFSET - 1));
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_LOW);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_low_thresh_1(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - (BATT_CURRENT_MTHRESH - BATT_CURRENT_SAFETY_OFFSET));
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_LOW | BATT_STAT_CURRENT_MTHRESH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_low_thresh_2(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - BATT_CURRENT_MTHRESH);
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_LOW | BATT_STAT_CURRENT_MTHRESH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_low_thresh_3(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - (BATT_CURRENT_OPTIM - 1));
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_LOW | BATT_STAT_CURRENT_MTHRESH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_optim_1(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - (BATT_CURRENT_OPTIM));
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_OPTIM);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_optim_2(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - (BATT_CURRENT_OPTIM + 1));
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_OPTIM);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_optim_3(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - (BATT_CURRENT_MAX - BATT_CURRENT_SAFETY_OFFSET - 1));
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_OPTIM);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_high_1(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - (BATT_CURRENT_MAX - BATT_CURRENT_SAFETY_OFFSET));
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_HIGH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_high_2(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - (BATT_CURRENT_MAX - BATT_CURRENT_SAFETY_OFFSET + 1));
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_HIGH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_high_3(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - (BATT_CURRENT_MAX - 1));
    // BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_HIGH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_error_1(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - BATT_CURRENT_MAX);
    // BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_HIGH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}

void test_battery_manager_current_check_discharge_error_2(void) {
    BATTERY_STATUS result = battery_manager_current_check(0 - (BATT_CURRENT_MAX + 1));
    // BATTERY_STATUS expect = (BATT_STAT_ERROR | BATT_STAT_DISCHARGING | BATT_STAT_CURRENT_MTHRESH | BATT_STAT_CURRENT_HIGH);
    BATTERY_STATUS expect = (BATT_STAT_GOOD | BATT_STAT_DISCHARGING);
    TEST_ASSERT_EQUAL(expect, result);
}


int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_battery_manager_cell_voltage_check_error_empty);
    RUN_TEST(test_battery_manager_cell_voltage_check_low);
    RUN_TEST(test_battery_manager_cell_voltage_check_good_l);
    RUN_TEST(test_battery_manager_cell_voltage_check_good_h);
    RUN_TEST(test_battery_manager_cell_voltage_check_high_1);
    RUN_TEST(test_battery_manager_cell_voltage_check_high_2);
    RUN_TEST(test_battery_manager_cell_voltage_check_high_3);
    RUN_TEST(test_battery_manager_cell_voltage_check_full_rest);
    RUN_TEST(test_battery_manager_cell_voltage_check_full_charging);
    RUN_TEST(test_battery_manager_cell_voltage_check_error_full_1);
    RUN_TEST(test_battery_manager_cell_voltage_check_error_full_2);
    RUN_TEST(test_battery_manager_cell_voltage_check_error_full_3);
    RUN_TEST(test_battery_manager_cell_voltage_check_error_full_4);

    RUN_TEST(test_battery_manager_temperature_check_error_low_1);
    RUN_TEST(test_battery_manager_temperature_check_error_low_2);
    RUN_TEST(test_battery_manager_temperature_check_low_1);
    RUN_TEST(test_battery_manager_temperature_check_low_2);
    RUN_TEST(test_battery_manager_temperature_check_good_1);
    RUN_TEST(test_battery_manager_temperature_check_good_2);
    RUN_TEST(test_battery_manager_temperature_check_high_1);
    RUN_TEST(test_battery_manager_temperature_check_high_2);
    RUN_TEST(test_battery_manager_temperature_check_error_high_1);
    RUN_TEST(test_battery_manager_temperature_check_error_high_2);

    RUN_TEST(test_battery_manager_current_check_rest);
    RUN_TEST(test_battery_manager_current_check_charge_low_1);
    RUN_TEST(test_battery_manager_current_check_charge_low_2);
    RUN_TEST(test_battery_manager_current_check_charge_low_3);
    RUN_TEST(test_battery_manager_current_check_charge_low_4);
    RUN_TEST(test_battery_manager_current_check_charge_low_thresh_1);
    RUN_TEST(test_battery_manager_current_check_charge_low_thresh_2);
    RUN_TEST(test_battery_manager_current_check_charge_low_thresh_3);
    RUN_TEST(test_battery_manager_current_check_charge_optim_1);
    RUN_TEST(test_battery_manager_current_check_charge_optim_2);
    RUN_TEST(test_battery_manager_current_check_charge_optim_3);
    RUN_TEST(test_battery_manager_current_check_charge_high_1);
    RUN_TEST(test_battery_manager_current_check_charge_high_2);
    RUN_TEST(test_battery_manager_current_check_charge_high_3);
    RUN_TEST(test_battery_manager_current_check_charge_error_1);
    RUN_TEST(test_battery_manager_current_check_charge_error_2);
    
    RUN_TEST(test_battery_manager_current_check_discharge_low_1);
    RUN_TEST(test_battery_manager_current_check_discharge_low_2);
    RUN_TEST(test_battery_manager_current_check_discharge_low_3);
    RUN_TEST(test_battery_manager_current_check_discharge_low_4);
    RUN_TEST(test_battery_manager_current_check_discharge_low_thresh_1);
    RUN_TEST(test_battery_manager_current_check_discharge_low_thresh_2);
    RUN_TEST(test_battery_manager_current_check_discharge_low_thresh_3);
    RUN_TEST(test_battery_manager_current_check_discharge_optim_1);
    RUN_TEST(test_battery_manager_current_check_discharge_optim_2);
    RUN_TEST(test_battery_manager_current_check_discharge_optim_3);
    RUN_TEST(test_battery_manager_current_check_discharge_high_1);
    RUN_TEST(test_battery_manager_current_check_discharge_high_2);
    RUN_TEST(test_battery_manager_current_check_discharge_high_3);
    RUN_TEST(test_battery_manager_current_check_discharge_error_1);
    RUN_TEST(test_battery_manager_current_check_discharge_error_2);
    
    UNITY_END();

    while (1) {
    }
}
