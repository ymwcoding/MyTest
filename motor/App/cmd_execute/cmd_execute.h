#ifndef _CMD_EXECUTE_H__
#define _CMD_EXECUTE_H__
#include <stm32f10x.h>


enum
{
	EYE_CONTROL_CMD,       //是控制眼睛灯指令
	EAR_CONTROL_CMD,       //是控制耳朵灯指令
	TOP_CONTROL_CMD,       //是控制头部灯指令
	READ_DATA_CMD,         //获取头部一些参数
	XF_SOURCE_CMD,         //讯飞语音控制指令
	GET_VERSION_CMD,       //获取版本号指令
	IN_BOOT_CONTROL,       //控制进入bootloader指令
};


/*数据通信结构体*/
typedef struct
{
    uint16_t target_id;   //目标设备ID
    uint16_t my_id;       //当前设备ID
    uint16_t serial_number;//序列号
    uint16_t commd_type;  //命令类型
    uint16_t commd_naber; //命令编号
    uint8_t  *databuffer; //发送缓存数据指针
    uint16_t buffer_light;//发送缓存长度
} CMD_DATA;






void cmd_execute(CMD_DATA * receive_cmd);








#endif
