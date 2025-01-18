#ifndef __MQ135_H
#define	__MQ135_H
#include "stm32f10x.h"
#include "AD.h"
#include "delay.h"
#include "math.h"

/*
											STM32
 * �ļ�			:	MQ-135��������������h�ļ�                   
 * MCU			:	STM32F103C8T6
 * �ӿ�			:	������							

*/

#define MQ135_READ_TIMES	10  //MQ-135������ADCѭ����ȡ����

//ģʽѡ��	
//ģ��AO:	1
//����DO:	0
#define	MODE 	1

/***************�����Լ��������****************/
// MQ-135 GPIO�궨��
#if MODE
#define		MQ135_AO_GPIO_CLK								RCC_APB2Periph_GPIOB
#define 	MQ135_AO_GPIO_PORT							GPIOB
#define 	MQ135_AO_GPIO_PIN								GPIO_Pin_1
#define   	MQ135_ADC_CHANNEL               			ADC_Channel_9	// ADC ͨ���궨��

#else
#define		MQ135_DO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	MQ135_DO_GPIO_PORT							GPIOA
#define 	MQ135_DO_GPIO_PIN								GPIO_Pin_1			

#endif
/*********************END**********************/


uint16_t MQ135_GetData(void);
float MQ135_GetData_PPM(void);

#endif /* __ADC_H */

