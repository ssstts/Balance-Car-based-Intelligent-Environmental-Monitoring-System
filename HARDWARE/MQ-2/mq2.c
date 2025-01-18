#include "mq2.h"

/*
											STM32
 * 文件			:	MQ-2烟雾传感器c文件                   
 * MCU			:	STM32F103C8T6
 * 接口			:	见代码							

*/


#if MODE
uint16_t MQ2_ADC_Read(void)
{
	return AD_Value[1];
}
#endif

uint16_t MQ2_GetData(void)
{
	
	#if MODE
	uint32_t  tempData = 0;
	uint8_t i;
	for (i = 0; i < MQ2_READ_TIMES; i++)
	{
		tempData += MQ2_ADC_Read();
		delay_ms(5);
	}

	tempData /= MQ2_READ_TIMES;
	return tempData;
	
	#else
	uint16_t tempData;
	tempData = !GPIO_ReadInputDataBit(MQ2_DO_GPIO_PORT, MQ2_DO_GPIO_PIN);
	return tempData;
	#endif
}


float MQ2_GetData_PPM(void)
{
	#if MODE
	float  tempData = 0;
	
	uint8_t i;
	for (i = 0; i < MQ2_READ_TIMES; i++)
	{
		tempData += MQ2_ADC_Read();
		delay_ms(5);
	}
	tempData /= MQ2_READ_TIMES;
	
	float Vol = (tempData*5/4096);
	float RS = (5-Vol)/(Vol*0.5);
	float R0=6.64;
	
	float ppm = pow(11.5428*R0/RS, 0.6549f);
	
	return ppm;
	#endif
}
