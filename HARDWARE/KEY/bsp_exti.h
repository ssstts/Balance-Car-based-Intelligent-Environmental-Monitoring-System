#ifndef __EXTI_H
#define	__EXTI_H


#include "stm32f10x.h"


// 引脚定义
#define KEY1_INT_GPIO_PORT         GPIOC
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC) // 只启用GPIOC时钟，不需要AFIO
#define KEY1_INT_GPIO_PIN          GPIO_Pin_13
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource13
#define KEY1_INT_EXTI_LINE         EXTI_Line13 // PC13对应的是EXTI_Line13
#define KEY1_INT_EXTI_IRQ          EXTI15_10_IRQn // PC13对应的中断是EXTI15_10_IRQn

#define KEY1_IRQHandler            EXTI15_10_IRQHandler // 修改为对应的中断处理函数

void EXTI_Key_Config(void);


#endif /* __EXTI_H */
