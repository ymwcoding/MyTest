#ifndef __USART3_H
#define __USART3_H
#include "stm32f10x.h"


void usart3_init(uint32_t bound,uint8_t pre_prty,uint8_t sub_prty);
void usart3_send_buf(uint8_t *buf,uint16_t len);
#endif
