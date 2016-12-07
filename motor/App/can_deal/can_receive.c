#include "can_receive.h"
#include "cmd_analyze.h"
#include "timer2.h"
#include "my_types.h"
#include "cmd_execute.h"
#include "config.h"


ReceiveRingBuffer canReceiveRingBuffer;                //CAN接收数据缓冲结构体
static uint8_t  canReceiveData[CAN_RX_MAX_LEN]={0};    //数据接收缓存
uint8_t  canReceiveOkCmdBuffer[CAN_RX_MAX_LEN+1] = {0};//接收到的正确的命令缓冲
uint16_t canReceiveOkCmdLen = 0;                       //接收到的正确命令的总长度

/*
**开启can数据解码
*/
static void can_start_decode(void)
{
	if(FlagBit.flag.canStartDecodeFlag == 0)//还没开启过解码
	{
		timer2_start();                       //开启定时器3开始解码
		FlagBit.flag.canStartDecodeFlag = 1;
	}
}

/*
**创建can数据接收的环形缓冲
*/
void can_receive_ring_buffer_create(void)
{
	  canReceiveRingBuffer.byteSize  = CAN_RX_MAX_LEN;//缓冲中总字节数
    canReceiveRingBuffer.readIdx = 0;//开始读取的地址
    canReceiveRingBuffer.writeIdx = 0;//写的地址
    canReceiveRingBuffer.data_ptr = canReceiveData;//数据缓冲指针
	  canReceiveRingBuffer.bufferId = 2;                      //缓冲标号，用于识别是哪个环形缓冲
	  canReceiveRingBuffer.cmdDataSaveCount = 0;
	  canReceiveRingBuffer.startDecodeFunc = can_start_decode;//开始单元数据解析的入口函数
	  canReceiveRingBuffer.state = 0;
}



/*********************************************************************************
 * 描述  :中断接收处理程序
 * 输入  :接收到的中断数据
 * 输出  :无
 * 说明  :USB上传一包中数据会自动加一个结束符（0x00），应滤除
*********************************************************************************/
void can_data_receive(CanRxMsg RxMessage)
{
	if(((uint16_t)(RxMessage.StdId)==MY_DEVICE_ID)||((uint16_t)(RxMessage.StdId)==ALL_DEVICE_ID))
	{
		cmd_check_head_tail(&canReceiveRingBuffer,RxMessage.Data,RxMessage.DLC);
	}
	
}
/*------------------------------------------------------------------------------*/


/*
**can开始解码接收到的数据
*/
void can_decode_receive(void)
{
	timer2_stop();                 //进来就把接收到数据解完
	while(canReceiveRingBuffer.cmdDataSaveCount)
	{
		canReceiveRingBuffer.cmdDataSaveCount = 0;
		get_cmd(&canReceiveRingBuffer);//can解码canReceiveRingBuffer中的数据
	}
	FlagBit.flag.canStartDecodeFlag = 0;//解完后后才可以重新开启解析
}



