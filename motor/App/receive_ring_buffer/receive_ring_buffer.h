#ifndef _RECEIVE_RING_BUFFER_H__
#define _RECEIVE_RING_BUFFER_H__
#include <stm32f10x.h>


typedef void(*FuncP)();//一个无输入无返回的函数指针声明


typedef struct {
    uint16_t     byteSize;        //总共的字节数
    uint16_t     readIdx;         //读的标号
    uint16_t     writeIdx;        //写标号
	  uint16_t     cmdDataSaveCount;//已存在的命令数
    uint8_t      *data_ptr;       //数据缓冲
	  uint8_t      bufferId;        //标记buffer的个数
	  uint8_t      state;           //进行寻找帧头帧尾的状态机值
	  FuncP        startDecodeFunc; //开始解码的函数入口   
}ReceiveRingBuffer;

void receive_ring_buffer_check_uinit_data(ReceiveRingBuffer* buffer,uint8_t* wrdata, uint16_t len);
void receive_ring_buffer_write_items(ReceiveRingBuffer* buffer, uint8_t* wrdata, uint8_t write_amount);
void receive_ring_buffer_read_items(ReceiveRingBuffer* buffer, uint8_t* rddata, uint8_t read_amount,uint16_t read_index);

#endif


