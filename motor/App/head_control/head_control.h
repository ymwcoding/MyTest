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

#define MIC_NABER 4  //4��mic�ӿ�


extern uint8_t _head_top;
extern uint8_t _head_top_heat;//ͷ������0Ϊû������1Ϊ�Ѿ�����


void head_lamplight_led_no_off(uint8_t led_naber,uint8_t led_rgb,uint8_t mode); //�ƿ��غ���
void head_init_control_rhythml(uint16_t ControlTim);//��ʼ���������ж�
void head_lamplight_init(void);   //���Ƴ�ʼ��
void head_control_interrupt(void);//�����жϺ���
void head_usart_interrupt(uint8_t r_data);//���ڽ����жϺ���

void head_eye_control_main(void);//�۾�����������
void head_ear_control_main(void);//�������������
void head_top_control_main(void);//ͷ���ƿ���������
/*********************************************************/	
void head_eye_graduated_colors(void);//�۾�����ɫ�ʱ任
void head_eye_blink(void);           //�۾�գ��
void head_eye_marquee(uint8_t led_r,uint8_t led_g,uint8_t led_b);//�۾��ȴ�ģʽ

void head_ear_graduated_colors(void);//��������ɫ�ʱ任
void head_ear_bln_control(void);     //���������

void head_top_graduated_colors(void);//ͷ��������ɫ�ʱ任
void head_top_bln_control(void);     //ͷ��������

void head_eye_control(uint8_t mode,uint8_t *rgbdata,uint16_t rgblght);//�۾�����
void head_ear_control(uint8_t mode,uint8_t *rgbdata,uint16_t rgblght);//�������
void head_top_control(uint8_t mode,uint8_t *rgbdata,uint16_t rgblght);//ͷ������
void head_read_data(uint8_t *readdata);//������ȡ
void head_setsoundsourceangle(uint8_t anglenaber);//���ò������νǶ�






#endif

