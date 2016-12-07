#include "can_receive.h"
#include "cmd_analyze.h"
#include "timer2.h"
#include "my_types.h"
#include "cmd_execute.h"
#include "config.h"


ReceiveRingBuffer canReceiveRingBuffer;                //CAN�������ݻ���ṹ��
static uint8_t  canReceiveData[CAN_RX_MAX_LEN]={0};    //���ݽ��ջ���
uint8_t  canReceiveOkCmdBuffer[CAN_RX_MAX_LEN+1] = {0};//���յ�����ȷ�������
uint16_t canReceiveOkCmdLen = 0;                       //���յ�����ȷ������ܳ���

/*
**����can���ݽ���
*/
static void can_start_decode(void)
{
	if(FlagBit.flag.canStartDecodeFlag == 0)//��û����������
	{
		timer2_start();                       //������ʱ��3��ʼ����
		FlagBit.flag.canStartDecodeFlag = 1;
	}
}

/*
**����can���ݽ��յĻ��λ���
*/
void can_receive_ring_buffer_create(void)
{
	  canReceiveRingBuffer.byteSize  = CAN_RX_MAX_LEN;//���������ֽ���
    canReceiveRingBuffer.readIdx = 0;//��ʼ��ȡ�ĵ�ַ
    canReceiveRingBuffer.writeIdx = 0;//д�ĵ�ַ
    canReceiveRingBuffer.data_ptr = canReceiveData;//���ݻ���ָ��
	  canReceiveRingBuffer.bufferId = 2;                      //�����ţ�����ʶ�����ĸ����λ���
	  canReceiveRingBuffer.cmdDataSaveCount = 0;
	  canReceiveRingBuffer.startDecodeFunc = can_start_decode;//��ʼ��Ԫ���ݽ�������ں���
	  canReceiveRingBuffer.state = 0;
}



/*********************************************************************************
 * ����  :�жϽ��մ������
 * ����  :���յ����ж�����
 * ���  :��
 * ˵��  :USB�ϴ�һ�������ݻ��Զ���һ����������0x00����Ӧ�˳�
*********************************************************************************/
void can_data_receive(CanRxMsg RxMessage)
{
	if(((uint16_t)(RxMessage.StdId)==MY_DEVICE_ID)||((uint16_t)(RxMessage.StdId)==ALL_DEVICE_ID))
	{
		cmd_check_head_tail(&canReceiveRingBuffer,RxMessage.Data,RxMessage.DLC);
	}
	
}
/*------------------------------------------------------------------------------*/


/*
**can��ʼ������յ�������
*/
void can_decode_receive(void)
{
	timer2_stop();                 //�����Ͱѽ��յ����ݽ���
	while(canReceiveRingBuffer.cmdDataSaveCount)
	{
		canReceiveRingBuffer.cmdDataSaveCount = 0;
		get_cmd(&canReceiveRingBuffer);//can����canReceiveRingBuffer�е�����
	}
	FlagBit.flag.canStartDecodeFlag = 0;//������ſ������¿�������
}



