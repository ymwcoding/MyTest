#include "cmd_analyze.h"
#include "my_types.h"
#include "crc_lookup.h"
#include "string.h"
#include "can_receive.h"
#include "cmd_execute.h"
#include "config.h"


#define CMD_BUFFER_LENGTH       (512)    //ͨ�ŵ�buffer����

CMD_DATA      receiveCmdData;

void cmd_read_buffer(ReceiveRingBuffer* buffer,uint8_t *data,uint16_t len);

/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/

/**
*����:���ν���usb��ȡ��������,���֡ͷ��֡β
*����:*p_data -- ��������ָ��
			 len --�������ݳ���   
*���:��
*/
void cmd_check_head_tail(ReceiveRingBuffer* buffer,uint8_t *p_data,uint16_t len)
{
	receive_ring_buffer_check_uinit_data(buffer,p_data,len);//���д��������Ƿ��Ѿ��������ݵ�Ԫ
}


/*
**��check++����all�Ƚ��Ƿ�Խ��
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
**���������е�ָ��
*/
void get_cmd(ReceiveRingBuffer* buffer)
{
	uint8_t  state = 0;
	uint16_t i = 0;
	uint8_t  cmd_buffer[CMD_BUFFER_LENGTH] = {0};//һ��������ָ���
  uint16_t cmd_crc_value = 0;//֡�����Դ���crc
  uint16_t cmd_get_crc = 0;  //����֡����ֵ���������crc

  uint16_t read_index = buffer->readIdx;//���뿪ʼ����ָ��
  uint16_t content_index = 0;
  uint16_t cmd_size = 0;

  while(1)
	{
		switch(state)
		{
			case 0://��һ��֡ͷ
			{
				if(buffer->data_ptr[read_index] == CMD_FRAME_HEADER_L)
				{
					state = 1;
				}
				if(++read_index >= buffer->byteSize)//����Ƿ�Խ��
				{
					read_index = 0;
				}
			}
			break;
			
			case 1://�ڶ���֡ͷ
			{
				if(buffer->data_ptr[read_index] == CMD_FRAME_HEADER_H)
				{
					state = 2;
				}
				if(++read_index >= buffer->byteSize)//����Ƿ�Խ��
				{
					read_index = 0;
				}
			}
			
			break;
			
			case 2:
			{
				/*--------------------ȡ֡��------------------------------*/
				content_index = read_index;
				cmd_size = (uint16_t)(buffer->data_ptr[content_index]);
				cmd_buffer[2] = (buffer->data_ptr[content_index]);//��֡�������ȥ
				content_index = check_out(content_index,buffer->byteSize);//content_index++������Ƿ�Խ��
				cmd_size |= ((uint16_t)(buffer->data_ptr[content_index]))<<8;//����֡��
				cmd_buffer[3] = (buffer->data_ptr[content_index]);
				
				/*--------------------ȡ֡β-------------------------------*/
				content_index = read_index + cmd_size + 3;	
				
				content_index = check_out(content_index,buffer->byteSize);//content_index++������Ƿ�Խ��
				if((buffer->data_ptr[(content_index) % buffer->byteSize] != CMD_FRAME_TAIL_L))
				{
					state = 0;
					break;
				}
				
				content_index = check_out(content_index,buffer->byteSize);//content_index++������Ƿ�Խ��
				if((buffer->data_ptr[(content_index) % buffer->byteSize] != CMD_FRAME_TAIL_H))
				{
					state = 0;
					break;
				}
				/*-----------------ȡ���ݴ��뻺��---------------------------*/
				content_index = read_index+1;
				for(i=0;i<cmd_size;i++)
				{
					content_index = check_out(content_index,buffer->byteSize);//content_index++������Ƿ�Խ��
					cmd_buffer[i+4]= buffer->data_ptr[content_index];
				}
				
				/*--------------------��ȡCRCУ��ֵ------------------------*/
				content_index = check_out(content_index,buffer->byteSize);//content_index++������Ƿ�Խ��
				cmd_get_crc  = (uint16_t)(buffer->data_ptr[content_index]);
				cmd_buffer[cmd_size+4] = buffer->data_ptr[content_index];//���Ȱ�У��λ���ȥ
				if(++content_index >= buffer->byteSize) content_index -= buffer->byteSize;
				cmd_get_crc |= (uint16_t)(buffer->data_ptr[content_index])<<8;
				cmd_buffer[cmd_size+5] = buffer->data_ptr[content_index];//���Ȱ�У��λ���ȥ

				/*-----------����CRCУ��ֵ������յ�CRCУ��ֵ�ȶ�-----------*/
				cmd_crc_value = crc16_lookup(&cmd_buffer[4],cmd_size);//����CRCУ��ֵ
				if(cmd_get_crc == cmd_crc_value)//���У����ȷ
				{					
					cmd_buffer[0] = CMD_FRAME_HEADER_L;//��֡ͷ֡β���ȥ
					cmd_buffer[1] = CMD_FRAME_HEADER_H;
					cmd_buffer[cmd_size+6] = CMD_FRAME_TAIL_L;
					cmd_buffer[cmd_size+7] = CMD_FRAME_TAIL_H;
					cmd_read_buffer(buffer,cmd_buffer,cmd_size+8);//�����������������ִ������
					
					if( read_index >= 2 )
						content_index = read_index-2;
					else
						content_index = read_index + buffer->byteSize - 2;

					for(i=0;i<cmd_size+8;i++)//����Ѵ��������
					{
						buffer->data_ptr[content_index] = 0;
						content_index = check_out(content_index,buffer->byteSize);//content_index++������Ƿ�Խ��
					}
					read_index = content_index;//ָ������ĩ��
					buffer->readIdx= read_index;//��������ָ���ۼ�
				}
				/*-----------���в�ѯһ�ν�����������-----------*/
				state=0;//����ģʽ0����ִ��
				
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
���ܣ�����ָ���е�����
���룺��
�������
���ã��ⲿ����
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
	cmd_execute(&receiveCmdData);//ִ��ָ��
}


