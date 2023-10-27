#include "moc_adc_read.h"

#include "stm32l1xx.h"

uint16_t moc_adc_read_cell_voltage(void) {
    srand(SysTick->VAL);                       // Seed the random number generator
    double volt = (double)rand() / RAND_MAX * 2.0 + 2.0;  // random [2.0 - 4.0] volt
    return (uint16_t)(volt * 10000);    // accuracy in 0.1 milivolt.
}

uint16_t moc_adc_read_batt_current(void) {
    srand(SysTick->VAL);                       // Seed the random number generator
    double volt = (double)rand() / RAND_MAX * 10.0;  // random [0.0 - 10.0] Amps
    return (uint16_t)(volt * 10000);    // accuracy in 0.1 milivolt.
}

uint16_t moc_adc_read_batt_temp(void)
{
    srand(SysTick->VAL);                       // Seed the random number generator
    double temp = ((double)rand() / RAND_MAX - 0.5) * 200.0;  // random [-100.0 - 100.0] Celcius
    int16_t temp_int = (int)(temp * 10);
    uint16_t encoded_temp = (uint16_t)temp_int;
    return encoded_temp;    // accuracy in 0.1 celcius.
}