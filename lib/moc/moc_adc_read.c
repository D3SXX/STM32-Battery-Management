#include "moc_adc_read.h"

#include "stm32l1xx.h"

bool adc_read_cell_voltage(int cell_number, uint16_t* data) {
    srand(SysTick->VAL);                       // Seed the random number generator
    double volt = (double)rand() / RAND_MAX * 2.0 + 2.0;  // random [2.0 - 4.0] volt
    *data       = (uint16_t)(volt * 10000);    // accuracy in 0.1 milivolt
    return true;
}

bool adc_read_batt_current(uint16_t* data) {
    srand(SysTick->VAL);                       // Seed the random number generator
    double volt = (double)rand() / RAND_MAX * 10.0;  // random [0.0 - 10.0] Amps
    *data       = (uint16_t)(volt * 10000);    // accuracy in 0.1 milivolt
    return true;
}
bool adc_read_batt_temp(uint16_t* data)
{
    // bad data
    return false;
}