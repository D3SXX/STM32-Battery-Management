/* Authors : Vladislav R., Date : 23.11.2023 
Description: This is modified lm35-LL-v2 code that works using CD4051BE multiplexer

This code here showcases: 
Reading ADC values for Analog input A0, A1 and A3 with LL
Using USART with LL 
Using GPIO with LL (output using pins D3,D4,D5)
Manipulation of CD4051BE multiplexer
Measuring Negative and Positive temperature using lm35 

Changes:
Data for lm35 now is collected using A1 and A2 instead of using mux
Cleaned code (removed unused variables, old comments)
Added support for A2 input
Fixed a bug in find_median() (out of bounds)
Added correct limits for Mux and Lm35 (2.4V for Mux and 1.5 for lm35)
Reversed C and A in mux_read()


Notes:
Connect Multiplexer A, B and C to the pins D3, D4 and D5
Connect lm35 Positive pin to the A1 and Negative pin to the A2
*/

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include "nucleo152start.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stm32l1xx.h"
#include "stm32l1xx_ll_adc.h"
#include "stm32l1xx_ll_crc.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_exti.h"
#include "stm32l1xx_ll_usart.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_cortex.h"
#include "stm32l1xx_ll_exti.h"
#include "stm32l1xx_ll_dma.h"
#include <stdbool.h>
/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */
void USART2_Init(void);
void USART2_write(char data);
int find_median(int[], int);
char USART2_read(void);
int read_adc_A0(void);
bool mux_read(int *chan,uint32_t A,uint32_t B,uint32_t C, int *data);
bool lm35_read(int *data);
/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
  	/* Configure the system clock to 32 MHz and update SystemCoreClock */
	SetSysClock(); // Set system clock
	SystemCoreClockUpdate(); // Update global variable SystemCoreClock
	LL_SYSTICK_SetClkSource(SystemCoreClock); // Set clock's source to be SystemCoreClock for SysTick
	USART2_Init(); // Initialize USART2

	/*This function is needed for LL_mDelay()
	from p. 1213: This function configures the Cortex-M SysTick source to have 1ms time base.*/
	LL_Init1msTick(SystemCoreClock);

	/*Configure Analog pins (inputs)*/
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Enable clock for GPIOA (to configure its pins)
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG); // Set up pin A0 (PA0) for analog input
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG); // Set up pin A1 (PA1) for analog input
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG); // Set up pin A2 (PA4) for analog input

	/*Configure ADC1*/
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1); // Enable ADC1 Clock
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE); // Set ADC1 to Single Conversion Mode
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_384CYCLES); // Set Sampling Time for Channel 0 to 384 cycles
	LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B); // Set ADC Resolution to 12 bits
	LL_ADC_INJ_SetTriggerSource(ADC1,LL_ADC_INJ_TRIG_SOFTWARE); // Set Injection Trigger Source for ADC1 to Software Trigger

	/*Configure internal LED*/
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Enable clock for GPIOA (to configure its pins)
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT); // Set up pin 5 for output

	char buf[100];

	/*Pin Setup for A B C Mux Controls*/

	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); // Enable clock for GPIOB (to configure its pins)

	// Pin A
	LL_GPIO_SetPinOutputType(GPIOB,LL_GPIO_PIN_3,LL_GPIO_OUTPUT_PUSHPULL); // Configure pin as a push-pull output
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT); // Configure Pin to be output
	LL_GPIO_SetPinPull(GPIOB,LL_GPIO_PIN_3,LL_GPIO_PULL_DOWN); // Disable pull-up and pull-down resistors
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH); // Set pin speed to HIGH
	LL_GPIO_SetPinOutputType(GPIOB,LL_GPIO_PIN_4,LL_GPIO_OUTPUT_PUSHPULL); // Pin C
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT); 
	LL_GPIO_SetPinPull(GPIOB,LL_GPIO_PIN_4,LL_GPIO_PULL_DOWN);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(GPIOB,LL_GPIO_PIN_5,LL_GPIO_OUTPUT_PUSHPULL); // Pin B
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinPull(GPIOB,LL_GPIO_PIN_5,LL_GPIO_PULL_DOWN);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);

	/* Infinite loop */
	int channel=0;
	int temp_voltage = 0;
	
	while (1)
	{
		int adc_mux;
		int adc_lm35;
        bool check_mux = mux_read(&channel,LL_GPIO_PIN_3,LL_GPIO_PIN_5,LL_GPIO_PIN_4, &adc_mux);
		bool check_lm35 = lm35_read(&adc_lm35);

		/* Turn on internal LED if Voltage > 1.92V */
		if(adc_mux<2382 || adc_lm35 > 1861) // 2382/4095 x 3.3V = 1.92V (ADC Circuit + Mux), 1861/4095 x 3.3V = 1.5V (lm35)
			LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
	  	else
			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);

		/*Prepare data for USART2 Output (A0)*/
		float voltage_mux = adc_mux * (3.3/4095); // Calculate voltage from 12-bit ADC value
		voltage_mux = voltage_mux * 1000; // Get Millivolts
		int volt_integer=(int)voltage_mux; // Get whole number

		int original_voltage = voltage_mux / 0.8; // Original Voltage before the gain (about -0.8)

		if (check_mux)
			sprintf(buf,"CH%d_ADC=%d CH%d_Volt = %d CH%d_0.8x Volt=%dmV CH%d_Temp=%dC",channel-1,adc_mux,channel-1,original_voltage,channel-1,volt_integer,channel-1,temp_voltage/10);
		else
			sprintf(buf,"MUX - The voltage is too low");

		float voltage_lm35 = adc_mux * (3.3/4095);
		int temp_lm35 = voltage_lm35 * 100;

		if(check_lm35)
			sprintf(buf,"lm35_temp - %d",temp_lm35);
		else
			sprintf(buf,"LM35 - The voltage is too high");
		/*Get size of the buf*/
	  	int len=0;
		while(buf[len]!='\0')
		{
			len++;
		}

		/*Transmit each element*/
		for(int i=0;i<len;i++)
		{
			USART2_write(buf[i]);
		}

		/*Start new line*/
		USART2_write('\n');
		USART2_write('\r');


		LL_mDelay(1000); // Wait 1 second
  	
  	}
  return 0;
}

