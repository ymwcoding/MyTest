#include "receive_ring_buffer.h"
#include "stdlib.h"
#include "string.h"
#include "config.h"


/**
*��������:�������������Ƿ�������ݵ�Ԫ
*����:buffer--�����Ļ��λ��壬wrdata--д�������,len--д���������
*���:��
*/
void receive_ring_buffer_check_uinit_data(ReceiveRingBuffer* buffer,uint8_t* wrdata, uint16_t len)
{
	uint16_t i = 0;
	
	for(i=0; i<len; i++)
	{
		if(++buffer->writeIdx > buffer->byteSize)//�������û�г���
		{
			buffer->writeIdx = 0;
		}
		buffer->data_ptr[buffer->writeIdx] = wrdata[i];//���ݴ��뻺��
		switch(buffer->state)
		{
			case 0:
       if(wrdata[i] == CMD_FRAME_HEADER_L) 
			 {
				 buffer->state=1;
			 }
      break;
			 
			case 1:
				if(wrdata[i] == CMD_FRAME_HEADER_H) //�ҵ�֡ͷ
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
				if(wrdata[i] == CMD_FRAME_TAIL_H)  //��⵽֡ͷ�ּ�⵽֡β��˵������һ��������
				{
					buffer->state=0;   
					buffer->cmdDataSaveCount++;
					buffer->startDecodeFunc();//��ʼ����
				}					
			 break;
				
			default:{buffer->state=0;}break;
		}		
	}
}
