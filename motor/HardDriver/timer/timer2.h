#ifndef _TIMER2_H__
#define _TIMER2_H__
#include <stm32f10x.h>

void timer2_init(uint16_t arr,uint16_t psc,uint8_t pre_prty,uint8_t sub_prty);
void timer2_start(void);
void timer2_stop(void);


#endif
