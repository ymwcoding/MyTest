#include "timer2.h"
#include "can_receive.h"


/*
**��ʼ���ȴ���ʱ��
*/
void timer2_init(uint16_t arr,uint16_t psc,uint8_t pre_prty,uint8_t sub_prty)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig( 		 	  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, 						 //TIM2
		TIM_IT_Update,     //TIM �ж�Դ  //TIM �����ж�Դ 
		ENABLE  					 //ʹ��
		);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  						       //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre_prty;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_prty;  			  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				          //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                                   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, DISABLE);  //ʹ��TIMx����	
}




/*
**������ʱ��2
*/
void timer2_start(void)
{
	TIM_SetCounter(TIM2,0);//��ն�ʱ��
	TIM_Cmd(TIM2, ENABLE); //ʹ��timer2
}

/*
**�رն�ʱ��2
*/
void timer2_stop(void)
{
	TIM_Cmd(TIM2, DISABLE);
}




//��ʱ��3�жϷ�����
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)  //����ж�
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
		can_decode_receive();                       //can��ʼ�������յ�������
	}
}


