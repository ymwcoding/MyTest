#ifndef _CAN_H__
#define _CAN_H__
#include <stm32f10x.h>


uint8_t can_send_msg(uint8_t* msg,uint8_t len,uint32_t id);
void can_init(void);

#endif
