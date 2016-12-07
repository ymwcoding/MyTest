#ifndef _TOP_EAR_LED_CONTROL_H__
#define _TOP_EAR_LED_CONTROL_H__
#include <stm32f10x.h>


enum
{
	EAR,
	TOP,
};


void top_ear_led_control(uint8_t mode,uint8_t *rgbdata,uint16_t rgblght,uint8_t type);

void top_ear_led_timer_task(void);
void top_ear_led_control_main(uint8_t type);

#endif
