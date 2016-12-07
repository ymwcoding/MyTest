#include "timer3.h"
#include "eye_led_control.h"
#include "top_ear_led_control.h"

/*
**定时器3初始化
*/
void timer3_init(uint16_t arr,uint16_t psc,uint8_t pre_prty,uint8_t sub_prty)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	 													
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre_prty;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_prty;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructure.TIM_Period=arr;//装载值
	TIM_TimeBaseStructure.TIM_Prescaler= psc;//时钟预分频
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);																		
}


/*
**定时器3中断处理
*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET ) 
	{
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);//清除中断标志位
		top_ear_led_timer_task();
		eye_led_timer_task();
	}
}


