#include "receive_ring_buffer.h"
#include "stdlib.h"
#include "string.h"
#include "config.h"


/**
*函数功能:缓冲检测数据中是否存在数据单元
*输入:buffer--创建的环形缓冲，wrdata--写入的数据,len--写入的数据量
*输出:无
*/
void receive_ring_buffer_check_uinit_data(ReceiveRingBuffer* buffer,uint8_t* wrdata, uint16_t len)
{
	uint16_t i = 0;
	
	for(i=0; i<len; i++)
	{
		if(++buffer->writeIdx > buffer->byteSize)//如果数据没有超限
		{
			buffer->writeIdx = 0;
		}
		buffer->data_ptr[buffer->writeIdx] = wrdata[i];//数据存入缓冲
		switch(buffer->state)
		{
			case 0:
       if(wrdata[i] == CMD_FRAME_HEADER_L) 
			 {
				 buffer->state=1;
			 }
      break;
			 
			case 1:
				if(wrdata[i] == CMD_FRAME_HEADER_H) //找到帧头
				{
					buffer->state=2;
				}
			break;
				
			case 2:
				if(wrdata[i] == CMD_FRAME_TAIL_L) 
				{
					buffer->state=3;
				}				
			break;
				
			case 3:
				if(wrdata[i] == CMD_FRAME_TAIL_H)  //检测到帧头又检测到帧尾，说明是有一条命令了
				{
					buffer->state=0;   
					buffer->cmdDataSaveCount++;
					buffer->startDecodeFunc();//开始解码
				}					
			 break;
				
			default:{buffer->state=0;}break;
		}		
	}
}
