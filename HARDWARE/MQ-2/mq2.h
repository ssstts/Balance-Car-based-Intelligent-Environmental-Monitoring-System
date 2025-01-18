#ifndef __MQ2_H
#define	__MQ2_H
#include "stm32f10x.h"
#include "delay.h"
#include "math.h"

/*
											STM32
 * �ļ�			:	MQ-2��������h�ļ�                   
 * MCU			:	STM32F103C8T6
 * �ӿ�			:	������							

*/

#define MQ2_READ_TIMES	10  //MQ-2������ADCѭ����ȡ����

//ģʽѡ��	
//ģ��AO:	1
//����DO:	0
#define	MODE 	1

/***************�����Լ��������****************/
// MQ-2 GPIO�궨��
#if MODE
#define		MQ2_AO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	MQ2_AO_GPIO_PORT							GPIOA
#define 	MQ2_AO_GPIO_PIN								GPIO_Pin_7
#define   	MQ2_ADC_CHANNEL               				ADC_Channel_7	// ADC ͨ���궨��

#else
#define		MQ2_DO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	MQ2_DO_GPIO_PORT							GPIOA
#define 	MQ2_DO_GPIO_PIN								GPIO_Pin_1			

#endif
/*********************END**********************/


void MQ2_Init(void);
uint16_t MQ2_GetData(void);
float MQ2_GetData_PPM(void);

#endif /* __ADC_H */

