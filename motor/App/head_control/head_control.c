#include "head_control.h"
#include "delay.h"
#include "stdlib.h"

uint8_t  _head_tim=0;

/******************************************************************
 * 函数介绍:led控制节奏（定时器3）中断处理函数
 * 参数：无
 * 返回值：无
 * 备注:
 ******************************************************************/
void head_control_interrupt(void)
{
	uint8_t i,j;
	static uint32_t eye_data=0;
	
	if(_head_tim==0)
	{
		for(i=0;i<10;i++)
		{
			for(j=0;j<3;j++)
			{
				_head_rgb_ds_ls[i][j]=_head_rgb_ds[i][j];
			}
		}	
	}
	
  //眼睛和耳朵灯光
	for(i=0;i<2;i++)
	{
		for(j=0;j<3;j++)
		{
			 if(_head_tim==0)
			 {
				 if(_head_rgb_ds_ls[i][j]!=0)
				 {
						head_lamplight_led_no_off(i,j,1);//打开对应灯
				 }
			 }
			 else if(_head_rgb_ds_ls[i][j]<=_head_tim)
			 {
				 head_lamplight_led_no_off(i,j,0);//关闭对应灯
			 }
		}
	}	
  //眼睛灯光显示
	for(i=2;i<10;i++)
	{
		for(j=0;j<3;j++)
		{
			 if(_head_tim==0)
			 {
				 if(_head_rgb_ds_ls[i][j]!=0)
				 {
						eye_data |=(0x00000001<<((i-2)*3+j));//打开对应灯
				 }
			 }
			 else if(_head_rgb_ds_ls[i][j]<=_head_tim)
			 {
				 eye_data &=(~(0x00000001<<((i-2)*3+j)));//关闭对应灯
			 }
		}
	}	
	head_eye_transpose(eye_data);//LED控制
	
	_head_tim+=8;
	if(_head_tim>=248)
	{
		_head_tim=0;
	}
}






