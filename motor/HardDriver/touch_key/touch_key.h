#ifndef _TOUCH_KEY_H__
#define _TOUCH_KEY_H__
#include <stm32f10x.h>


#define  READ_KEY()              GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)



void get_touch_key_state(uint8_t* get_data);
void touch_key_init(void);
void touch_key_timer_task(void);

#endif
