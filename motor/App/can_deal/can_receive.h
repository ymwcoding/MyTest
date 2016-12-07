#ifndef _CAN_RECEIVE_H__
#define _CAN_RECEIVE_H__
#include <stm32f10x.h>
#include "receive_ring_buffer.h"

#define CAN_RX_MAX_LEN   512

extern uint16_t canReceiveOkCmdLen;
extern uint8_t canReceiveOkCmdBuffer[CAN_RX_MAX_LEN+1];
extern ReceiveRingBuffer canReceiveRingBuffer;

void can_data_receive(CanRxMsg RxMessage);
void can_receive_ring_buffer_create(void);
void can_decode_receive(void);

#endif
