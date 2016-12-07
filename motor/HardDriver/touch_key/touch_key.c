#include "touch_key.h"
#include "my_types.h"
#include "timer4.h"

/*
**获取按键状态
*/
void get_touch_key_state(uint8_t* get_data)
{
	*(get_data) = FlagBit.flag.keyState;
	*(get_data+1) = FlagBit.flag.keyTouchStateFlag;
	
	FlagBit.flag.keyTouchStateFlag = 0;         //头部触摸状态清除
}





/*
**按键io初始化
*/
static void touch_key_io_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;            //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	     //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 	   //根据设定参数初始化		
}



/*
**按键中断初始化
*/
static void touch_key_interrupt_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);			    	  //使能APB2时钟线的复用时钟
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);      //连接按键引脚的中断线
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;                     //使能key
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;             //触发方式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;			   //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	
	/**----------------------NVIC配置-----------------------**/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;   						//按键中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	 //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/*
**按键初始化
*/
void touch_key_init(void)
{
	touch_key_io_init();
	touch_key_interrupt_init();
}




/*
**按键中断处理
*/
static void touch_key_interrupt_task(void)
{
	if(FlagBit.flag.keyTimerStartFlag == 0)
	{
		FlagBit.flag.keyTimerStartFlag = 1;
		timer4_start();
	}
}



/*
**按键定时器4中断处理
*/
void touch_key_timer_task(void)
{
	FlagBit.flag.keyState = READ_KEY();
	if(FlagBit.flag.keyState == 0)//按键还是按下的
	{
		FlagBit.flag.keyTouchStateFlag = 1;
	}
	else                          //按键放开就清零
	{
		  FlagBit.flag.keyTouchStateFlag = 0;
			FlagBit.flag.keyTimerStartFlag = 0;
			timer4_stop();
	}
	
}



/******************************************
**key0中断服务函数
*******************************************/
void EXTI4_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line4);//清除中断标志
	touch_key_interrupt_task();
} 