void adc_a0_config(void){
	/*Configure ADC*/
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_0); // Specify the sequence of channels for ADC1	(make channel 0 to be the first in sequence)
	LL_ADC_Enable(ADC1); // Enable ADC1
	LL_ADC_REG_StartConversionSWStart(ADC1); // Start conversion using Software Trigger
}

int read_adc_A0(void)
{
	/*Read ADC*/
	while(!LL_ADC_IsActiveFlag_ADRDY(ADC1)){} // Wait until ADC1 is ready (flag is set)
	int result = LL_ADC_REG_ReadConversionData12(ADC1); //Read ADC1 using 12 bits
	return result;
}

void adc_a1_config(void){
	/*Configure ADC*/
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_1); // Specify the sequence of channels for ADC1	(make channel 1 to be the first in sequence)
	LL_ADC_Enable(ADC1); // Enable ADC1
	LL_ADC_REG_StartConversionSWStart(ADC1); // Start conversion using Software Trigger
}

int read_adc_A1(void)
{
	/*Read ADC*/
	while(!LL_ADC_IsActiveFlag_ADRDY(ADC1)){} // Wait until ADC1 is ready (flag is set)
	int result = LL_ADC_REG_ReadConversionData12(ADC1); //Read ADC1 using 12 bits
	return result;
	
}

void adc_a2_config(void){
	/*Configure ADC*/
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_2); // Specify the sequence of channels for ADC1	(make channel 1 to be the first in sequence)
	LL_ADC_Enable(ADC1); // Enable ADC1
	LL_ADC_REG_StartConversionSWStart(ADC1); // Start conversion using Software Trigger
}

int read_adc_A2(void)
{
	/*Read ADC*/
	while(!LL_ADC_IsActiveFlag_ADRDY(ADC1)){} // Wait until ADC1 is ready (flag is set)
	int result = LL_ADC_REG_ReadConversionData12(ADC1); //Read ADC1 using 12 bits
	return result;
	
}


void USART2_Init(void)
{
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	//Enables USART2 clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	//Enables GPIOA port clock
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
	//Sets pinmode on Pin2 to alternate mode
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
	//Sets pinmode on Pin3 to alternate mode
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_2, LL_GPIO_AF_7);
	//Uses alternative function 7
	LL_GPIO_SetAFPin_0_7(GPIOA,LL_GPIO_PIN_2, LL_GPIO_AF_7);
	//Uses alternative function 7
    LL_USART_SetBaudRate(USART2, SystemCoreClock, LL_USART_OVERSAMPLING_16, 9600);
	//Sets Baud Rate : 	 USART2  SystemClock (32MHz) Oversampling  ;  baud  9600
    LL_USART_ConfigCharacter(USART2, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
	// Sets data stream : on USART2, Width of 8 bits, 		No parity, 			  1 stop bit
	LL_USART_EnableDirectionRx(USART2);
	//Enables RX
	LL_USART_EnableDirectionTx(USART2);
	//Enables TX
    LL_USART_Enable(USART2);
	//Enables USART2
}


