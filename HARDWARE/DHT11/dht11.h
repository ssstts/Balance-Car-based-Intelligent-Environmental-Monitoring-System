#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "AD.h"

/*************************************************
											STM32
 * 文件			:	DHT11温度湿度传感器h文件                   

**********************BEGIN***********************/


/***************根据自己需求更改****************/
//DHT11引脚宏定义
#define DHT11_GPIO_PORT  GPIOB
#define DHT11_GPIO_PIN   GPIO_Pin_0
#define DHT11_GPIO_CLK   RCC_APB2Periph_GPIOB
/*********************END**********************/

//输出状态定义
#define OUT 1
#define IN  0

//控制DHT11引脚输出高低电平
#define DHT11_Low  GPIO_ResetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN)
#define DHT11_High GPIO_SetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN)

void DHT11_Init(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取温湿度数据
u8 DHT11_Read_Byte(void);//读取一个字节的数据
u8 DHT11_Read_Bit(void);//读取一位的数据
u8 DHT11_Check(void);//检测DHT11
void DHT11_Rst(void);//复位DHT11   
void DHT11_Mode(u8 mode);
#endif
