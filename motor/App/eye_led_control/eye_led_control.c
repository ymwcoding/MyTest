#include "eye_led_control.h"
#include "delay.h"
#include "string.h"
#include "eye_led.h"

#define EYE_LED_COUNT   8                     //眼睛led灯数量

uint8_t eyeLedMarquee[3] = {0};               //眼睛跑马灯变量
uint8_t eyeLedRgbData[EYE_LED_COUNT][3] = {0};//眼睛灯rgb数据
 uint8_t eyeLedMode = 0;                       //眼睛灯模式
 uint8_t eyeLedRgbDs[EYE_LED_COUNT][3]={0};    //临时变量
 uint8_t eyeLedRgbDsLs[EYE_LED_COUNT][3]={0};    //临时变量


static uint8_t eyeLedTimeCount = 0;


/******************************************************************
 * 函数介绍:根据参数控制眼睛灯
 * 参数：无
 * 返回值：无
 * 备注:
 ******************************************************************/
void eye_led_control(uint8_t mode,uint8_t *rgb_data,uint8_t rgb_lght)
{
	if(rgb_lght!=24) return;
	
	memcpy(&eyeLedRgbData[0][0],rgb_data,24);
	
	eyeLedMode = mode;
	if(eyeLedMode == 3)
	{
		memcpy(eyeLedMarquee,rgb_data,3);
	}
}


/****************************************
功能：眨眼一次
输入：无
输出：无
调用方式;外部调用
*****************************************/
static void eye_led_blink(void)
{
	static const uint8_t offset[EYE_LED_COUNT] = {7,7,7,3,2,1,2,3};//固定的偏移量
	uint8_t i = 0,j = 0;
  uint8_t led_lings[EYE_LED_COUNT][3] = {0};

/*------------------------------------------------------*/

  memcpy(&led_lings[0][0],&eyeLedRgbDs[0][0],24);//复制数据
	
/*------------------------------------------------------*/  
	for(i=0; i<EYE_LED_COUNT; i++)
	{
		for(j=0; j<3; j++)
		{
			eyeLedRgbDs[i][j] >>= offset[i];
		}
	}
/*------------------------------------------------------*/
	
	delay_ms(100);
	memcpy(&eyeLedRgbDs[0][0],&led_lings[0][0],24);//复制数据
}	



/****************************************
功能：跑马灯
输入：led_r、led_g、led_b:跑马灯灯的rgb颜色
输出：无
调用方式;外部循环调用，建议调用周期为50ms
*****************************************/
static void eye_led_marquee(uint8_t led_r,uint8_t led_g,uint8_t led_b)
{
	int16_t j,i;
	static uint16_t tounaber=0;
	
	for(j=0; j<EYE_LED_COUNT; j++)
	{
		i=tounaber+j;
		if(i >= EYE_LED_COUNT) 
		{
			i=i-7;
		}
		eyeLedRgbDs[i][0]=led_r/(j*j);
		eyeLedRgbDs[i][1]=led_g/(j*j);		
		eyeLedRgbDs[i][2]=led_b/(j*j);
	}
	if(++tounaber >= EYE_LED_COUNT) 
	{
		tounaber=0;
	}
	delay_ms(60);
}


/******************************************************************
 * 函数介绍:led眼睛颜色渐变（注意死循环）
 * 参数：无
 * 返回值：无
 * 备注:
 ******************************************************************/
