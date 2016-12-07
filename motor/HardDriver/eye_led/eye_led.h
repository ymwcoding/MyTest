#ifndef _EYE_LED_H__
#define _EYE_LED_H__

#include <stm32f10x.h>
#include "gpio_bit.h"


#define LED0_OE  PAout(5)
#define LED0_LE  PAout(6) 
#define LED0_CLK PAout(7)  
#define LED0_SDI PBout(0) 

#define LED1_OE  PAout(8) 
#define LED1_LE  PBout(15)
#define LED1_CLK PBout(14) 
#define LED1_SDI PBout(2)  
 
#define LED2_OE  PAout(9)  
#define LED2_LE  PAout(10)   
#define LED2_CLK PAout(11)    
#define LED2_SDI PAout(12) 

#define LED3_OE  PBout(5)
#define LED3_LE  PBout(4)
#define LED3_CLK PBout(3) 
#define LED3_SDI PAout(15)


void head_eye_gpio_nit(void);
void head_eye_ledout(uint16_t led0,uint16_t led1,uint16_t led2,uint16_t led3);
void head_eye_transpose(uint32_t led_out);


#endif

