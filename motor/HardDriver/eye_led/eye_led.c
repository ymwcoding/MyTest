#include "eye_led.h"


/******************************************************************
 * 函数介绍:控制眼睛
 * 参数：无
 * 返回值：无
 * 备注:
 ******************************************************************/
void head_eye_ledout(uint16_t led0,uint16_t led1,uint16_t led2,uint16_t led3)
{
	uint8_t i;
	
	LED0_OE=1;
	LED1_OE=1;
	LED2_OE=1;
	LED3_OE=1;
	LED0_OE=0;
	LED1_OE=0;
	LED2_OE=0;
	LED3_OE=0;
	
	for(i=0;i<16;i++)
	{
		GPIOA->BRR |=0x0880;//LED_CLK=0;
		GPIOB->BRR |=0x4008;//LED_CLK=0;

		LED0_SDI=(led0>>i)&0x0001;
		LED1_SDI=(led1>>i)&0x0001;
    LED2_SDI=(led2>>i)&0x0001;
    LED3_SDI=(led3>>i)&0x0001;

		GPIOA->BSRR |= 0x0880;//LED_CLK=1;
		GPIOB->BSRR |= 0x4008;//LED_CLK=1;
	}

	GPIOA->BRR |=0x0440;//LED_LE=0;
	GPIOB->BRR |=0x8010;//LED_LE=0;
	
	GPIOA->BSRR |=0x0440;//LED_LE=1;
	GPIOB->BSRR |=0x8010;//LED_LE=1;
}


/******************************************************************
 * 函数介绍:数据进行转接
 * 参数：无
 * 返回值：无
 * 备注:
 ******************************************************************/
void head_eye_transpose(uint32_t led_out)
{
	uint16_t leddata[2]={0};	
	
	leddata[1]|=(uint16_t)((led_out>>6)&0x00000001)<<15;//0—R
	leddata[1]|=(uint16_t)((led_out>>7)&0x00000001)<<14;//0—G
	leddata[1]|=(uint16_t)((led_out>>8)&0x00000001)<<13;//0—B

	leddata[1]|=(uint16_t)((led_out>>3)&0x00000001)<<12;//1—R
	leddata[1]|=(uint16_t)((led_out>>4)&0x00000001)<<11;//1—G
	leddata[1]|=(uint16_t)((led_out>>5)&0x00000001)<<10;//1—B

	leddata[1]|=(uint16_t)((led_out>>0)&0x00000001)<<9;//2—R
	leddata[1]|=(uint16_t)((led_out>>1)&0x00000001)<<8;//2—G
	leddata[1]|=(uint16_t)((led_out>>2)&0x00000001)<<7;//2—B

	leddata[1]|=(uint16_t)((led_out>>21)&0x00000001)<<6;//3—R
	leddata[1]|=(uint16_t)((led_out>>22)&0x00000001)<<5;//3—G
	leddata[1]|=(uint16_t)((led_out>>23)&0x00000001)<<4;//3—B

	leddata[1]|=(uint16_t)((led_out>>18)&0x00000001)<<3;//4—R
	leddata[1]|=(uint16_t)((led_out>>19)&0x00000001)<<2;//4—G
	leddata[1]|=(uint16_t)((led_out>>20)&0x00000001)<<1;//4—B

	leddata[0]|=(uint16_t)((led_out>>15)&0x00000001)<<15;//5—R
	leddata[0]|=(uint16_t)((led_out>>16)&0x00000001)<<14;//5—G
	leddata[0]|=(uint16_t)((led_out>>17)&0x00000001)<<13;//5—B
	
	leddata[0]|=(uint16_t)((led_out>>9 )&0x00000001)<<12;//6—R
	leddata[0]|=(uint16_t)((led_out>>10)&0x00000001)<<11;//6—G
	leddata[0]|=(uint16_t)((led_out>>11)&0x00000001)<<10;//6—B
	
	leddata[0]|=(uint16_t)((led_out>>12)&0x00000001)<<9;//7—R
	leddata[0]|=(uint16_t)((led_out>>13)&0x00000001)<<8;//7—G
	leddata[0]|=(uint16_t)((led_out>>14)&0x00000001)<<7;//7—B	
	
	head_eye_ledout(leddata[0],leddata[1],leddata[0],leddata[1]);			
}



/******************************************************************
 * 函数介绍:初始化眼睛灯板控制
 * 参数：无
 * 返回值：无
 * 备注:
 ******************************************************************/
void head_eye_gpio_nit(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTB时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
	  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE); //关闭调试口
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|
		                              GPIO_Pin_6|
																	GPIO_Pin_7|
														  		GPIO_Pin_8|
																	GPIO_Pin_9|
																	GPIO_Pin_10|
																	GPIO_Pin_11|
																	GPIO_Pin_12|
																	GPIO_Pin_15;          //端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		//开漏输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);					 		//根据设定参数初始化
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|
		                              GPIO_Pin_2|
																	GPIO_Pin_3|
																	GPIO_Pin_4|
																	GPIO_Pin_5|
																	GPIO_Pin_14|
																	GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		//开漏输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					 		//根据设定参数初始化
		
    LED0_OE =1;
    LED0_LE =1;
    LED0_CLK=1;
    LED0_SDI=1;

    LED1_OE =1; 
    LED1_LE =1;
    LED1_CLK=1;
    LED1_SDI=1;
 
    LED2_OE =1;  
    LED2_LE =1;
    LED2_CLK=1;   
    LED2_SDI=1;

    LED3_OE =1;
    LED3_LE =1;
    LED3_CLK=1; 
    LED3_SDI=1;	
		
		LED0_OE=0;
		LED1_OE=0;
		LED2_OE=0;
		LED3_OE=0;
		
		LED0_LE=0;
		LED1_LE=0;
		LED2_LE=0;
		LED3_LE=0;
		
		head_eye_transpose(0x00000000);
}
