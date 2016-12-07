#ifndef _TOP_EAR_LED_H__
#define _TOP_EAR_LED_H__
#include <stm32f10x.h>
#include "gpio_bit.h"

#define LED1_R  PAout(2)
#define LED2_R  PBout(1)  

#define LED1_G  PAout(1)
#define LED2_G  PBout(6) 

#define LED1_B  PAout(3)
#define LED2_B  PBout(7) 


void top_ear_led_on_off(uint8_t led_naber,uint8_t led_rgb,uint8_t mode);
void top_ear_led_gpio_init(void);

#endif
