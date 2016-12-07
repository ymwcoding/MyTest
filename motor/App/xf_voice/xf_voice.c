#include "xf_voice.h"
#include "stdlib.h"
#include "usart3.h"
#include "my_types.h"

#define MIC_NABER 4  //4��mic�ӿ�

static uint8_t  headAngleHeat=0;//ͷ�����ѱ�־
static char     xfUsartReceiveBuffer[3]={0};//��Դ���ڽ��ջ���
static uint16_t headSoundAngle=0;//��Դ�Ƕ�


/*
**��ȡѶ���������ѺͽǶ�����
*/
void xf_voice_get_data(uint8_t* get_data)
{
	*(get_data)   = LO_UINT16_T(headSoundAngle);
	*(get_data+1) = HI_UINT16_T(headSoundAngle);
	*(get_data+2) = headAngleHeat & 0x01;	
	
	headAngleHeat = 0;
}


/******************************************************************
 * ��������:���ÿƴ�Ѷ�ɲ�������
 * ��������
 * ����ֵ����
 * ��ע:
 ******************************************************************/
void xf_voice_set_angle(uint8_t anglenaber)
{
	char senddata[7]={0};
	
	if(anglenaber>=MIC_NABER) return;//�������Ĳ���������˷�������Ӧ
  sprintf(senddata,"BEAM %d\n",anglenaber);

  usart3_send_buf((uint8_t*)senddata,7);
}



/******************************************************************
 * ��������:�����жϽ���
 * ��������
 * ����ֵ����
 * ��ע:
 ******************************************************************/
void xf_usart_receive_deal(uint8_t r_data)
{
	static uint8_t  headUsartMode=0;//��Դ���ڽ���ģʽ
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
    	headAngleHeat=1;//ͷ�����ѱ�־
		}break;
	}
}
