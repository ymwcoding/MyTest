#include "top_ear_led_control.h"
#include "string.h"
#include "delay.h"
#include "top_ear_led.h"

uint8_t topEarLedMarquee[2][3] = {0};//ͷ���Ͷ�������Ʊ���
 uint8_t topEarLedRgbData[2][3] = {0};//ͷ���Ͷ����rgb����
uint8_t topEarLedMode[2] = {0};        //ͷ���Ͷ����ģʽ
 uint8_t topEarLedRgbDs[2][3]={0};    //��ʱ����
uint8_t topEarLedRgbDsLs[2][3]={0};    //��ʱ����



static uint8_t topEarLedTimeCount = 0;


/******************************************************************
 * ��������:���ݲ�������ͷ���Ͷ����
 * ��������
 * ����ֵ����
 * ��ע:
 ******************************************************************/
void top_ear_led_control(uint8_t mode,uint8_t *rgbdata,uint16_t rgblght,uint8_t type)
{
	if(rgblght > 3) return;
	memcpy(&topEarLedRgbData[type],rgbdata,3);
	
	topEarLedMode[type]=mode;
	
	if(topEarLedMode[type]==2)
	{
		memcpy(&topEarLedMarquee[type],&topEarLedRgbData[type],3);
	}
}



/******************************************************************
 * ��������:ͷ���Ͷ�����ɫ���䣨ע����ѭ����
 * ��������
 * ����ֵ����
 * ��ע:
 ******************************************************************/
void top_ear_led_graduated_colors(uint8_t type)
{
		static uint8_t i;
   //-----------------------------
		topEarLedRgbDs[type][0]=255;
		topEarLedRgbDs[type][1]=0;
		topEarLedRgbDs[type][2]=255;
		for(i=0;i<254;i++)
		{
			topEarLedRgbDs[type][2]--;
			if(topEarLedMode[type] != 1) return;
			delay_ms(5);
		}
   //-----------------------------
		topEarLedRgbDs[type][0]=255;
		topEarLedRgbDs[type][1]=0;
		topEarLedRgbDs[type][2]=0;
		for(i=0;i<254;i++)
		{
			topEarLedRgbDs[type][1]++;
			if(topEarLedMode[type] != 1) return;
			delay_ms(5);
		}
		//-------------------------
		topEarLedRgbDs[type][0]=255;
		topEarLedRgbDs[type][1]=255;
		topEarLedRgbDs[type][2]=0;
		for(i=0;i<254;i++)
		{
			topEarLedRgbDs[type][0]--;
			if(topEarLedMode[type] != 1) return;
			delay_ms(5);
		}
		//-------------------------
		topEarLedRgbDs[type][0]=0;
		topEarLedRgbDs[type][1]=255;
		topEarLedRgbDs[type][2]=0;
		for(i=0;i<254;i++)
		{
			topEarLedRgbDs[type][2]++;
			if(topEarLedMode[type] != 1) return;
			delay_ms(5);
		}
		//-------------------------
		topEarLedRgbDs[type][0]=0;
		topEarLedRgbDs[type][1]=255;
		topEarLedRgbDs[type][2]=255;
		for(i=0;i<254;i++)
		{
			topEarLedRgbDs[type][1]--;
			if(topEarLedMode[type] != 1) return;
			delay_ms(5);
		}
		//-------------------------
		topEarLedRgbDs[type][0]=0;
		topEarLedRgbDs[type][1]=0;
		topEarLedRgbDs[type][2]=255;
		for(i=0;i<254;i++)
		{
			topEarLedRgbDs[type][0]++;
			if(topEarLedMode[type] != 1) return;
			delay_ms(5);
		}
}



/******************************************************************
 * ��������:ͷ���Ͷ��������
 * ��������
 * ����ֵ����
 * ��ע:
 ******************************************************************/
void top_ear_led_bln_control(uint8_t type)
{
	static uint16_t naber=0;
	uint8_t i = 0;
	
	if(++naber>=200) naber=0;
	
	if(naber<100)
	{
		for(i=0; i<3; i++)
		{
			topEarLedRgbDs[type][i]=(uint8_t)((uint16_t)topEarLedMarquee[type][i]*naber/100);
		}
	}
	else
	{
		for(i=0; i<3; i++)
		{
			topEarLedRgbDs[type][i]=(uint8_t)((uint16_t)topEarLedMarquee[type][i]*(200-naber)/100);
		}
	}
	delay_ms(20);
}



/******************************************************************
 * ��������:ͷ���Ͷ���led����������
 * ��������
 * ����ֵ����
 * ��ע:
 * ģʽ0��ȫ��ģʽ
 * ģʽ1��ɫ�������任ģʽ
 * ģʽ2��������ģʽ
 ******************************************************************/
void top_ear_led_control_main(uint8_t type)
{
	memcpy(&topEarLedRgbDs[type],&topEarLedRgbData[type],3);
	switch(topEarLedMode[type])
	{
		case 0://ֱ����ʾ������ɫ
		{
			delay_ms(50);
		}break;
			
		case 1://ɫ�������任ģʽ
		{
			top_ear_led_graduated_colors(type);
		}break;
		
		case 2://������ģʽ
		{
			 top_ear_led_bln_control(type);
		}break;
	}
}	


/*
**��ʱ������
*/
void top_ear_led_timer_task(void)
{
	uint8_t i=0,j=0;
	if(topEarLedTimeCount == 0)
	{
		memcpy(&topEarLedRgbDsLs[0][0],&topEarLedRgbDs[0][0],6);
	}
	
	//�۾��Ͷ���ƹ�
	for(i=0;i<2;i++)
	{
		for(j=0;j<3;j++)
		{
			 if(topEarLedTimeCount==0)
			 {
				 if(topEarLedRgbDsLs[i][j]!=0)
				 {
						top_ear_led_on_off(i,j,1);//�򿪶�Ӧ��
				 }
			 }
			 else if(topEarLedRgbDsLs[i][j]<=topEarLedTimeCount)
			 {
				 top_ear_led_on_off(i,j,0);//�رն�Ӧ��
			 }
		}
	}	
	
	topEarLedTimeCount+=8;
	if(topEarLedTimeCount>=248)
	{
		topEarLedTimeCount=0;
	}
}


