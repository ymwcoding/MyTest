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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	      	    //��ʱ������ʼ��	 
	delay_ms(3000);           //������Ҫʹ��PA15���Խ�������Ҫ��ʱ3������
	usart3_init(115200,2,2);    //����3��ʼ��Ϊ115200,�ƴ�Ѷ��ģ��
	can_init();               //CANͨ�ų�ʼ��
	can_receive_ring_buffer_create();
	head_eye_gpio_nit();      //��ʼ���۾���
	top_ear_led_gpio_init();  //�����ͷ���Ƴ�ʼ��
	touch_key_init();         //����������ʼ��
	timer3_init(20000,0,3,3); //led�ƶ�ʱ����ʼ�� 100us
	timer2_init(49,7199,2,0);     //can���붨ʱ�� 5ms
	timer4_init(99,7199,2,1);     //������ʱ����ʼ�� 10ms
	watchdog_init();          //��ʼ�����Ź�
	xf_voice_set_angle(3);
}

