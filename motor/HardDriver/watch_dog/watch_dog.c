#include "watch_dog.h"
#include "gpio_bit.h"


/*********************************************************************
//初始化看门狗
*********************************************************************/
void watchdog_init(void)
{
// GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能端口时钟
//	
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 	//端口配置
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
// GPIO_Init(GPIOB, &GPIO_InitStructure);					 		//根据设定参数初始化
// watchdog_enable();//使能看门狗
// PBout(12)=0;
}
 


/*********************************************************************
//喂狗
*********************************************************************/
void watchdog_feed(void)
{
//	PBout(12)=~PBout(12);
}


/*********************************************************************
//关闭喂狗
*********************************************************************/
void watchdog_disable(void)
{
	
}	

/*********************************************************************
//使能喂狗
*********************************************************************/
void watchdog_enable(void)
{
	
}	
