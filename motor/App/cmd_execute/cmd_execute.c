#include "cmd_execute.h"
#include "top_ear_led_control.h"
#include "eye_led_control.h"
#include "can_send.h"
#include "touch_key.h"
#include "xf_voice.h"
#include "config.h"
#include "my_types.h"

/*
**�ظ��������
*/
void cmd_reply_data_fill(CMD_DATA* cmd_repley_data,CMD_DATA* receive_cmd)
{
	cmd_repley_data->target_id     = receive_cmd->my_id;        //Ŀ���豸��ַ
	cmd_repley_data->my_id         = MY_DEVICE_ID;              //����ID
	cmd_repley_data->serial_number = receive_cmd->serial_number;//���к�
	cmd_repley_data->commd_type    = receive_cmd->commd_type;   //��������
	cmd_repley_data->commd_naber   = receive_cmd->commd_naber;  //������
}



/*
**�۾��ƿ���
*/
void eye_led_execute(CMD_DATA* receive_cmd)
{
	uint8_t returnmode=1;
	uint8_t mode;
	uint8_t *controldata;
	
	CMD_DATA cmd_repley_data;
	
	if(receive_cmd->buffer_light != 25) return;       //������������ݲ�ͬ���򷵻�
	cmd_reply_data_fill(&cmd_repley_data,receive_cmd);//�����ͬ����
	cmd_repley_data.databuffer    = &returnmode;      //���ͻ�������ָ��
	cmd_repley_data.buffer_light  = 1;                //���ͻ��泤��
	if(cmd_repley_data.target_id !=ALL_DEVICE_ID)
	{
		cmd_send_pack(&cmd_repley_data);//���ͷ�������
	}		
	mode = receive_cmd->databuffer[0];//�۾���ģʽ
	controldata = &receive_cmd->databuffer[1];//�Ƶ�rgb
	eye_led_control(mode,controldata,24);//����ͷ���Ƶ���	
	
}


/*
**����ƿ���
*/
void ear_led_execute(CMD_DATA* receive_cmd)
{
	uint8_t returnmode=1;
	uint8_t mode;
	uint8_t *controldata;
	CMD_DATA cmd_repley_data;
	
	if(receive_cmd->buffer_light != 4) return;       //������������ݲ�ͬ���򷵻�
	cmd_reply_data_fill(&cmd_repley_data,receive_cmd);//�����ͬ����
	cmd_repley_data.databuffer    = &returnmode;      //���ͻ�������ָ��
	cmd_repley_data.buffer_light  = 1;                //���ͻ��泤��
	
	if(receive_cmd->target_id !=ALL_DEVICE_ID)  
	{
		cmd_send_pack(&cmd_repley_data);                //���ͷ�������
	}
			
	mode = receive_cmd->databuffer[0];                //�����ģʽ
	controldata = &receive_cmd->databuffer[1];        //�Ƶ�rgb
	top_ear_led_control(mode,controldata,3,EAR);              //���ƶ������
}


/*
**ͷ���ƿ���
*/
void top_led_execute(CMD_DATA* receive_cmd)
{
	uint8_t returnmode=1;
	uint8_t mode;
	uint8_t *controldata;
	CMD_DATA cmd_repley_data;
	
	if(receive_cmd->buffer_light != 4) return;         //������������ݲ�ͬ���򷵻�
	cmd_reply_data_fill(&cmd_repley_data,receive_cmd); //�����ͬ����
	cmd_repley_data.databuffer    = &returnmode;       //���ͻ�������ָ��
	cmd_repley_data.buffer_light  = 1;                 //���ͻ��泤��
	
	if(receive_cmd->target_id !=ALL_DEVICE_ID) 
	{
		 cmd_send_pack(&cmd_repley_data);                //���ͷ�������
	}
	
  //����ͷ���Ƶ���
	mode = receive_cmd->databuffer[0];                 //ͷ����ģʽ
	controldata = &receive_cmd->databuffer[1];         //�Ƶ�rgb
	top_ear_led_control(mode,controldata,3,TOP);               //����ͷ���Ƶ���
}


