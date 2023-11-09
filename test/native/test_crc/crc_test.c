#include "crc.h"

#include <unity.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_crc16_positive(void) {
    uint8_t  test_data[6]          = {05, 04, 00, 01, 00, 01};
    uint16_t crc16                 = CRC16(test_data, 6);
    uint16_t expected_crc16_result = ((uint16_t)142 << 8) + ((uint16_t)97);
    TEST_ASSERT_EQUAL(crc16, expected_crc16_result);
}

void test_crc16_negative(void) {
    uint8_t  test_data[6]          = {05, 04, 00, 01, 00, 01};
    uint16_t crc16                 = CRC16(test_data, 6);
    uint16_t expected_crc16_result = ((uint16_t)142 << 8) + ((uint16_t)90);
    TEST_ASSERT_NOT_EQUAL(crc16, expected_crc16_result);
}

void test_crc16_edge_max(void) {
    uint8_t  test_data[6]          = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint16_t crc16                 = CRC16(test_data, 6);
    uint16_t expected_crc16_result = ((uint16_t)0x94 << 8) + ((uint16_t)0x01);
    TEST_ASSERT_EQUAL(crc16, expected_crc16_result);
}

void test_crc16_edge_min(void) {
    uint8_t  test_data[6]          = {0, 0, 0, 0, 0, 0};
    uint16_t crc16                 = CRC16(test_data, 6);
    uint16_t expected_crc16_result = ((uint16_t)0x1b << 8) + ((uint16_t)0x00);
    TEST_ASSERT_EQUAL(crc16, expected_crc16_result);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_crc16_positive);
    RUN_TEST(test_crc16_negative);
    RUN_TEST(test_crc16_edge_max);
    RUN_TEST(test_crc16_edge_min);
    UNITY_END();
}