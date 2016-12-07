#ifndef _TIMER4_H__
#define _TIMER4_H__
#include <stm32f10x.h>



void timer4_start(void);
void timer4_stop(void);
void timer4_init(uint16_t arr,uint16_t psc,uint8_t pre_prty,uint8_t sub_prty);



#endif
