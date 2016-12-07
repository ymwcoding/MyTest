#include "top_ear_led.h"



/******************************************************************
 * 函数介绍:设置某个灯的开关
 * 参数：led_naber：灯的编号
         led_rgb  ：灯的颜色
         mode：开关模式，0为关，1为开
 * 返回值：无
 * 备注:
 ******************************************************************/
void top_ear_led_on_off(uint8_t led_naber,uint8_t led_rgb,uint8_t mode) 
{
	switch(led_naber)
	{
		case 0:{switch(led_rgb){case 0:{LED1_R=mode;}break;case 1:{LED1_G=mode;}break;case 2:{LED1_B=mode;}break;}}break;
		case 1:{switch(led_rgb){case 0:{LED2_R=mode;}break;case 1:{LED2_G=mode;}break;case 2:{LED2_B=mode;}break;}}break;
	}
}


/*
**top_led初始化
*/
void top_ear_led_gpio_init(void)
{
	uint8_t i,j;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能端口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能端口时钟
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//端口配置 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 	//根据设定参数初始化	
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;        //端口配置 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 	//根据设定参数初始化	
	
	for(i=0;i<3;i++)//关闭所有的灯
	{
		for(j=0;j<2;j++)
		{
			top_ear_led_on_off(j,i,0);
		}
	}
	
	
}






