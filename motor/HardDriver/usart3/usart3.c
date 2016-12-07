#include "usart3.h"
#include "xf_voice.h"
/*
**����3��ʼ��
*/
void usart3_init(uint32_t bound,uint8_t pre_prty,uint8_t sub_prty)
{

  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	  //GPIOB ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);   //USART3ʱ��

	USART_DeInit(USART3);  //��λ����3

	//USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //�ܽ�10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                //TX��ʼ��

	//USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //�ܽ�11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);   

	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre_prty ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_prty;		

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART3


	//USART3��ʼ������
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}

/*
**����
*/
void usart3_send_buf(uint8_t *buf,uint16_t len)
{  
	uint16_t i;
	for (i=0;i<len;i++)
	{ 
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
		USART_SendData(USART3,buf[i]);	
	}
}



/*
**����1�жϴ���
*/
void USART3_IRQHandler(void)        
{
	 uint8_t res = 0;
	 /*-------------------���մ�����--------------------------*/
   if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)  
	 {  
		 USART_ReceiveData(USART3);                   
		 USART_ClearFlag(USART3, USART_FLAG_PE);  
	 }
	 
	 if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  
   {  
     USART_ReceiveData(USART3);  
     USART_ClearFlag(USART3, USART_FLAG_ORE);  
   }  
	 
	 if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)  
   {  
     USART_ReceiveData(USART3);  
     USART_ClearFlag(USART3, USART_FLAG_FE);  
   }  
/*--------------------------------------------------------------*/
	
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART3, USART_FLAG_RXNE);  
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); 
		
		res =USART_ReceiveData(USART3);	//��ȡ���յ�������
		xf_usart_receive_deal(res);
	} 
} 
