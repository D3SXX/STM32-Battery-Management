/*Authors : Vladislav R., Date : 10.11.2023 
Description: This is modified lm35-LL-v2 code that works using CD4051BE multiplexer
Made for unit testing
*/

#include "ADC.h"

// We are testing 3 cases here
// First test - testing ADC conversion to the actual volts
// (It will be different for lm35 and battery etc.)
// Second test - Edge cases
// (If the value for ADC (voltage) is not in the range it is supposed to be)
// Third test - Mux test
// (See if the function correctly works with mux)


uint16_t adc_to_volts(const uint16_t adc){

		float voltage = adc * (3.3/4095); // Calculate voltage
        return voltage;

}

u_int16_t find_median(int arr[],int len){
	int apu;
		/*Sort array and get medium value*/
		for(int k=0;k<len;k++)
			{
				for(int i=0;i<len;i++)
				{
					if(arr[i]>arr[i+1])
					{
						apu=arr[i];
						arr[i]=arr[i+1];
						arr[i+1]=apu;
					}
				}
			}	
    if (len % 2 == 0)
    {
        /*If the length is even, return the average of the two middle values*/
        return (arr[len / 2 - 1] + arr[len / 2]) / 2;
    }
    else
    {
        /*If the length is odd, return the middle value*/
        return arr[len / 2];
	}
    
}

bool adc_edge(const uint16_t adc, const uint16_t chan){

    switch(chan){
        case 1:
        case 2:
        case 3:
        case 4:
            if (adc < 3102){return false;}else{return true;}  // checking if battery cell voltage < 2.5V
        break;
        case 5:
        return true;
        case 6:
            if(adc > 1861){return false;}else{return true;} // checking if lm35 voltage > 1.5V
        default:
            return true;
    }

}

bool mux_read (int *chan, uint32_t C,uint32_t B,uint32_t A, int *data){ // Possibly need to change A and C  
bool mux_state[3];

// Modify a value thru pointer, return Bool (to do)
int read_times = 5;  // Specify the amount of reads
int adc_values[read_times];
if(*chan > 7) *chan = 0;
switch (*chan) // C B A
{
case 0: // 0 0 0
    mux_state[0] = false;
    mux_state[1] = false;
    mux_state[2] = false;
    break;
case 1: // 0 0 1
    mux_state[0] = true;
    mux_state[1] = false;
    mux_state[2] = false;
    break;
case 2 : // 0 1 0
    mux_state[0] = false;
    mux_state[1] = true;
    mux_state[2] = false;
    break;
case 3 : // 0 1 1
    mux_state[0] = true;
    mux_state[1] = true;
    mux_state[2] = false;
    break;
case 4 : // 1 0 0
    mux_state[0] = false;
    mux_state[1] = false;
    mux_state[2] = true;
    break;
case 5 : // 1 0 1
    mux_state[0] = true;
    mux_state[1] = false;
    mux_state[2] = true;
    break;
case 6 : // 1 1 0
    mux_state[0] = false;
    mux_state[1] = true;
    mux_state[2] = true;
    break;
case 7 : // 1 1 1
    mux_state[0] = true;
    mux_state[1] = true;
    mux_state[2] = true;
    break;
default: // 0 0 0
    mux_state[0] = false;
    mux_state[1] = false;
    mux_state[2] = false;
    break;
}

	for(int k=0;k<read_times-1;k++)
	{
		adc_values[k] = read_adc_A0(); // Read A0 and record its output to an array
	}
	*data = find_median(adc_values,read_times);

	if(*chan <= 7)
		(*chan)++;
	/*Example of implementing False*/
	if (adc_edge(*data, chan)) return true;
    return false;
}