static void eye_led_graduated_colors(void)
{
		static uint8_t i,j;
//-----------------------------
		for(j=0; j<EYE_LED_COUNT; j++)
		{
			eyeLedRgbDs[j][0]=255;
			eyeLedRgbDs[j][1]=0;
			eyeLedRgbDs[j][2]=255;
		}
		for(i=0;i<254;i++)
		{
			for(j=0; j<EYE_LED_COUNT; j++)
			{
				eyeLedRgbDs[j][2]--;
			}
			if(eyeLedMode != 1) return;
			delay_ms(5);
		}
//-----------------------------
		for(j=0; j<EYE_LED_COUNT; j++)
		{
			eyeLedRgbDs[j][0]=255;
			eyeLedRgbDs[j][1]=0;
			eyeLedRgbDs[j][2]=0;
		}
		for(i=0;i<254;i++)
		{
			for(j=0; j<EYE_LED_COUNT; j++)
			{
				eyeLedRgbDs[j][1]++;
			}
			if(eyeLedMode != 1) return;
			delay_ms(5);
		}
//-----------------------------
		for(j=0; j<EYE_LED_COUNT; j++)
		{
		eyeLedRgbDs[j][0]=255;
		eyeLedRgbDs[j][1]=255;
		eyeLedRgbDs[j][2]=0;
		}
		for(i=0;i<254;i++)
		{
			for(j=0; j<EYE_LED_COUNT; j++)
			{
				eyeLedRgbDs[j][0]--;
			}
			if(eyeLedMode != 1) return;
			delay_ms(5);
		}
//-----------------------------
		for(j=0; j<EYE_LED_COUNT; j++)
		{
			eyeLedRgbDs[j][0]=0;
			eyeLedRgbDs[j][1]=255;
			eyeLedRgbDs[j][2]=0;
		}
		for(i=0;i<254;i++)
		{
			for(j=0; j<EYE_LED_COUNT; j++)
			{
				eyeLedRgbDs[j][2]++;
			}
			if(eyeLedMode != 1) return;
			delay_ms(5);
		}
//-----------------------------
		for(j=0; j<EYE_LED_COUNT; j++)
		{
			eyeLedRgbDs[j][0]=0;
			eyeLedRgbDs[j][1]=255;
			eyeLedRgbDs[j][2]=255;
		}
		for(i=0;i<254;i++)
		{
			for(j=0; j<EYE_LED_COUNT; j++)
			{
				eyeLedRgbDs[j][1]--;
			}
			if(eyeLedMode != 1) return;
			delay_ms(5);
		}
//-----------------------------
		for(j=0; j<EYE_LED_COUNT; j++)
		{
			eyeLedRgbDs[j][0]=0;
			eyeLedRgbDs[j][1]=0;
			eyeLedRgbDs[j][2]=255;
		}
		for(i=0;i<254;i++)
		{
			for(j=0; j<EYE_LED_COUNT; j++)
			{
				eyeLedRgbDs[j][0]++;
			}
			if(eyeLedMode != 1) return;
			delay_ms(5);
		}
}


/******************************************************************
 * 函数介绍:眼睛led控制主函数
 * 参数：无
 * 返回值：无
 * 备注:
 * 模式0：全亮模式
 * 模式1：眨眼模式
 * 模式2：等待模式
 * 模式3：色彩连续变换模式
 ******************************************************************/
void eye_led_control_main(void)
{
	memcpy(&eyeLedRgbDs[0][0],&eyeLedRgbData[0][0],24);//把数据复制出来
	switch(eyeLedMode)
	{
		case 0://直接显示控制颜色
		{
			delay_ms(50);
		}break;
			
		case 1://连续色彩变换模式
		{
			eye_led_graduated_colors();
		}break;
		
		case 2://眨眼
		{
			eye_led_blink();
			eyeLedMode=0;
		}break;
		
		case 3://眼睛等待模式
		{
			eye_led_marquee(eyeLedMarquee[0],eyeLedMarquee[1],eyeLedMarquee[2]);
		}break;
		
		default:break;
	}
}	


/*
**眼睛LED灯定时器任务
*/
void eye_led_timer_task(void)
{
	static uint32_t eye_data=0;
	uint8_t i = 0,j = 0;
	if(eyeLedTimeCount == 0)
	{
		memcpy(eyeLedRgbDsLs,eyeLedRgbDs,24);
	}
	//眼睛灯光显示
	for(i=0;i<EYE_LED_COUNT;i++)
	{
		for(j=0;j<3;j++)
		{
			 if(eyeLedTimeCount==0)
			 {
				 if(eyeLedRgbDsLs[i][j]!=0)
				 {
						eye_data |=(0x00000001<<((i-2)*3+j));//打开对应灯
				 }
			 }
			 else if(eyeLedRgbDsLs[i][j] <= eyeLedTimeCount)
			 {
				 eye_data &=(~(0x00000001<<((i-2)*3+j)));//关闭对应灯
			 }
		}
	}	
	head_eye_transpose(eye_data);//LED控制
	eyeLedTimeCount+=8;
	if(eyeLedTimeCount>=248)
	{
		eyeLedTimeCount=0;
	}
	
}

