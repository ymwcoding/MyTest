#include "cmd_analyze.h"
#include "my_types.h"
#include "crc_lookup.h"
#include "string.h"
#include "can_receive.h"
#include "cmd_execute.h"
#include "config.h"


#define CMD_BUFFER_LENGTH       (512)    //通信的buffer长度

CMD_DATA      receiveCmdData;

void cmd_read_buffer(ReceiveRingBuffer* buffer,uint8_t *data,uint16_t len);

/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/

/**
*描述:初次解析usb获取到的数据,检测帧头和帧尾
*输入:*p_data -- 传入数据指针
			 len --传入数据长度   
*输出:无
*/
void cmd_check_head_tail(ReceiveRingBuffer* buffer,uint8_t *p_data,uint16_t len)
{
	receive_ring_buffer_check_uinit_data(buffer,p_data,len);//检测写入的数据是否已经存在数据单元
}


/*
**将check++后与all比较是否越界
*/
uint16_t check_out(uint16_t check,uint16_t all)
{
	uint16_t temp = check;
	temp++;
	if(temp >= all)
	{
		temp -= all;
	}
	return temp;
}



/*
**解析缓冲中的指令
*/
void get_cmd(ReceiveRingBuffer* buffer)
{
	uint8_t  state = 0;
	uint16_t i = 0;
	uint8_t  cmd_buffer[CMD_BUFFER_LENGTH] = {0};//一条完整的指令缓冲
  uint16_t cmd_crc_value = 0;//帧数据自带的crc
  uint16_t cmd_get_crc = 0;  //根据帧内容值计算出来的crc

  uint16_t read_index = buffer->readIdx;//传入开始读的指针
  uint16_t content_index = 0;
  uint16_t cmd_size = 0;

  while(1)
	{
		switch(state)
		{
			case 0://第一个帧头
			{
				if(buffer->data_ptr[read_index] == CMD_FRAME_HEADER_L)
				{
					state = 1;
				}
				if(++read_index >= buffer->byteSize)//检测是否越界
				{
					read_index = 0;
				}
			}
			break;
			
			case 1://第二个帧头
			{
				if(buffer->data_ptr[read_index] == CMD_FRAME_HEADER_H)
				{
					state = 2;
				}
				if(++read_index >= buffer->byteSize)//检测是否越界
				{
					read_index = 0;
				}
			}
			
			break;
			
			case 2:
			{
				/*--------------------取帧长------------------------------*/
				content_index = read_index;
				cmd_size = (uint16_t)(buffer->data_ptr[content_index]);
				cmd_buffer[2] = (buffer->data_ptr[content_index]);//将帧长先填进去
				content_index = check_out(content_index,buffer->byteSize);//content_index++并检测是否越界
				cmd_size |= ((uint16_t)(buffer->data_ptr[content_index]))<<8;//数据帧长
				cmd_buffer[3] = (buffer->data_ptr[content_index]);
				
				/*--------------------取帧尾-------------------------------*/
				content_index = read_index + cmd_size + 3;	
				
				content_index = check_out(content_index,buffer->byteSize);//content_index++并检测是否越界
				if((buffer->data_ptr[(content_index) % buffer->byteSize] != CMD_FRAME_TAIL_L))
				{
					state = 0;
					break;
				}
				
				content_index = check_out(content_index,buffer->byteSize);//content_index++并检测是否越界
				if((buffer->data_ptr[(content_index) % buffer->byteSize] != CMD_FRAME_TAIL_H))
				{
					state = 0;
					break;
				}
				/*-----------------取数据存入缓存---------------------------*/
				content_index = read_index+1;
				for(i=0;i<cmd_size;i++)
				{
					content_index = check_out(content_index,buffer->byteSize);//content_index++并检测是否越界
					cmd_buffer[i+4]= buffer->data_ptr[content_index];
				}
				
				/*--------------------读取CRC校验值------------------------*/
				content_index = check_out(content_index,buffer->byteSize);//content_index++并检测是否越界
				cmd_get_crc  = (uint16_t)(buffer->data_ptr[content_index]);
				cmd_buffer[cmd_size+4] = buffer->data_ptr[content_index];//事先把校验位填进去
				if(++content_index >= buffer->byteSize) content_index -= buffer->byteSize;
				cmd_get_crc |= (uint16_t)(buffer->data_ptr[content_index])<<8;
				cmd_buffer[cmd_size+5] = buffer->data_ptr[content_index];//事先把校验位填进去

				/*-----------计算CRC校验值并与接收的CRC校验值比对-----------*/
				cmd_crc_value = crc16_lookup(&cmd_buffer[4],cmd_size);//计算CRC校验值
				if(cmd_get_crc == cmd_crc_value)//如果校验正确
				{					
					cmd_buffer[0] = CMD_FRAME_HEADER_L;//把帧头帧尾填进去
					cmd_buffer[1] = CMD_FRAME_HEADER_H;
					cmd_buffer[cmd_size+6] = CMD_FRAME_TAIL_L;
					cmd_buffer[cmd_size+7] = CMD_FRAME_TAIL_H;
					cmd_read_buffer(buffer,cmd_buffer,cmd_size+8);//调用命令解析函数，执行命令
					
					if( read_index >= 2 )
						content_index = read_index-2;
					else
						content_index = read_index + buffer->byteSize - 2;

					for(i=0;i<cmd_size+8;i++)//清空已处理命令缓存
					{
						buffer->data_ptr[content_index] = 0;
						content_index = check_out(content_index,buffer->byteSize);//content_index++并检测是否越界
					}
					read_index = content_index;//指向数据末端
					buffer->readIdx= read_index;//数据运算指针累加
				}
				/*-----------运行查询一次结束跳出继续-----------*/
				state=0;//跳回模式0重新执行
				
			}
				
			break;
			
			default:
				state = 0;
			  read_index = 0;
			break;
		}
		
		if(buffer->writeIdx==buffer->byteSize-1) 	
		{
			if(read_index==0)return;
		}else 
		{
			if(read_index==buffer->writeIdx+1)return;
		}
	}
	
}

/******************************************************************************
功能：解析指令中的数据
输入：无
输出：无
调用：外部调用
******************************************************************************/
void cmd_read_buffer(ReceiveRingBuffer* buffer,uint8_t *data,uint16_t len)
{
	receiveCmdData.target_id    = BUILD_UINT16_T(data[4],data[5]);
	receiveCmdData.my_id        = BUILD_UINT16_T(data[6],data[7]);
	receiveCmdData.serial_number= BUILD_UINT16_T(data[8],data[9]);
	receiveCmdData.commd_type   = BUILD_UINT16_T(data[10],data[11]);
	receiveCmdData.commd_naber  = BUILD_UINT16_T(data[12],data[13]);
	receiveCmdData.buffer_light = len-18;
	if(receiveCmdData.buffer_light)
	{
			receiveCmdData.databuffer = &data[14];
	}
	cmd_execute(&receiveCmdData);//执行指令
}


