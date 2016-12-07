#ifndef _CAN_SEND_H__
#define _CAN_SEND_H__
#include <stm32f10x.h>
#include "cmd_execute.h"


#define CAN_TX_MAX_LEN	512  //CAN缓存


typedef struct
{
	uint8_t  can_send_buffer[CAN_TX_MAX_LEN];//CAN数据发送缓存
	uint8_t  can_send_byus;           //CAN正在发送标志
	uint32_t can_send_id;             //CAN发送数据的目标ID
	uint16_t can_send_start_piont;    //CAN数据发送当前指针
	uint16_t can_send_end_piont;      //CAN数据发送结束指针
}CAN_SEND_DATA;  


void cmd_send_pack(CMD_DATA* repley_data);
void can_start_send(void);
#endif
