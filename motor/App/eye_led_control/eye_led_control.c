#include "eye_led_control.h"
#include "delay.h"
#include "string.h"
#include "eye_led.h"

#define EYE_LED_COUNT   8                     //�۾�led������

uint8_t eyeLedMarquee[3] = {0};               //�۾�����Ʊ���
uint8_t eyeLedRgbData[EYE_LED_COUNT][3] = {0};//�۾���rgb����
 uint8_t eyeLedMode = 0;                       //�۾���ģʽ
 uint8_t eyeLedRgbDs[EYE_LED_COUNT][3]={0};    //��ʱ����
 uint8_t eyeLedRgbDsLs[EYE_LED_COUNT][3]={0};    //��ʱ����


static uint8_t eyeLedTimeCount = 0;


/******************************************************************
 * ��������:���ݲ��������۾���
 * ��������
 * ����ֵ����
 * ��ע:
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
���ܣ�գ��һ��
���룺��
�������
���÷�ʽ;�ⲿ����
*****************************************/
static void eye_led_blink(void)
{
	static const uint8_t offset[EYE_LED_COUNT] = {7,7,7,3,2,1,2,3};//�̶���ƫ����
	uint8_t i = 0,j = 0;
  uint8_t led_lings[EYE_LED_COUNT][3] = {0};

/*------------------------------------------------------*/

  memcpy(&led_lings[0][0],&eyeLedRgbDs[0][0],24);//��������
	
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
	memcpy(&eyeLedRgbDs[0][0],&led_lings[0][0],24);//��������
}	



/****************************************
���ܣ������
���룺led_r��led_g��led_b:����ƵƵ�rgb��ɫ
�������
���÷�ʽ;�ⲿѭ�����ã������������Ϊ50ms
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
 * ��������:led�۾���ɫ���䣨ע����ѭ����
 * ��������
 * ����ֵ����
 * ��ע:
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
 * ��������:�۾�led����������
 * ��������
 * ����ֵ����
 * ��ע:
 * ģʽ0��ȫ��ģʽ
 * ģʽ1��գ��ģʽ
 * ģʽ2���ȴ�ģʽ
 * ģʽ3��ɫ�������任ģʽ
 ******************************************************************/
void eye_led_control_main(void)
{
	memcpy(&eyeLedRgbDs[0][0],&eyeLedRgbData[0][0],24);//�����ݸ��Ƴ���
	switch(eyeLedMode)
	{
		case 0://ֱ����ʾ������ɫ
		{
			delay_ms(50);
		}break;
			
		case 1://����ɫ�ʱ任ģʽ
		{
			eye_led_graduated_colors();
		}break;
		
		case 2://գ��
		{
			eye_led_blink();
			eyeLedMode=0;
		}break;
		
		case 3://�۾��ȴ�ģʽ
		{
			eye_led_marquee(eyeLedMarquee[0],eyeLedMarquee[1],eyeLedMarquee[2]);
		}break;
		
		default:break;
	}
}	


/*
**�۾�LED�ƶ�ʱ������
*/
void eye_led_timer_task(void)
{
	static uint32_t eye_data=0;
	uint8_t i = 0,j = 0;
	if(eyeLedTimeCount == 0)
	{
		memcpy(eyeLedRgbDsLs,eyeLedRgbDs,24);
	}
	//�۾��ƹ���ʾ
	for(i=0;i<EYE_LED_COUNT;i++)
	{
		for(j=0;j<3;j++)
		{
			 if(eyeLedTimeCount==0)
			 {
				 if(eyeLedRgbDsLs[i][j]!=0)
				 {
						eye_data |=(0x00000001<<((i-2)*3+j));//�򿪶�Ӧ��
				 }
			 }
			 else if(eyeLedRgbDsLs[i][j] <= eyeLedTimeCount)
			 {
				 eye_data &=(~(0x00000001<<((i-2)*3+j)));//�رն�Ӧ��
			 }
		}
	}	
	head_eye_transpose(eye_data);//LED����
	eyeLedTimeCount+=8;
	if(eyeLedTimeCount>=248)
	{
		eyeLedTimeCount=0;
	}
	
}

