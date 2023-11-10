#include <unity.h>
#include <stdbool.h>
#include <string.h>
#define UNITY_INCLUDE_PRINT_FORMATTED
#define size 10
void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}
void find_median(){
//mock data
int arr[5]={4095, 3500, 3200, 3700, 3900};
int median;
	int apu;
		/*Sort array and get medium value*/
		for(int k=0;k<5;k++)
			{
				for(int i=0;i<5;i++)
				{
					if(arr[i]>arr[i+1])
					{
						apu=arr[i];
						arr[i]=arr[i+1];
						arr[i+1]=apu;
					}
				}
			}	
    if (5 % 2 == 0)
    {
        /*If the length is even, return the average of the two middle values*/
        median =  (arr[5 / 2 - 1] + arr[5 / 2]) / 2;
    }
    else
    {
        /*If the length is odd, return the middle value*/
        median =  arr[5 / 2];
	}
    TEST_ASSERT_EQUAL(3700, median);
    
}


void adc_edge(){
int adc[7] = {3103, 3300, 3400, 3700, 3800, 3900, 4000};
bool check[7];
for ( int chan = 0; chan < 7; ++chan)
{
    switch(chan){
        case 0:
        case 1:
        case 2:
        case 3: 
            if (adc[chan] < 3102){check[chan] =  false;}else{check[chan] =  true;}  // checking if battery cell voltage < 2.5V
        break;
        case 4:
        check[chan] =  true;
        break;
        case 5:
            if(adc[chan] > 1861){check[chan] =  false;}else{check[chan] =  true;} // checking if lm35 voltage > 1.5V
            break;
        default:
            check[chan] =  true;
    }
}


for ( int i = 0; i < 5; i++)
    {
        TEST_ASSERT_TRUE(check[i]);
    }
    TEST_ASSERT_FALSE(check[5]);
    TEST_ASSERT_TRUE(check[6]);

}
/*
void mux_read (){
//mux mock data
int *chan = 4;
bool A = FALSE;
bool B = TRUE;
bool C = FALSE;


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
	LL_mDelay(5); // We have to wait for a switch (about 1 ms measured with oscilloscope), otherwise we get garbage values
	/*Read channels read_times times
	//adc_a0_config(); // Configure ADC only once when starting the readings
	for(int k=0;k<read_times-1;k++)
	{
			adc_values[k] = read_adc_A0(); // Read A0 and record its output to an array
			LL_mDelay(1); // 1 ms delay for each reading
	}
	*data = find_median(adc_values,read_times);

	if(*chan <= 7)
		(*chan)++;
	/*Example of implementing False
	if (*chan-1 == 1 && *data > 1861) return false; // We check if channel 1 has voltage over 1.5 Volts

}
*/
void voltconversion(){
    //adc read volt conversion
    //mock data
    bool check[size];
    float voltage[size];
    int adc[size] = {3000,3150, 3200, 3300, 3500, 3750, 4000, 4090, 4092, 4095}; //ADC values inbetween 2.5v and 3.3V
    for (int8_t i = 0; i < size; i++)
    {
        check[i]=true;
	    voltage[i] = adc[i] * (3.3/4095); // Calculate voltage
        if(voltage[i]<1.5){check[i] = false;} //lower limit for ADC
        if(voltage[i]>3.3){check[i] = false;} //upper liit for ADC
        TEST_ASSERT(check[i]);
    }
      

}

int main() {
    UNITY_BEGIN();
    RUN_TEST(find_median);
    RUN_TEST(adc_edge);
    RUN_TEST(voltconversion);
    UNITY_END();
}

