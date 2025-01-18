#ifndef __EXTI_H
#define	__EXTI_H


#include "stm32f10x.h"


// ���Ŷ���
#define KEY1_INT_GPIO_PORT         GPIOC
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC) // ֻ����GPIOCʱ�ӣ�����ҪAFIO
#define KEY1_INT_GPIO_PIN          GPIO_Pin_13
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource13
#define KEY1_INT_EXTI_LINE         EXTI_Line13 // PC13��Ӧ����EXTI_Line13
#define KEY1_INT_EXTI_IRQ          EXTI15_10_IRQn // PC13��Ӧ���ж���EXTI15_10_IRQn

#define KEY1_IRQHandler            EXTI15_10_IRQHandler // �޸�Ϊ��Ӧ���жϴ�����

void EXTI_Key_Config(void);


#endif /* __EXTI_H */