void USART2_write(char data)
{	
	while(!(LL_USART_IsActiveFlag_TXE(USART2))){} // Wait for TXE flag to set
	LL_USART_TransmitData8(USART2, data); // Transmit 8 bit data
}

char USART2_read()
{	
	while(!(LL_USART_IsActiveFlag_RXNE(USART2))){} // Wait for RXNE flag to set
	return LL_USART_ReceiveData8(USART2); // Receive 8 bit data
}

int find_median(int arr[],int len){
	int apu;
		/*Sort array and get medium value*/
		for(int k=0;k<len;k++)
			{
				for(int i=0;i<len-1;i++) // Fix out of bounds issue
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



bool mux_read (int *chan, uint32_t A,uint32_t B,uint32_t C, int *data){ // Possibly need to change A and C  
// Modify a value thru pointer, return Bool
int read_times = 5;  // Specify the amount of reads
int adc_values[read_times];
if(*chan > 7) *chan = 0;
switch (*chan) // C B A
{
case 0: // 0 0 0
    LL_GPIO_ResetOutputPin(GPIOB, A); // Disable A
	LL_GPIO_ResetOutputPin(GPIOB, B); // Disable B
    LL_GPIO_ResetOutputPin(GPIOB, C); // Disable C
    break;
case 1: // 0 0 1
	LL_GPIO_SetOutputPin(GPIOB, A); // Enable A
    LL_GPIO_ResetOutputPin(GPIOB, B); // Disable B
    LL_GPIO_ResetOutputPin(GPIOB, C); // Disable C
    break;
case 2 : // 0 1 0
    LL_GPIO_ResetOutputPin(GPIOB, A);  // Disable A
    LL_GPIO_SetOutputPin(GPIOB, B); // Enable B 
    LL_GPIO_ResetOutputPin(GPIOB, C);  // Disable C
    break;
case 3 : // 0 1 1
	LL_GPIO_SetOutputPin(GPIOB, A);  // Enable A
	LL_GPIO_SetOutputPin(GPIOB, B); // Enable B 
    LL_GPIO_ResetOutputPin(GPIOB, C);  // Disable C
    break;
case 4 : // 1 0 0
    LL_GPIO_ResetOutputPin(GPIOB, A);  // Disable A
    LL_GPIO_ResetOutputPin(GPIOB, B);  // Disable B
	LL_GPIO_SetOutputPin(GPIOB, C); // Enable C
    break;
case 5 : // 1 0 1
    LL_GPIO_SetOutputPin(GPIOB, A); // Enable A 
    LL_GPIO_ResetOutputPin(GPIOB, B);  // Disable B
    LL_GPIO_SetOutputPin(GPIOB, C); // Enable C 
    break;
case 6 : // 1 1 0
    LL_GPIO_ResetOutputPin(GPIOB, A);  // Disable A
    LL_GPIO_SetOutputPin(GPIOB, B); // Enable B
    LL_GPIO_SetOutputPin(GPIOB, C); // Enable C 
    break;
case 7 : // 1 1 1
    LL_GPIO_SetOutputPin(GPIOB, A); // Enable A 
    LL_GPIO_SetOutputPin(GPIOB, B); // Enable B
    LL_GPIO_SetOutputPin(GPIOB, C); // Enable C 
    break;
default: // 0 0 0
    LL_GPIO_ResetOutputPin(GPIOB, A); // Disable A
	LL_GPIO_ResetOutputPin(GPIOB, B); // Disable B
    LL_GPIO_ResetOutputPin(GPIOB, C); // Disable C
    break;
}
	LL_mDelay(5); // We have to wait for a switch (about 1 ms measured with oscilloscope), otherwise we get garbage values
	/*Read channels read_times times*/
	adc_a0_config(); // Configure ADC only once when starting the readings
	for(int k=0;k<read_times-1;k++)
	{
			adc_values[k] = read_adc_A0(); // Read A0 and record its output to an array
			LL_mDelay(1); // 1 ms delay for each reading
	}
	*data = find_median(adc_values,read_times);

	if(*chan <= 7)
		(*chan)++;
	/*Example of implementing False*/
	if (*chan < 4 && *data < 2382) return false; // We check_mux if channels 0-3 have voltage over 1.92 Volts (2.4 Volts * 0.8 gain)

    return true;
}
bool lm35_read(int *data){
	int read_times = 5;
	int adc_values[read_times];
	for(int i = 0; i<read_times-1;i++){
		adc_a1_config();
		LL_mDelay(5);
		int positive_v = read_adc_A1();
		adc_a2_config();
		LL_mDelay(5);
		int negative_v = read_adc_A2();
		adc_values[i] = positive_v-negative_v;
		LL_mDelay(5);
	}
	*data = find_median(adc_values, read_times);
	if(*data > 1861) return false;

	return true;
}