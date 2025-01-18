#include "stm32f10x.h"                  // Device header
#include "tim3.h"

void TIM3_Int_Init()
{
		GPIO_InitTypeDef GPIO_InitStruct;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			
		
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//设置为推挽输出
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//设置为输入
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
		GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		//定时器3初始化
		TIM_TimeBaseStructure.TIM_Period = 999; //ARR
		TIM_TimeBaseStructure.TIM_Prescaler =7199; //PSC
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );//使能制定TIM3中断，允许更新中断
		//中断设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		TIM_Cmd(TIM3, DISABLE);
}
