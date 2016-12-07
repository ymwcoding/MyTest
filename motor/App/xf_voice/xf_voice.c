#include "xf_voice.h"
#include "stdlib.h"
#include "usart3.h"
#include "my_types.h"

#define MIC_NABER 4  //4个mic接口

static uint8_t  headAngleHeat=0;//头部唤醒标志
static char     xfUsartReceiveBuffer[3]={0};//音源串口接收缓存
static uint16_t headSoundAngle=0;//音源角度


/*
**获取讯飞语音唤醒和角度数据
*/
void xf_voice_get_data(uint8_t* get_data)
{
	*(get_data)   = LO_UINT16_T(headSoundAngle);
	*(get_data+1) = HI_UINT16_T(headSoundAngle);
	*(get_data+2) = headAngleHeat & 0x01;	
	
	headAngleHeat = 0;
}


/******************************************************************
 * 函数介绍:设置科大讯飞波束成形
 * 参数：无
 * 返回值：无
 * 备注:
 ******************************************************************/
void xf_voice_set_angle(uint8_t anglenaber)
{
	char senddata[7]={0};
	
	if(anglenaber>=MIC_NABER) return;//如果输入的参数大于麦克风数则不响应
  sprintf(senddata,"BEAM %d\n",anglenaber);

  usart3_send_buf((uint8_t*)senddata,7);
}



/******************************************************************
 * 函数介绍:串口中断接收
 * 参数：无
 * 返回值：无
 * 备注:
 ******************************************************************/
void xf_usart_receive_deal(uint8_t r_data)
{
	static uint8_t  headUsartMode=0;//音源串口接收模式
	uint16_t angle = 0;
	
	switch(headUsartMode)
	{
		case 0:{if(r_data=='a'){headUsartMode=1;}}break;
		case 1:{if(r_data=='n'){headUsartMode++;}else{headUsartMode=0;}}break;
		case 2:{if(r_data=='g'){headUsartMode++;}else{headUsartMode=0;}}break;		
		case 3:{if(r_data=='l'){headUsartMode++;}else{headUsartMode=0;}}break;		
		case 4:{if(r_data=='e'){headUsartMode++;}else{headUsartMode=0;}}break;		
		case 5:{if(r_data==':'){headUsartMode++;}else{headUsartMode=0;}}break;	
		case 6:{headUsartMode++; xfUsartReceiveBuffer[0]=(char)r_data;}break;	
		case 7:{headUsartMode++; xfUsartReceiveBuffer[1]=(char)r_data;}break;
		case 8:{ headUsartMode=0;xfUsartReceiveBuffer[2]=(char)r_data; 
			angle=atoi(xfUsartReceiveBuffer);
			if(angle<=360)
			{
				headSoundAngle = angle;
			}
			xf_voice_set_angle(3);
    	headAngleHeat=1;//头部唤醒标志
		}break;
	}
}
