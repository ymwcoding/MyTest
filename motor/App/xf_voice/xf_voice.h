#ifndef _XF_VOICE_H__
#define _XF_VOICE_H__
#include <stm32f10x.h>

void xf_voice_get_data(uint8_t* get_data);
void xf_usart_receive_deal(uint8_t r_data);
void xf_voice_set_angle(uint8_t anglenaber);


#endif
