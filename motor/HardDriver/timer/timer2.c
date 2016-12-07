#include "timer2.h"
#include "can_receive.h"


/*
**初始化等待定时器
*/
void timer2_init(uint16_t arr,uint16_t psc,uint8_t pre_prty,uint8_t sub_prty)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig( 		 	  //使能或者失能指定的TIM中断
		TIM2, 						 //TIM2
		TIM_IT_Update,     //TIM 中断源  //TIM 触发中断源 
		ENABLE  					 //使能
		);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  						       //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre_prty;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_prty;  			  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				          //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                                   //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, DISABLE);  //使能TIMx外设	
}




/*
**开启定时器2
*/
void timer2_start(void)
{
	TIM_SetCounter(TIM2,0);//清空定时器
	TIM_Cmd(TIM2, ENABLE); //使能timer2
}

/*
**关闭定时器2
*/
void timer2_stop(void)
{
	TIM_Cmd(TIM2, DISABLE);
}




//定时器3中断服务函数
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)  //溢出中断
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
		can_decode_receive();                       //can开始解析接收到的数据
	}
}


