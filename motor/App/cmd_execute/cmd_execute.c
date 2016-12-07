#include "cmd_execute.h"
#include "top_ear_led_control.h"
#include "eye_led_control.h"
#include "can_send.h"
#include "touch_key.h"
#include "xf_voice.h"
#include "config.h"
#include "my_types.h"

/*
**回复数据填充
*/
void cmd_reply_data_fill(CMD_DATA* cmd_repley_data,CMD_DATA* receive_cmd)
{
	cmd_repley_data->target_id     = receive_cmd->my_id;        //目标设备地址
	cmd_repley_data->my_id         = MY_DEVICE_ID;              //本机ID
	cmd_repley_data->serial_number = receive_cmd->serial_number;//序列号
	cmd_repley_data->commd_type    = receive_cmd->commd_type;   //命令类型
	cmd_repley_data->commd_naber   = receive_cmd->commd_naber;  //命令编号
}



/*
**眼睛灯控制
*/
void eye_led_execute(CMD_DATA* receive_cmd)
{
	uint8_t returnmode=1;
	uint8_t mode;
	uint8_t *controldata;
	
	CMD_DATA cmd_repley_data;
	
	if(receive_cmd->buffer_light != 25) return;       //如果，命令数据不同，则返回
	cmd_reply_data_fill(&cmd_repley_data,receive_cmd);//填充相同数据
	cmd_repley_data.databuffer    = &returnmode;      //发送缓存数据指针
	cmd_repley_data.buffer_light  = 1;                //发送缓存长度
	if(cmd_repley_data.target_id !=ALL_DEVICE_ID)
	{
		cmd_send_pack(&cmd_repley_data);//发送返回数据
	}		
	mode = receive_cmd->databuffer[0];//眼睛灯模式
	controldata = &receive_cmd->databuffer[1];//灯的rgb
	eye_led_control(mode,controldata,24);//控制头顶灯点亮	
	
}


/*
**耳朵灯控制
*/
void ear_led_execute(CMD_DATA* receive_cmd)
{
	uint8_t returnmode=1;
	uint8_t mode;
	uint8_t *controldata;
	CMD_DATA cmd_repley_data;
	
	if(receive_cmd->buffer_light != 4) return;       //如果，命令数据不同，则返回
	cmd_reply_data_fill(&cmd_repley_data,receive_cmd);//填充相同数据
	cmd_repley_data.databuffer    = &returnmode;      //发送缓存数据指针
	cmd_repley_data.buffer_light  = 1;                //发送缓存长度
	
	if(receive_cmd->target_id !=ALL_DEVICE_ID)  
	{
		cmd_send_pack(&cmd_repley_data);                //发送返回数据
	}
			
	mode = receive_cmd->databuffer[0];                //耳朵灯模式
	controldata = &receive_cmd->databuffer[1];        //灯的rgb
	top_ear_led_control(mode,controldata,3,EAR);              //控制耳朵点亮
}


/*
**头顶灯控制
*/
void top_led_execute(CMD_DATA* receive_cmd)
{
	uint8_t returnmode=1;
	uint8_t mode;
	uint8_t *controldata;
	CMD_DATA cmd_repley_data;
	
	if(receive_cmd->buffer_light != 4) return;         //如果，命令数据不同，则返回
	cmd_reply_data_fill(&cmd_repley_data,receive_cmd); //填充相同数据
	cmd_repley_data.databuffer    = &returnmode;       //发送缓存数据指针
	cmd_repley_data.buffer_light  = 1;                 //发送缓存长度
	
	if(receive_cmd->target_id !=ALL_DEVICE_ID) 
	{
		 cmd_send_pack(&cmd_repley_data);                //发送返回数据
	}
	
  //控制头顶灯点亮
	mode = receive_cmd->databuffer[0];                 //头顶灯模式
	controldata = &receive_cmd->databuffer[1];         //灯的rgb
	top_ear_led_control(mode,controldata,3,TOP);               //控制头顶灯点亮
}


/*
**头部参数获取
*/
void read_data(CMD_DATA* receive_cmd)
{
	uint8_t return_data[5]={0};
	CMD_DATA cmd_repley_data;

	if(receive_cmd -> buffer_light != 0) return;//如果，命令数据不同，则返回
  cmd_reply_data_fill(&cmd_repley_data,receive_cmd);
  cmd_repley_data.databuffer    = return_data;//发送缓存数据指针
	cmd_repley_data.buffer_light  = 5;          //发送缓存长度
  
  xf_voice_get_data(return_data);             //获取讯飞语音数据
  get_touch_key_state(&return_data[3]);       //获取按键数据

  if(receive_cmd->target_id !=ALL_DEVICE_ID) 
	{
		cmd_send_pack(&cmd_repley_data);          //发送返回数据
	}
}


/*
**设置音源
*/
void set_sound_source_angle(CMD_DATA* receive_cmd)
{
	
}


/*
**控制进入bootloader
*/
void control_in_boot(CMD_DATA* receive_cmd)
{
	uint8_t return_data = 0x06;
	CMD_DATA cmd_repley_data;
	if(receive_cmd -> buffer_light != 0) return;//如果，命令数据不同，则返回
	cmd_reply_data_fill(&cmd_repley_data,receive_cmd);
	cmd_repley_data.databuffer    = &return_data;       //发送缓存数据指针
	cmd_repley_data.buffer_light  = 1;                  //发送缓存长度
	
	if(receive_cmd->target_id !=ALL_DEVICE_ID) 
	{
		 cmd_send_pack(&cmd_repley_data);                //发送返回数据
	}
	                                                   //进入升级模式
	
}


/*
**获取固件版本号
*/
void read_version(CMD_DATA* receive_cmd)
{
	uint8_t return_data[2]={0};
	CMD_DATA cmd_repley_data;
	if(receive_cmd -> buffer_light != 0) return;//如果，命令数据不同，则返回
	cmd_reply_data_fill(&cmd_repley_data,receive_cmd);
	cmd_repley_data.databuffer    = return_data;       //发送缓存数据指针
	cmd_repley_data.buffer_light  = 2;                 //发送缓存长度

  return_data[0] = LO_UINT16_T(FIRMWARE_VERSION);   //获取版本号
  return_data[1] = HI_UINT16_T(FIRMWARE_VERSION);
	
	if(receive_cmd->target_id !=ALL_DEVICE_ID) 
	{
		 cmd_send_pack(&cmd_repley_data);                //发送返回数据
	}
}




/*
**根据指令执行
*/
void cmd_execute(CMD_DATA * receive_cmd)
{
	if((receive_cmd -> target_id == MY_DEVICE_ID) || (receive_cmd -> target_id == ALL_DEVICE_ID))
	{
		if(receive_cmd -> commd_type == MY_TYPE_ID)
		{
				switch(receive_cmd -> commd_naber)//根据帧指令执行
				{
					case EYE_CONTROL_CMD://控制眼睛指令
						eye_led_execute(receive_cmd);
					break;
					
					case EAR_CONTROL_CMD://控制耳朵指令
						ear_led_execute(receive_cmd);
					break;
					
					case TOP_CONTROL_CMD://控制头顶LED
						top_led_execute(receive_cmd);
					break;
					
					case READ_DATA_CMD: //读取数据
						read_data(receive_cmd);
					break;
					
					case XF_SOURCE_CMD:
						
					break;
					
					case GET_VERSION_CMD://获取版本号
						read_version(receive_cmd);
					break;
					
					case IN_BOOT_CONTROL://进入boot
						control_in_boot(receive_cmd);
					break;
					default:break;
				}
		}
	}
}


