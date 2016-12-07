#include "can_send.h"
#include "can.h"
#include "my_types.h"
#include "delay.h"
#include "string.h"
#include "crc_lookup.h"
#include "config.h"


CAN_SEND_DATA canSendData;//can数据发送数据结构体

uint8_t cmdSendBuffer[CAN_TX_MAX_LEN] = {0};//发送缓冲

static uint16_t get_can_send_buffer_remain_length(void);

/*******************************************************************************************
 * 函数名：can_data_send
 * 描述  ：can 缓存数据发送程序
 * 输入  ：接收到的中断数据
 * 输出  : 成功返回0  失败返回其他
 * 调用  ：内部调用
 *******************************************************************************************/
static uint8_t can_send_data(uint8_t *data,uint16_t length,uint16_t target_id)
{
	  uint16_t i = 0;
	  if(get_can_send_buffer_remain_length() < length) 
		{ 
			return (1);
		}
		canSendData.can_send_id = target_id;
		/****************数据存入缓存************************/
    for(i=0; i<length; i++)
    {
			canSendData.can_send_buffer[canSendData.can_send_end_piont] = data[i];//数据存入buffer
			if(++canSendData.can_send_end_piont >= CAN_TX_MAX_LEN)//指针累加，环形缓冲
			{
				canSendData.can_send_end_piont=0;
			}
    }
		/***发送一次以后数据发送在数据发送完成中断里面进行*****/
		if(canSendData.can_send_byus==0)//CAN处于空闲模式则开启发送
		{
			can_start_send();
		}
    return(0);
		
}

/*********************************************************************
can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
len:数据长度(最大为8)				     
msg:数据指针,最大为8个字节.
返回值:0,成功;
		   其他,失败;
*********************************************************************/
void can_start_send(void)
{
	int16_t over = CAN_TX_MAX_LEN - get_can_send_buffer_remain_length();//计算剩余需要发送的buffer有多少
	if(over == 0) 
	{
		canSendData.can_send_byus=0;//设置CAN发送空闲模式
		return;//如果已经发送完成则跳出
	}
	else
	{
		canSendData.can_send_byus=1;//设置CAN发送忙碌模式
	}
	
	if(over>8) over=8;//限定单次最大发送量
	if(canSendData.can_send_start_piont+over>CAN_TX_MAX_LEN)
	{
		over=CAN_TX_MAX_LEN-canSendData.can_send_start_piont;
	}
	
	if(can_send_msg(&canSendData.can_send_buffer[canSendData.can_send_start_piont],over,canSendData.can_send_id))
	{
		return;
	}

	canSendData.can_send_start_piont+=over;//指针累加
	if(canSendData.can_send_start_piont>= CAN_TX_MAX_LEN)
	{
		canSendData.can_send_start_piont-=CAN_TX_MAX_LEN;
	}
}



/*******************************************************************************************
 * 函数名：get_can_send_buffer_remain_length
 * 描述  ：计算CAN缓存的剩余空间
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 *******************************************************************************************/
static uint16_t get_can_send_buffer_remain_length(void)
{
	if(canSendData.can_send_end_piont >= canSendData.can_send_start_piont)
	{
		return(CAN_TX_MAX_LEN - canSendData.can_send_end_piont + canSendData.can_send_start_piont);
	}
	else
	{
		return(canSendData.can_send_start_piont - canSendData.can_send_end_piont);
	}
}



/*
**指令发送
*/
static void cmd_send(uint8_t *data,uint16_t size,uint16_t target_id)
{
	uint16_t crc_data;
	
	cmdSendBuffer[0]=CMD_FRAME_HEADER_L;
	cmdSendBuffer[1]=CMD_FRAME_HEADER_H;
	cmdSendBuffer[2]=LO_UINT16_T(size);
	cmdSendBuffer[3]=HI_UINT16_T(size);
	
	memcpy(&cmdSendBuffer[4],data,size);
	
	crc_data = crc16_lookup(&cmdSendBuffer[4],size);//计算CRC校验值
	cmdSendBuffer[size+4]=LO_UINT16_T(crc_data);
	cmdSendBuffer[size+5]=HI_UINT16_T(crc_data);	
	cmdSendBuffer[size+6]=CMD_FRAME_TAIL_L;
	cmdSendBuffer[size+7]=CMD_FRAME_TAIL_H;	
	//把数据传输出去
	can_send_data(cmdSendBuffer,size+8,target_id);//发送数据
}


/*
**命令打包
*/
void cmd_send_pack(CMD_DATA* repley_data)
{
	  uint8_t send_buffer[CAN_TX_MAX_LEN] = {0};
    uint16_t data_len = 0;
    uint16_t i = 0;

    send_buffer[0] = LO_UINT16_T(repley_data->target_id);
    send_buffer[1] = HI_UINT16_T(repley_data->target_id);
    send_buffer[2] = LO_UINT16_T(repley_data->my_id);
    send_buffer[3] = HI_UINT16_T(repley_data->my_id);
    send_buffer[4] = LO_UINT16_T(repley_data->serial_number);
    send_buffer[5] = HI_UINT16_T(repley_data->serial_number);
    send_buffer[6] = LO_UINT16_T(repley_data->commd_type);
    send_buffer[7] = HI_UINT16_T(repley_data->commd_type);
    send_buffer[8] = LO_UINT16_T(repley_data->commd_naber);
    send_buffer[9] = HI_UINT16_T(repley_data->commd_naber);

    data_len= 10 + repley_data->buffer_light;
    for(i=10; i<data_len; i++)
    {
        send_buffer[i] = repley_data->databuffer[i-10];
    }
    cmd_send(send_buffer,data_len,repley_data->target_id);
}



