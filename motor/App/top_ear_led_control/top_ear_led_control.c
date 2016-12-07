#include "top_ear_led_control.h"
#include "string.h"
#include "delay.h"
#include "top_ear_led.h"

uint8_t topEarLedMarquee[2][3] = {0};//头顶和耳朵跑马灯变量
 uint8_t topEarLedRgbData[2][3] = {0};//头顶和耳朵灯rgb数据
uint8_t topEarLedMode[2] = {0};        //头顶和耳朵灯模式
 uint8_t topEarLedRgbDs[2][3]={0};    //临时变量
uint8_t topEarLedRgbDsLs[2][3]={0};    //临时变量



static uint8_t topEarLedTimeCount = 0;


/******************************************************************
 * 函数介绍:根据参数控制头顶和耳朵灯
 * 参数：无
 * 返回值：无
 * 备注:
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
 * 函数介绍:头顶和耳朵颜色渐变（注意死循环）
 * 参数：无
 * 返回值：无
 * 备注:
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
 * 函数介绍:头顶和耳朵呼吸灯
 * 参数：无
 * 返回值：无
 * 备注:
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
 * 函数介绍:头顶和耳朵led控制主函数
 * 参数：无
 * 返回值：无
 * 备注:
 * 模式0：全亮模式
 * 模式1：色彩连续变换模式
 * 模式2：呼吸灯模式
 ******************************************************************/
void top_ear_led_control_main(uint8_t type)
{
	memcpy(&topEarLedRgbDs[type],&topEarLedRgbData[type],3);
	switch(topEarLedMode[type])
	{
		case 0://直接显示控制颜色
		{
			delay_ms(50);
		}break;
			
		case 1://色彩连续变换模式
		{
			top_ear_led_graduated_colors(type);
		}break;
		
		case 2://呼吸灯模式
		{
			 top_ear_led_bln_control(type);
		}break;
	}
}	


/*
**定时器任务
*/
void top_ear_led_timer_task(void)
{
	uint8_t i=0,j=0;
	if(topEarLedTimeCount == 0)
	{
		memcpy(&topEarLedRgbDsLs[0][0],&topEarLedRgbDs[0][0],6);
	}
	
	//眼睛和耳朵灯光
	for(i=0;i<2;i++)
	{
		for(j=0;j<3;j++)
		{
			 if(topEarLedTimeCount==0)
			 {
				 if(topEarLedRgbDsLs[i][j]!=0)
				 {
						top_ear_led_on_off(i,j,1);//打开对应灯
				 }
			 }
			 else if(topEarLedRgbDsLs[i][j]<=topEarLedTimeCount)
			 {
				 top_ear_led_on_off(i,j,0);//关闭对应灯
			 }
		}
	}	
	
	topEarLedTimeCount+=8;
	if(topEarLedTimeCount>=248)
	{
		topEarLedTimeCount=0;
	}
}


