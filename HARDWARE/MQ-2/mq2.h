#ifndef __MQ2_H
#define	__MQ2_H
#include "stm32f10x.h"
#include "delay.h"
#include "math.h"

/*
											STM32
 * 文件			:	MQ-2烟雾传感器h文件                   
 * MCU			:	STM32F103C8T6
 * 接口			:	见代码							

*/

#define MQ2_READ_TIMES	10  //MQ-2传感器ADC循环读取次数

//模式选择	
//模拟AO:	1
//数字DO:	0
#define	MODE 	1

/***************根据自己需求更改****************/
// MQ-2 GPIO宏定义
#if MODE
#define		MQ2_AO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	MQ2_AO_GPIO_PORT							GPIOA
#define 	MQ2_AO_GPIO_PIN								GPIO_Pin_7
#define   	MQ2_ADC_CHANNEL               				ADC_Channel_7	// ADC 通道宏定义

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

