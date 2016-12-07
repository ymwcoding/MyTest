#include "can_send.h"
#include "can.h"
#include "my_types.h"
#include "delay.h"
#include "string.h"
#include "crc_lookup.h"
#include "config.h"


CAN_SEND_DATA canSendData;//can���ݷ������ݽṹ��

uint8_t cmdSendBuffer[CAN_TX_MAX_LEN] = {0};//���ͻ���

static uint16_t get_can_send_buffer_remain_length(void);

/*******************************************************************************************
 * ��������can_data_send
 * ����  ��can �������ݷ��ͳ���
 * ����  �����յ����ж�����
 * ���  : �ɹ�����0  ʧ�ܷ�������
 * ����  ���ڲ�����
 *******************************************************************************************/
static uint8_t can_send_data(uint8_t *data,uint16_t length,uint16_t target_id)
{
	  uint16_t i = 0;
	  if(get_can_send_buffer_remain_length() < length) 
		{ 
			return (1);
		}
		canSendData.can_send_id = target_id;
		/****************���ݴ��뻺��************************/
    for(i=0; i<length; i++)
    {
			canSendData.can_send_buffer[canSendData.can_send_end_piont] = data[i];//���ݴ���buffer
			if(++canSendData.can_send_end_piont >= CAN_TX_MAX_LEN)//ָ���ۼӣ����λ���
			{
				canSendData.can_send_end_piont=0;
			}
    }
		/***����һ���Ժ����ݷ��������ݷ�������ж��������*****/
		if(canSendData.can_send_byus==0)//CAN���ڿ���ģʽ��������
		{
			can_start_send();
		}
    return(0);
		
}

/*********************************************************************
can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
len:���ݳ���(���Ϊ8)				     
msg:����ָ��,���Ϊ8���ֽ�.
����ֵ:0,�ɹ�;
		   ����,ʧ��;
*********************************************************************/
void can_start_send(void)
{
	int16_t over = CAN_TX_MAX_LEN - get_can_send_buffer_remain_length();//����ʣ����Ҫ���͵�buffer�ж���
	if(over == 0) 
	{
		canSendData.can_send_byus=0;//����CAN���Ϳ���ģʽ
		return;//����Ѿ��������������
	}
	else
	{
		canSendData.can_send_byus=1;//����CAN����æµģʽ
	}
	
	if(over>8) over=8;//�޶������������
	if(canSendData.can_send_start_piont+over>CAN_TX_MAX_LEN)
	{
		over=CAN_TX_MAX_LEN-canSendData.can_send_start_piont;
	}
	
	if(can_send_msg(&canSendData.can_send_buffer[canSendData.can_send_start_piont],over,canSendData.can_send_id))
	{
		return;
	}

	canSendData.can_send_start_piont+=over;//ָ���ۼ�
	if(canSendData.can_send_start_piont>= CAN_TX_MAX_LEN)
	{
		canSendData.can_send_start_piont-=CAN_TX_MAX_LEN;
	}
}



/*******************************************************************************************
 * ��������get_can_send_buffer_remain_length
 * ����  ������CAN�����ʣ��ռ�
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
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
**ָ���
*/
static void cmd_send(uint8_t *data,uint16_t size,uint16_t target_id)
{
	uint16_t crc_data;
	
	cmdSendBuffer[0]=CMD_FRAME_HEADER_L;
	cmdSendBuffer[1]=CMD_FRAME_HEADER_H;
	cmdSendBuffer[2]=LO_UINT16_T(size);
	cmdSendBuffer[3]=HI_UINT16_T(size);
	
	memcpy(&cmdSendBuffer[4],data,size);
	
	crc_data = crc16_lookup(&cmdSendBuffer[4],size);//����CRCУ��ֵ
	cmdSendBuffer[size+4]=LO_UINT16_T(crc_data);
	cmdSendBuffer[size+5]=HI_UINT16_T(crc_data);	
	cmdSendBuffer[size+6]=CMD_FRAME_TAIL_L;
	cmdSendBuffer[size+7]=CMD_FRAME_TAIL_H;	
	//�����ݴ����ȥ
	can_send_data(cmdSendBuffer,size+8,target_id);//��������
}


/*
**������
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



