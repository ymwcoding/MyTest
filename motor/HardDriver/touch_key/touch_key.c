#include "touch_key.h"
#include "my_types.h"
#include "timer4.h"

/*
**��ȡ����״̬
*/
void get_touch_key_state(uint8_t* get_data)
{
	*(get_data) = FlagBit.flag.keyState;
	*(get_data+1) = FlagBit.flag.keyTouchStateFlag;
	
	FlagBit.flag.keyTouchStateFlag = 0;         //ͷ������״̬���
}





/*
**����io��ʼ��
*/
static void touch_key_io_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ�ܶ˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;            //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	     //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 	   //�����趨������ʼ��		
}



/*
**�����жϳ�ʼ��
*/
static void touch_key_interrupt_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);			    	  //ʹ��APB2ʱ���ߵĸ���ʱ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);      //���Ӱ������ŵ��ж���
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;                     //ʹ��key
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;             //������ʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;			   //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	
	/**----------------------NVIC����-----------------------**/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;   						//�����ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	 //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/*
**������ʼ��
*/
void touch_key_init(void)
{
	touch_key_io_init();
	touch_key_interrupt_init();
}




/*
**�����жϴ���
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
**������ʱ��4�жϴ���
*/
void touch_key_timer_task(void)
{
	FlagBit.flag.keyState = READ_KEY();
	if(FlagBit.flag.keyState == 0)//�������ǰ��µ�
	{
		FlagBit.flag.keyTouchStateFlag = 1;
	}
	else                          //�����ſ�������
	{
		  FlagBit.flag.keyTouchStateFlag = 0;
			FlagBit.flag.keyTimerStartFlag = 0;
			timer4_stop();
	}
	
}



/******************************************
**key0�жϷ�����
*******************************************/
void EXTI4_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line4);//����жϱ�־
	touch_key_interrupt_task();
} 


