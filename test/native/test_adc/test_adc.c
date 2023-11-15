#include <unity.h>

#include "adc_utils.h"
#include "moc_adc_read.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_adc_convert_batt_current_positive(void) {
    uint16_t adc_value = 0;
    uint16_t current   = 0;
    moc_adc_set(3732);  // should be 37590mA
    adc_value = moc_adc_read((uint8_t)0xff);
    current   = adc_convert_batt_current(adc_value);
    TEST_ASSERT_EQUAL((uint16_t)37593, current);
}

void test_adc_convert_batt_current_negative(void) {
    uint16_t adc_value = 0;
    uint16_t current   = 0;
    moc_adc_set(3733);
    adc_value = moc_adc_read((uint8_t)0xff);
    current   = adc_convert_batt_current(adc_value);
    TEST_ASSERT_NOT_EQUAL((uint16_t)37600, current);
}

void test_adc_convert_batt_current_edge_max(void) {
    uint16_t adc_value = 0;
    uint16_t current   = 0;
    moc_adc_set(4095);
    adc_value = moc_adc_read((uint8_t)0xff);
    current   = adc_convert_batt_current(adc_value);
    TEST_ASSERT_EQUAL((uint16_t)41250, current);
}

void test_adc_convert_batt_current_edge_min(void) {
    uint16_t adc_value = 0;
    uint16_t current   = 0;
    moc_adc_set(0);
    adc_value = moc_adc_read((uint8_t)0xff);
    current   = adc_convert_batt_current(adc_value);
    TEST_ASSERT_EQUAL((uint16_t)0, current);
}

void test_adc_convert_cell_voltage_positive(void) {
    uint16_t volta = 0;
    moc_adc_set(3623);
    volta = adc_convert_cell_voltage(moc_adc_read);
    TEST_ASSERT_EQUAL((uint16_t)3649, volta);
}

void test_adc_convert_cell_voltage_negatitive(void) {
    uint16_t volta = 0;
    moc_adc_set(3624);
    volta = adc_convert_cell_voltage(moc_adc_read);
    TEST_ASSERT_NOT_EQUAL((uint16_t)3649, volta);
}

void test_adc_convert_cell_voltage_edge_max(void) {
    uint16_t volta = 0;
    moc_adc_set(4095);
    volta = adc_convert_cell_voltage(moc_adc_read);
    TEST_ASSERT_EQUAL((uint16_t)4125, volta);
}

void test_adc_convert_cell_voltage_edge_min(void) {
    uint16_t volta = 0;
    moc_adc_set(4095);
    volta = adc_convert_cell_voltage(moc_adc_read);
    TEST_ASSERT_EQUAL((uint16_t)4125, volta);
}

void test_adc_convert_temperature_positive(void) {
    int16_t temp_int = 0;
    moc_adc_set(LM35_ADC_REF);
    temp_int = (int16_t)adc_convert_batt_temp(moc_adc_read);
    TEST_ASSERT_EQUAL((int16_t)0, temp_int);
}

void test_adc_convert_temperature_negative(void) {
    int16_t temp_int = 0;
    moc_adc_set(LM35_ADC_REF - 10);
    temp_int = (int16_t)adc_convert_batt_temp(moc_adc_read);
    TEST_ASSERT_NOT_EQUAL((int16_t)0, temp_int);
}

void test_adc_convert_temperature_edge_max(void) {
    int16_t temp_int = 0;
    moc_adc_set(LM35_ADC_MAX);
    temp_int = (int16_t)adc_convert_batt_temp(moc_adc_read);
    TEST_ASSERT_EQUAL((int16_t)1500, temp_int);
}

void test_adc_convert_temperature_edge_min(void) {
    int16_t temp_int = 0;
    moc_adc_set(LM35_ADC_MIN);
    temp_int = (int16_t)adc_convert_batt_temp(moc_adc_read);
    TEST_ASSERT_EQUAL((int16_t)-549, temp_int);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_adc_convert_batt_current_positive);

    RUN_TEST(test_adc_convert_batt_current_negative);

    RUN_TEST(test_adc_convert_batt_current_edge_max);

    RUN_TEST(test_adc_convert_batt_current_edge_min);

    RUN_TEST(test_adc_convert_cell_voltage_positive);

    RUN_TEST(test_adc_convert_cell_voltage_negatitive);

    RUN_TEST(test_adc_convert_cell_voltage_edge_max);

    RUN_TEST(test_adc_convert_cell_voltage_edge_min);

    RUN_TEST(test_adc_convert_temperature_positive);

    RUN_TEST(test_adc_convert_temperature_negative);

    RUN_TEST(test_adc_convert_temperature_edge_max);

    RUN_TEST(test_adc_convert_temperature_edge_min);

    UNITY_END();
}