/*
**ͷ��������ȡ
*/
void read_data(CMD_DATA* receive_cmd)
{
	uint8_t return_data[5]={0};
	CMD_DATA cmd_repley_data;

	if(receive_cmd -> buffer_light != 0) return;//������������ݲ�ͬ���򷵻�
  cmd_reply_data_fill(&cmd_repley_data,receive_cmd);
  cmd_repley_data.databuffer    = return_data;//���ͻ�������ָ��
	cmd_repley_data.buffer_light  = 5;          //���ͻ��泤��
  
  xf_voice_get_data(return_data);             //��ȡѶ����������
  get_touch_key_state(&return_data[3]);       //��ȡ��������

  if(receive_cmd->target_id !=ALL_DEVICE_ID) 
	{
		cmd_send_pack(&cmd_repley_data);          //���ͷ�������
	}
}


/*
**������Դ
*/
void set_sound_source_angle(CMD_DATA* receive_cmd)
{
	
}


/*
**���ƽ���bootloader
*/
void control_in_boot(CMD_DATA* receive_cmd)
{
	uint8_t return_data = 0x06;
	CMD_DATA cmd_repley_data;
	if(receive_cmd -> buffer_light != 0) return;//������������ݲ�ͬ���򷵻�
	cmd_reply_data_fill(&cmd_repley_data,receive_cmd);
	cmd_repley_data.databuffer    = &return_data;       //���ͻ�������ָ��
	cmd_repley_data.buffer_light  = 1;                  //���ͻ��泤��
	
	if(receive_cmd->target_id !=ALL_DEVICE_ID) 
	{
		 cmd_send_pack(&cmd_repley_data);                //���ͷ�������
	}
	                                                   //��������ģʽ
	
}


/*
**��ȡ�̼��汾��
*/
void read_version(CMD_DATA* receive_cmd)
{
	uint8_t return_data[2]={0};
	CMD_DATA cmd_repley_data;
	if(receive_cmd -> buffer_light != 0) return;//������������ݲ�ͬ���򷵻�
	cmd_reply_data_fill(&cmd_repley_data,receive_cmd);
	cmd_repley_data.databuffer    = return_data;       //���ͻ�������ָ��
	cmd_repley_data.buffer_light  = 2;                 //���ͻ��泤��

  return_data[0] = LO_UINT16_T(FIRMWARE_VERSION);   //��ȡ�汾��
  return_data[1] = HI_UINT16_T(FIRMWARE_VERSION);
	
	if(receive_cmd->target_id !=ALL_DEVICE_ID) 
	{
		 cmd_send_pack(&cmd_repley_data);                //���ͷ�������
	}
}




/*
**����ָ��ִ��
*/
void cmd_execute(CMD_DATA * receive_cmd)
{
	if((receive_cmd -> target_id == MY_DEVICE_ID) || (receive_cmd -> target_id == ALL_DEVICE_ID))
	{
		if(receive_cmd -> commd_type == MY_TYPE_ID)
		{
				switch(receive_cmd -> commd_naber)//����ָ֡��ִ��
				{
					case EYE_CONTROL_CMD://�����۾�ָ��
						eye_led_execute(receive_cmd);
					break;
					
					case EAR_CONTROL_CMD://���ƶ���ָ��
						ear_led_execute(receive_cmd);
					break;
					
					case TOP_CONTROL_CMD://����ͷ��LED
						top_led_execute(receive_cmd);
					break;
					
					case READ_DATA_CMD: //��ȡ����
						read_data(receive_cmd);
					break;
					
					case XF_SOURCE_CMD:
						
					break;
					
					case GET_VERSION_CMD://��ȡ�汾��
						read_version(receive_cmd);
					break;
					
					case IN_BOOT_CONTROL://����boot
						control_in_boot(receive_cmd);
					break;
					default:break;
				}
		}
	}
}


