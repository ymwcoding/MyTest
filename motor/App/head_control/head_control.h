#ifndef _HEAD_CONTROL_H__
#define _HEAD_CONTROL_H__
#include <stm32f10x.h>
#include "gpio_bit.h"


#define LED1_R  PAout(2)
#define LED2_R  PBout(1)  

#define LED1_G  PAout(1)
#define LED2_G  PBout(6) 

#define LED1_B  PAout(3)
#define LED2_B  PBout(7) 

#define MIC_NABER 4  //4个mic接口


extern uint8_t _head_top;
extern uint8_t _head_top_heat;//头部触摸0为没触摸，1为已经触摸


void head_lamplight_led_no_off(uint8_t led_naber,uint8_t led_rgb,uint8_t mode); //灯开关函数
void head_init_control_rhythml(uint16_t ControlTim);//初始化控制终中断
void head_lamplight_init(void);   //控制初始化
void head_control_interrupt(void);//控制中断函数
void head_usart_interrupt(uint8_t r_data);//串口接收中断函数

void head_eye_control_main(void);//眼睛控制主函数
void head_ear_control_main(void);//耳朵控制主函数
void head_top_control_main(void);//头顶灯控制主函数
/*********************************************************/	
void head_eye_graduated_colors(void);//眼睛连续色彩变换
void head_eye_blink(void);           //眼睛眨眼
void head_eye_marquee(uint8_t led_r,uint8_t led_g,uint8_t led_b);//眼睛等待模式

void head_ear_graduated_colors(void);//耳朵连续色彩变换
void head_ear_bln_control(void);     //耳朵呼吸灯

void head_top_graduated_colors(void);//头顶灯连续色彩变换
void head_top_bln_control(void);     //头顶呼吸灯

void head_eye_control(uint8_t mode,uint8_t *rgbdata,uint16_t rgblght);//眼睛控制
void head_ear_control(uint8_t mode,uint8_t *rgbdata,uint16_t rgblght);//耳朵控制
void head_top_control(uint8_t mode,uint8_t *rgbdata,uint16_t rgblght);//头顶控制
void head_read_data(uint8_t *readdata);//参数读取
void head_setsoundsourceangle(uint8_t anglenaber);//设置波束成形角度






#endif

