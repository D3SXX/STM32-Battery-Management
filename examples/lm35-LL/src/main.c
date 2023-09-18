/*Author : Sergiu Petrut, Date : 18.9.2023 
Conclusion : ALL LL functions (tested so far) work.
Might need to check more necessary LL functions.

Sidenote : If you don't want to pull your hair out 
like I did looking into everything, check the table
on page 1221 in the "HAL and LL" PDF. Saves a lot of time.

This code here showcases : 
Reading ADC values with LL
Using USART with LL 
Using GPIO with LL



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
/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */
void USART2_Init(void);
void USART2_write(char data);
char USART2_read(void);
//void delay_Ms(int delay);
int read_adc_A0(void);
int read_adc_A1(void);
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
  SetSysClock();
  SystemCoreClockUpdate();
  LL_SYSTICK_SetClkSource(SystemCoreClock);
  USART2_Init();
  /* TODO - Add your application code here */

  //set up pin PA5 for LED
  //LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  //LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
  //GPIOA->MODER&=~0x00000C00;	//clear pin mode

  //set up pin PA0 and PA1 for analog input
  //RCC->AHBENR|=1;				//enable GPIOA clock

  //WORKS
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);

  //GPIOA->MODER|=0x3;			//PA0 analog (A0)
  //GPIOA->MODER|=0xC;			//PA1 analog (A1)



  //setup ADC1. p272
	//WORKS
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_384CYCLES);
	LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
	LL_ADC_INJ_SetTriggerSource(ADC1,LL_ADC_INJ_TRIG_SOFTWARE);
  /*
  RCC->APB2ENR|=0x00000200;		//enable ADC1 clock
  ADC1->CR2=0;					//bit 1=0: Single conversion mode
  ADC1->SMPR3=7;				//384 cycles sampling time for channel 0 (longest)
  ADC1->CR1&=~0x03000000;		//resolution 12-bit
*/

  /*RCC->AHBENR|=1; //GPIOA ABH bus clock ON. p154
  GPIOA->MODER|=0x400; //GPIOA pin 5 to output. p184
*/
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);

  int adc_A0=0;
  float adc_value=0;
  int temp=0;
  int temp_degree=0;
  int temp_decimals=0;
  char buf[]="";
  int raw_value[5]={0};
  int i=0;
  int k=0;
  int apu=0;
  /* Infinite loop */
  while (1)
  {


	  for(k=0;k<5;k++)
	  {
	  raw_value[k]=read_adc_A0();
	  delay_Ms(50);
	  //LL_mDelay(50);
	  }

	  for(k=0;k<5;k++)
		{
			for(i=0;i<5;i++)
			{
				if(raw_value[i]>raw_value[i+1])
				{
					apu=raw_value[i];
					raw_value[i]=raw_value[i+1];
					raw_value[i+1]=apu;
				}
				else
				{

				}
			}
		}
	  adc_A0=raw_value[2];

	  if(adc_A0>1241) //1241/4095 x 3.3V = 1V
		  GPIOA->ODR|=0x20; //0010 0000 set bit 5. p186
	  else
		  GPIOA->ODR&=~0x20; //0000 0000 clear bit 5. p186


	  adc_value=((100.0/4095.0)*(float)adc_A0-50.0); //calculate temperature
	  adc_value=roundf(adc_value*100)/100;
	  temp=adc_value*100; //remove decimals and 34.54 = 3454
      temp_degree=(int)temp/100;
      temp_decimals=abs((int)temp%100);
	  sprintf(buf,"%d.%d Celcius",temp_degree,temp_decimals);

	  	int len=0;
		while(buf[len]!='\0')
		len++;

		for(int i=0;i<len;i++)
		{
			USART2_write(buf[i]);
		}

		USART2_write('\n');
		USART2_write('\r');

	  delay_Ms(1000);
	  read_adc_A1();
	  delay_Ms(1000);

  }
  return 0;
}

int read_adc_A0(void)
{
	int result=0;
	/*ADC1->SQR5=0;				//conversion sequence starts at ch0
	ADC1->CR2|=1;				//bit 0, ADC on/off (1=on, 0=off)
	ADC1->CR2|=0x40000000;		//start conversion
	*/
	//WORKS
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_0);	
	LL_ADC_Enable(ADC1);
	LL_ADC_REG_StartConversionSWStart(ADC1);	

	
	/*while(!(ADC1->SR & 2)){}	//wait for conversion complete
	result=ADC1->DR;			//read conversion result
	*/
	//WORKS
	while(!LL_ADC_IsActiveFlag_ADRDY(ADC1)){}
	result = LL_ADC_REG_ReadConversionData12(ADC1);
	return result;
}

int read_adc_A1(void)
{
	char buf[100];
	int result=0;
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_0); //Conversion starts on CH 0	
	LL_ADC_Enable(ADC1); //Enable ADC
	LL_ADC_REG_StartConversionSWStart(ADC1);		//start conversion
	while(!LL_ADC_IsActiveFlag_ADRDY(ADC1)){}
	result = LL_ADC_REG_ReadConversionData12(ADC1);
    float voltage = result * (3300/1023.0);
    int big_v = (int)voltage;
    int small_v = (voltage-big_v)*100;
    float temp = voltage/10;
    int big_temp = (int)temp;
    int small_temp = (temp-big_temp)*100;
	sprintf(buf,"A1=%d voltage=%d.%d temp=%d.%d",result,big_v,(int)small_v, big_temp,small_temp);
    
	int len=0;
	while(buf[len]!='\0')
	len++;

	for(int i=0;i<len;i++)
	{
		USART2_write(buf[i]);
	}
	USART2_write('\n');
	USART2_write('\r');
	LL_ADC_Disable(ADC1);
	//ADC1->CR2&=~1;				//bit 0, ADC on/off (1=on, 0=off)

	return result;
}

void USART2_Init(void) //WORKS
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
{	/*
	//wait while TX buffer is empty
	while(!(USART2->SR&0x0080)){} 	//TXE: Transmit data register empty. p736-737
		USART2->DR=(data);	*/	//p739
	while(!(LL_USART_IsActiveFlag_TXE(USART2))){}
	LL_USART_TransmitData8(USART2, data);

}

char USART2_read()
{	

	
	/*char data=0;
	//wait while RX buffer data is ready to be read
	while(!(USART2->SR&0x0020)){} //Bit 5 RXNE: Read data register not empty
		data=USART2->DR;			//p739
		return data;
	*/	
	while(!(LL_USART_IsActiveFlag_RXNE(USART2))){}
	return LL_USART_ReceiveData8(USART2);
}

void delay_Ms(int delay)
{
	int i=0;
	for(; delay>0;delay--)
		for(i=0;i<2460;i++); //measured with oscilloscope
}