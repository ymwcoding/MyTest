#include "head_control.h"
#include "delay.h"
#include "stdlib.h"

uint8_t  _head_tim=0;

/******************************************************************
 * ��������:led���ƽ��ࣨ��ʱ��3���жϴ�����
 * ��������
 * ����ֵ����
 * ��ע:
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
	
  //�۾��Ͷ���ƹ�
	for(i=0;i<2;i++)
	{
		for(j=0;j<3;j++)
		{
			 if(_head_tim==0)
			 {
				 if(_head_rgb_ds_ls[i][j]!=0)
				 {
						head_lamplight_led_no_off(i,j,1);//�򿪶�Ӧ��
				 }
			 }
			 else if(_head_rgb_ds_ls[i][j]<=_head_tim)
			 {
				 head_lamplight_led_no_off(i,j,0);//�رն�Ӧ��
			 }
		}
	}	
  //�۾��ƹ���ʾ
	for(i=2;i<10;i++)
	{
		for(j=0;j<3;j++)
		{
			 if(_head_tim==0)
			 {
				 if(_head_rgb_ds_ls[i][j]!=0)
				 {
						eye_data |=(0x00000001<<((i-2)*3+j));//�򿪶�Ӧ��
				 }
			 }
			 else if(_head_rgb_ds_ls[i][j]<=_head_tim)
			 {
				 eye_data &=(~(0x00000001<<((i-2)*3+j)));//�رն�Ӧ��
			 }
		}
	}	
	head_eye_transpose(eye_data);//LED����
	
	_head_tim+=8;
	if(_head_tim>=248)
	{
		_head_tim=0;
	}
}






