#include "timer4.h"
#include "touch_key.h"

/*
**定时器4初始化
*/
void timer4_init(uint16_t arr,uint16_t psc,uint8_t pre_prty,uint8_t sub_prty)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	 													
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre_prty;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_prty;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructure.TIM_Period=arr;//装载值
	TIM_TimeBaseStructure.TIM_Prescaler= psc;//时钟预分频
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	
	TIM_Cmd(TIM4, DISABLE);  //使能TIMx外设														
}



/*
**开启定时器4
*/
void timer4_start(void)
{
	TIM_SetCounter(TIM4,0);//清空定时器
	TIM_Cmd(TIM4, ENABLE); //使能timer4
}

/*
**关闭定时器4
*/
void timer4_stop(void)
{
	TIM_Cmd(TIM4, DISABLE);
}



/*
**定时器4中断处理
*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET ) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);//清除中断标志位
		touch_key_timer_task();
	}
}

