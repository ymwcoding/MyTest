#ifndef _RECEIVE_RING_BUFFER_H__
#define _RECEIVE_RING_BUFFER_H__
#include <stm32f10x.h>


typedef void(*FuncP)();//һ���������޷��صĺ���ָ������


typedef struct {
    uint16_t     byteSize;        //�ܹ����ֽ���
    uint16_t     readIdx;         //���ı��
    uint16_t     writeIdx;        //д���
	  uint16_t     cmdDataSaveCount;//�Ѵ��ڵ�������
    uint8_t      *data_ptr;       //���ݻ���
	  uint8_t      bufferId;        //���buffer�ĸ���
	  uint8_t      state;           //����Ѱ��֡ͷ֡β��״̬��ֵ
	  FuncP        startDecodeFunc; //��ʼ����ĺ������   
}ReceiveRingBuffer;

void receive_ring_buffer_check_uinit_data(ReceiveRingBuffer* buffer,uint8_t* wrdata, uint16_t len);
void receive_ring_buffer_write_items(ReceiveRingBuffer* buffer, uint8_t* wrdata, uint8_t write_amount);
void receive_ring_buffer_read_items(ReceiveRingBuffer* buffer, uint8_t* rddata, uint8_t read_amount,uint16_t read_index);

#endif


