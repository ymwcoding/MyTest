#include "app_init.h"
#include "touch_key.h"
#include "can.h"
#include "timer2.h"
#include "timer3.h"
#include "timer4.h"
#include "top_ear_led.h"
#include "eye_led.h"
#include "delay.h"
#include "usart3.h"
#include "watch_dog.h"
#include "can_receive.h"
#include "xf_voice.h"


void app_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();	      	    //延时函数初始化	 
	delay_ms(3000);           //由于需要使用PA15调试脚所以需要延时3秒启动
	usart3_init(115200,2,2);    //串口3初始化为115200,科大讯飞模块
	can_init();               //CAN通信初始化
	can_receive_ring_buffer_create();
	head_eye_gpio_nit();      //初始化眼睛灯
	top_ear_led_gpio_init();  //耳朵和头顶灯初始化
	touch_key_init();         //触摸按键初始化
	timer3_init(20000,0,3,3); //led灯定时器初始化 100us
	timer2_init(49,7199,2,0);     //can解码定时器 5ms
	timer4_init(99,7199,2,1);     //按键定时器初始化 10ms
	watchdog_init();          //初始化看门狗
	xf_voice_set_angle(3);
}

