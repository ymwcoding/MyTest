#include "can.h"
#include "can_receive.h"
#include "can_send.h"

/*------------------------------˵��------------------------------------*
���ļ�ʵ��STM32F1xx can�ĳ�ʼ��,ʵ��can���ͺͲ�ѯ���պ���
ͨ������can_data_receive_deal_handler��can_data_send_deal_handler���ⲿ�������
*-----------------------------------------------------------------------*/

#define can_data_receive_deal_handler   can_data_receive             //can�ж��жϴ�����
#define can_data_send_deal_handler      can_start_send             //can�����жϴ�����
	    

/*******************************************************************************************
 * ��������can_gpio_config
 * ����  ��CAN��GPIO ����,PB8�������룬PB9�������
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 *******************************************************************************************/
void can_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����ʱ������*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    /*IO����*/
    /* Configure CAN pin: RX */									 // PB8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* Configure CAN pin: TX */									 // PB9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);	           //�ض���CAN1��PB��
}


/*******************************************************************************************
 * ��������can_nvic_config
 * ����  ��CAN��NVIC ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 *******************************************************************************************/
void can_nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /*FIFO_0  �����ж�����*/
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	 //CAN1 RX0�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			     //�����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN1, CAN_IT_FMP0|CAN_IT_FF0|CAN_IT_FOV0, ENABLE);/*CANͨ���ж�ʹ��*/

    /*FIFO_1  �����ж�����*/
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;	         //CAN1 RX1�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			     //�����ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN1, CAN_IT_FMP1|CAN_IT_FF1|CAN_IT_FOV1, ENABLE);/*CANͨ���ж�ʹ��*/

    /*CAN1  �����ж�����*/
    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;	   //CAN1 TX0�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			     //�����ȼ�Ϊ2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN1,CAN_IT_TME, ENABLE); /*CANͨ�ŷ�������ж�ʹ��*/
}


/*******************************************************************************************
 * ��������can_mode_config
 * ����  ��CAN��ģʽ ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 *******************************************************************************************/
void can_mode_config(void)
{
    CAN_InitTypeDef        CAN_InitStructure;
    /**********CANͨ�Ų�������************/
    /*CAN�Ĵ�����ʼ��*/
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    /*CAN��Ԫ��ʼ��*/
    CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
    CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  �Զ����߹���
    CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  ʹ���Զ�����ģʽ
    CAN_InitStructure.CAN_NART=ENABLE;			   //MCR-NART  ENABLE-�����Զ��ش�	  DISABLE-�Զ��ش�
    CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���
    CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ��
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //��������ģʽ
//  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;  //�ػ�����ģʽ
    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		 //BTR-SJW ����ͬ����Ծ��� 1��ʱ�䵥Ԫ
    CAN_InitStructure.CAN_BS1=CAN_BS1_4tq;		 //BTR-TS1 ʱ���1 ռ����4��ʱ�䵥Ԫ
    CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		 //BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ
    CAN_InitStructure.CAN_Prescaler =6;		     //BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+4+3)/6=0.75Mbps

    CAN_Init(CAN1, &CAN_InitStructure);
}


/*******************************************************************************************
 * ��������can_filter_config
 * ����  ��CAN�Ĺ����� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 * ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0
 * ����˵��:��CAN_FilterId��ĳһλΪ1��ʱ���ʾ��λ��Ҫ����
 *            �����˵Ĺ������յ���ID�ڸ�λ�ϱ�����CAN_FilterMaskId��Ӧλ��ͬ���ܹ���ͨ��
 *            ������ʹ�ܵĹ���λ��ͨ����ʱ���ʾ����ͨ��
 *******************************************************************************************/
void can_filter_config(void)
{
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    /*CAN��������ʼ��*/
    CAN_FilterInitStructure.CAN_FilterNumber=0;		//��������0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //�����ڱ�ʶ������λģʽ
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//������λ��Ϊ����32λ��
    CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;				//Ҫ���˵�ID��λ
    CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000;        //Ҫ���˵�ID��λ
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;		//��������16λÿλ����ƥ��
    CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;		//��������16λÿλ����ƥ��
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;//��������������FIFO0��ͬʱʹ��FIF0_0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;		//ʹ�ܹ�����
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_FilterInitStructure.CAN_FilterNumber=1;		//��������1
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //�����ڱ�ʶ������λģʽ
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//������λ��Ϊ����32λ��
    CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;				//Ҫ���˵�ID��λ
    CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000;        //Ҫ���˵�ID��λ
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;		//��������16λÿλ����ƥ��
    CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;		//��������16λÿλ����ƥ��
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1 ;//��������������FIFO1��ͬʱʹ��FIF0_1
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;		//ʹ�ܹ�����
    CAN_FilterInit(&CAN_FilterInitStructure);
}



/*******************************************************************************************
 * ��������can_init
 * ����  ����ʼ��CAN�Ĺ���
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 *******************************************************************************************/
void can_init(void)
{
    can_gpio_config();
    can_mode_config();
    can_filter_config();
    can_nvic_config();//��������CAN��������ж�ʹ��
}


/*******************************************************************************************
**��������
*******************************************************************************************/
uint8_t can_send_msg(uint8_t* msg,uint8_t len,uint32_t id)
{
    uint8_t mbox;
    uint16_t i;
    CanTxMsg TxMessage;

    TxMessage.StdId =id & 0x000007FF;	//  ��׼��ʶ��Ϊ0
    TxMessage.ExtId =0;               // ������չ��ʾ����29λ��
    TxMessage.IDE   =CAN_Id_Standard; // ʹ�ñ�׼ʶ��
    TxMessage.RTR   =0;		            // ��Ϣ����Ϊ����֡��һ֡8λ
    TxMessage.DLC   =len;			        // ������Ϣ֡��֡��
    for(i=0; i<len; i++)  TxMessage.Data[i]  = msg[i];// ֡��Ϣ����
    for(; len<8; len++)   TxMessage.Data[len]= 0; // ֡��Ϣ����

    mbox=CAN_Transmit(CAN1, &TxMessage);

    if(mbox != CAN_TxStatus_NoMailBox)//����Ѿ�װ������
    {
        return(0);
    }
    else
    {
        return(1);
    }
}



/*
**�����жϷ�����
*/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0) !=0)//������治Ϊ��
	{
			CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);//can���߽������ݺ���
			can_data_receive_deal_handler(RxMessage);
	}
}

/*
**��������жϷ�����
*/
void USB_HP_CAN1_TX_IRQHandler(void)
{
   /***********************CAN���ݷ�����������ж�*******************/
	 if(CAN_GetITStatus(CAN1,CAN_IT_TME)!=RESET) 
	 {
		  CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	    can_data_send_deal_handler();
	 }

}









