#include "usart3.h"
#include "xf_voice.h"
/*
**串口3初始化
*/
void usart3_init(uint32_t bound,uint8_t pre_prty,uint8_t sub_prty)
{

  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	  //GPIOB 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);   //USART3时钟

	USART_DeInit(USART3);  //复位串口3

	//USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //管脚10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);                //TX初始化

	//USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //管脚11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);   

	//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre_prty ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_prty;		

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART3


	//USART3初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART3, ENABLE);                    //使能串口 

}

/*
**发送
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
**串口1中断处理
*/
void USART3_IRQHandler(void)        
{
	 uint8_t res = 0;
	 /*-------------------接收错误处理--------------------------*/
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
		
		res =USART_ReceiveData(USART3);	//读取接收到的数据
		xf_usart_receive_deal(res);
	} 
} 
