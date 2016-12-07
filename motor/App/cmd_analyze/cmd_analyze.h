#ifndef _CMD_ANALYZE_H__
#define _CMD_ANALYZE_H__

#include <stm32f10x.h>
#include "receive_ring_buffer.h"


void cmd_check_head_tail(ReceiveRingBuffer* buffer,uint8_t *p_data,uint16_t len);
void get_cmd(ReceiveRingBuffer* buffer);


#endif
