#include "watch_dog.h"
#include "gpio_bit.h"


/*********************************************************************
//��ʼ�����Ź�
*********************************************************************/
void watchdog_init(void)
{
// GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ�ܶ˿�ʱ��
//	
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 	//�˿�����
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
// GPIO_Init(GPIOB, &GPIO_InitStructure);					 		//�����趨������ʼ��
// watchdog_enable();//ʹ�ܿ��Ź�
// PBout(12)=0;
}
 


/*********************************************************************
//ι��
*********************************************************************/
void watchdog_feed(void)
{
//	PBout(12)=~PBout(12);
}


/*********************************************************************
//�ر�ι��
*********************************************************************/
void watchdog_disable(void)
{
	
}	

/*********************************************************************
//ʹ��ι��
*********************************************************************/
void watchdog_enable(void)
{
	
}	
