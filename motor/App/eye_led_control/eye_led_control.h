#ifndef _EYE_LED_CONTROL_H__
#define _EYE_LED_CONTROL_H__
#include <stm32f10x.h>

void eye_led_control(uint8_t mode,uint8_t *rgb_data,uint8_t rgb_lght);
void eye_led_control_main(void);
void eye_led_timer_task(void);

#endif



