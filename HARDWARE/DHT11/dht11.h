#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "AD.h"

/*************************************************
											STM32
 * �ļ�			:	DHT11�¶�ʪ�ȴ�����h�ļ�                   

**********************BEGIN***********************/


/***************�����Լ��������****************/
//DHT11���ź궨��
#define DHT11_GPIO_PORT  GPIOB
#define DHT11_GPIO_PIN   GPIO_Pin_0
#define DHT11_GPIO_CLK   RCC_APB2Periph_GPIOB
/*********************END**********************/

//���״̬����
#define OUT 1
#define IN  0

//����DHT11��������ߵ͵�ƽ
#define DHT11_Low  GPIO_ResetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN)
#define DHT11_High GPIO_SetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN)

void DHT11_Init(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ������
u8 DHT11_Read_Byte(void);//��ȡһ���ֽڵ�����
u8 DHT11_Read_Bit(void);//��ȡһλ������
u8 DHT11_Check(void);//���DHT11
void DHT11_Rst(void);//��λDHT11   
void DHT11_Mode(u8 mode);
#endif
