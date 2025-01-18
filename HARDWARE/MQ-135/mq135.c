#include "mq135.h"
/*
											STM32
 * �ļ�			:	MQ-135��������������c�ļ�                   
 * MCU			:	STM32F103C8T6
 * �ӿ�			:	������							

*/


#if MODE
uint16_t MQ135_ADC_Read(void)
{
	//����ָ��ADC�Ĺ�����ͨ��������ʱ��
	return AD_Value[2];
}
#endif

uint16_t MQ135_GetData(void)
{
	
	#if MODE
	uint32_t  tempData = 0;
	for (uint8_t i = 0; i < MQ135_READ_TIMES; i++)
	{
		tempData += MQ135_ADC_Read();
		delay_ms(5);
	}

	tempData /= MQ135_READ_TIMES;
	return tempData;
	
	#else
	uint16_t tempData;
	tempData = !GPIO_ReadInputDataBit(MQ135_DO_GPIO_PORT, MQ135_DO_GPIO_PIN);
	return tempData;
	#endif
}


float MQ135_GetData_PPM(void)
{
	#if MODE
	float  tempData = 0;
	

	for (uint8_t i = 0; i < MQ135_READ_TIMES; i++)
	{
		tempData += MQ135_ADC_Read();
		delay_ms(5);
	}
	tempData /= MQ135_READ_TIMES;
	
	float Vol = (tempData*5/4096);
	float RS = (5-Vol)/(Vol*0.5);
	float R0=6.64;
	
	float ppm = pow(11.5428*R0/RS, 0.6549f);
	
	return ppm;
	#endif
}
