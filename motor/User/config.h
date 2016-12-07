#ifndef _CONFIG_H__
#define _CONFIG_H__
#include <stm32f10x.h>


#define FIRMWARE_VERSION        310   //�̼��汾��          


#define CMD_FRAME_HEADER_L  0xB7  //֡ͷL
#define CMD_FRAME_HEADER_H  0x8C  //֡ͷH
#define CMD_FRAME_TAIL_L    0xAC  //֡βL
#define CMD_FRAME_TAIL_H    0xBD  //֡βH




#define MY_DEVICE_ID  (0x0400) //ͷ���ƹ�Ч������ģ��ID
#define ALL_DEVICE_ID (0x00FF) //ȫ���豸ID

#define MY_TYPE_ID    (0x0004)

#define HEAD_CMD_NABER  4




#endif
