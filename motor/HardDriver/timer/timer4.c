#include "timer4.h"
#include "touch_key.h"

/*
**��ʱ��4��ʼ��
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

	TIM_TimeBaseStructure.TIM_Period=arr;//װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler= psc;//ʱ��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	
	TIM_Cmd(TIM4, DISABLE);  //ʹ��TIMx����														
}



/*
**������ʱ��4
*/
void timer4_start(void)
{
	TIM_SetCounter(TIM4,0);//��ն�ʱ��
	TIM_Cmd(TIM4, ENABLE); //ʹ��timer4
}

/*
**�رն�ʱ��4
*/
void timer4_stop(void)
{
	TIM_Cmd(TIM4, DISABLE);
}



/*
**��ʱ��4�жϴ���
*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET ) 
	{
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);//����жϱ�־λ
		touch_key_timer_task();
	}
}

