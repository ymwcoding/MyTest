#ifndef __CRC_LOOPUP_H
#define __CRC_LOOPUP_H	 
#include <stm32f10x.h>



uint16_t crc16_lookup(uint8_t *data, uint16_t length);
uint16_t crc16_count(uint8_t *data, uint16_t length);

#endif
