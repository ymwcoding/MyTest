#ifndef _CMD_EXECUTE_H__
#define _CMD_EXECUTE_H__
#include <stm32f10x.h>


enum
{
	EYE_CONTROL_CMD,       //�ǿ����۾���ָ��
	EAR_CONTROL_CMD,       //�ǿ��ƶ����ָ��
	TOP_CONTROL_CMD,       //�ǿ���ͷ����ָ��
	READ_DATA_CMD,         //��ȡͷ��һЩ����
	XF_SOURCE_CMD,         //Ѷ����������ָ��
	GET_VERSION_CMD,       //��ȡ�汾��ָ��
	IN_BOOT_CONTROL,       //���ƽ���bootloaderָ��
};


/*����ͨ�Žṹ��*/
typedef struct
{
    uint16_t target_id;   //Ŀ���豸ID
    uint16_t my_id;       //��ǰ�豸ID
    uint16_t serial_number;//���к�
    uint16_t commd_type;  //��������
    uint16_t commd_naber; //������
    uint8_t  *databuffer; //���ͻ�������ָ��
    uint16_t buffer_light;//���ͻ��泤��
} CMD_DATA;






void cmd_execute(CMD_DATA * receive_cmd);








#endif
