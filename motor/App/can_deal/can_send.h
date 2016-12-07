#ifndef _CAN_SEND_H__
#define _CAN_SEND_H__
#include <stm32f10x.h>
#include "cmd_execute.h"


#define CAN_TX_MAX_LEN	512  //CAN����


typedef struct
{
	uint8_t  can_send_buffer[CAN_TX_MAX_LEN];//CAN���ݷ��ͻ���
	uint8_t  can_send_byus;           //CAN���ڷ��ͱ�־
	uint32_t can_send_id;             //CAN�������ݵ�Ŀ��ID
	uint16_t can_send_start_piont;    //CAN���ݷ��͵�ǰָ��
	uint16_t can_send_end_piont;      //CAN���ݷ��ͽ���ָ��
}CAN_SEND_DATA;  


void cmd_send_pack(CMD_DATA* repley_data);
void can_start_send(void);
#endif
