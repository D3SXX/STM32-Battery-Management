#ifndef ADC
#define ADC
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>



u_int16_t find_median(int arr[],int len);

bool mux_read (int *chan, uint32_t C,uint32_t B,uint32_t A, int *data);

uint16_t adc_to_volts(const uint16_t adc, const uint16_t chan);

#endif
