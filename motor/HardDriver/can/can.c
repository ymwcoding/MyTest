#include "can.h"
#include "can_receive.h"
#include "can_send.h"

/*------------------------------说明------------------------------------*
本文件实现STM32F1xx can的初始化,实现can发送和查询接收函数
通过配置can_data_receive_deal_handler和can_data_send_deal_handler和外部函数耦合
*-----------------------------------------------------------------------*/

#define can_data_receive_deal_handler   can_data_receive             //can中断中断处理函数
#define can_data_send_deal_handler      can_start_send             //can发送中断处理函数
	    

/*******************************************************************************************
 * 函数名：can_gpio_config
 * 描述  ：CAN的GPIO 配置,PB8上拉输入，PB9推挽输出
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 *******************************************************************************************/
void can_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*外设时钟设置*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    /*IO设置*/
    /* Configure CAN pin: RX */									 // PB8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* Configure CAN pin: TX */									 // PB9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);	           //重定义CAN1到PB口
}


/*******************************************************************************************
 * 函数名：can_nvic_config
 * 描述  ：CAN的NVIC 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 *******************************************************************************************/
void can_nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /*FIFO_0  接收中断设置*/
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	 //CAN1 RX0中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			     //子优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN1, CAN_IT_FMP0|CAN_IT_FF0|CAN_IT_FOV0, ENABLE);/*CAN通信中断使能*/

    /*FIFO_1  接收中断设置*/
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;	         //CAN1 RX1中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			     //子优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN1, CAN_IT_FMP1|CAN_IT_FF1|CAN_IT_FOV1, ENABLE);/*CAN通信中断使能*/

    /*CAN1  发送中断设置*/
    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;	   //CAN1 TX0中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			     //子优先级为2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN1,CAN_IT_TME, ENABLE); /*CAN通信发送完成中断使能*/
}


/*******************************************************************************************
 * 函数名：can_mode_config
 * 描述  ：CAN的模式 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 *******************************************************************************************/
void can_mode_config(void)
{
    CAN_InitTypeDef        CAN_InitStructure;
    /**********CAN通信参数设置************/
    /*CAN寄存器初始化*/
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    /*CAN单元初始化*/
    CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  关闭时间触发通信模式使能
    CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  自动离线管理
    CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  使用自动唤醒模式
    CAN_InitStructure.CAN_NART=ENABLE;			   //MCR-NART  ENABLE-报文自动重传	  DISABLE-自动重传
    CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文
    CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //正常工作模式
//  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;  //回环测试模式
    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		 //BTR-SJW 重新同步跳跃宽度 1个时间单元
    CAN_InitStructure.CAN_BS1=CAN_BS1_4tq;		 //BTR-TS1 时间段1 占用了4个时间单元
    CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		 //BTR-TS1 时间段2 占用了3个时间单元
    CAN_InitStructure.CAN_Prescaler =6;		     //BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+4+3)/6=0.75Mbps

    CAN_Init(CAN1, &CAN_InitStructure);
}


/*******************************************************************************************
 * 函数名：can_filter_config
 * 描述  ：CAN的过滤器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 * 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0
 * 过滤说明:当CAN_FilterId的某一位为1的时候表示该位需要过滤
 *            而过滤的规则是收到的ID在该位上必须与CAN_FilterMaskId对应位相同才能过滤通过
 *            当所有使能的过滤位都通过的时候表示过滤通过
 *******************************************************************************************/
void can_filter_config(void)
{
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    /*CAN过滤器初始化*/
    CAN_FilterInitStructure.CAN_FilterNumber=0;		//过滤器组0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //工作在标识符屏蔽位模式
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//过滤器位宽为单个32位。
    CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;				//要过滤的ID高位
    CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000;        //要过滤的ID低位
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;		//过滤器高16位每位必须匹配
    CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;		//过滤器低16位每位必须匹配
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;//过滤器被关联到FIFO0，同时使能FIF0_0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;		//使能过滤器
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_FilterInitStructure.CAN_FilterNumber=1;		//过滤器组1
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //工作在标识符屏蔽位模式
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//过滤器位宽为单个32位。
    CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;				//要过滤的ID高位
    CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000;        //要过滤的ID低位
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;		//过滤器高16位每位必须匹配
    CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;		//过滤器低16位每位必须匹配
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1 ;//过滤器被关联到FIFO1，同时使能FIF0_1
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;		//使能过滤器
    CAN_FilterInit(&CAN_FilterInitStructure);
}



/*******************************************************************************************
 * 函数名：can_init
 * 描述  ：初始化CAN的功能
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 *******************************************************************************************/
void can_init(void)
{
    can_gpio_config();
    can_mode_config();
    can_filter_config();
    can_nvic_config();//经过测试CAN必须最后开中断使能
}


/*******************************************************************************************
**发送数据
*******************************************************************************************/
uint8_t can_send_msg(uint8_t* msg,uint8_t len,uint32_t id)
{
    uint8_t mbox;
    uint16_t i;
    CanTxMsg TxMessage;

    TxMessage.StdId =id & 0x000007FF;	//  标准标识符为0
    TxMessage.ExtId =0;               // 设置扩展标示符（29位）
    TxMessage.IDE   =CAN_Id_Standard; // 使用标准识符
    TxMessage.RTR   =0;		            // 消息类型为数据帧，一帧8位
    TxMessage.DLC   =len;			        // 发送信息帧的帧长
    for(i=0; i<len; i++)  TxMessage.Data[i]  = msg[i];// 帧信息缓存
    for(; len<8; len++)   TxMessage.Data[len]= 0; // 帧信息缓存

    mbox=CAN_Transmit(CAN1, &TxMessage);

    if(mbox != CAN_TxStatus_NoMailBox)//如果已经装入邮箱
    {
        return(0);
    }
    else
    {
        return(1);
    }
}



/*
**接收中断服务函数
*/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0) !=0)//如果缓存不为空
	{
			CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);//can总线接收数据函数
			can_data_receive_deal_handler(RxMessage);
	}
}

/*
**发送完成中断服务函数
*/
void USB_HP_CAN1_TX_IRQHandler(void)
{
   /***********************CAN数据发送完成数据中断*******************/
	 if(CAN_GetITStatus(CAN1,CAN_IT_TME)!=RESET) 
	 {
		  CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	    can_data_send_deal_handler();
	 }

}









