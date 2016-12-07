#ifndef _CONFIG_H__
#define _CONFIG_H__
#include <stm32f10x.h>


#define FIRMWARE_VERSION        310   //固件版本号          


#define CMD_FRAME_HEADER_L  0xB7  //帧头L
#define CMD_FRAME_HEADER_H  0x8C  //帧头H
#define CMD_FRAME_TAIL_L    0xAC  //帧尾L
#define CMD_FRAME_TAIL_H    0xBD  //帧尾H




#define MY_DEVICE_ID  (0x0400) //头部灯光效果控制模块ID
#define ALL_DEVICE_ID (0x00FF) //全部设备ID

#define MY_TYPE_ID    (0x0004)

#define HEAD_CMD_NABER  4




#endif
