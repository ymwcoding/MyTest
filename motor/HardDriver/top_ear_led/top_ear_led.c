#include "top_ear_led.h"



/******************************************************************
 * ��������:����ĳ���ƵĿ���
 * ������led_naber���Ƶı��
         led_rgb  ���Ƶ���ɫ
         mode������ģʽ��0Ϊ�أ�1Ϊ��
 * ����ֵ����
 * ��ע:
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
**top_led��ʼ��
*/
void top_ear_led_gpio_init(void)
{
	uint8_t i,j;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ�ܶ˿�ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ�ܶ˿�ʱ��
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//�˿����� 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 	//�����趨������ʼ��	
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;        //�˿����� 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 	//�����趨������ʼ��	
	
	for(i=0;i<3;i++)//�ر����еĵ�
	{
		for(j=0;j<2;j++)
		{
			top_ear_led_on_off(j,i,0);
		}
	}
	
	
}






