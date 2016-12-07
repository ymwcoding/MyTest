#ifndef _WATCH_DOG_H__
#define _WATCH_DOG_H__
#include <stm32f10x.h>

void watchdog_enable(void);
void watchdog_disable(void);
void watchdog_feed(void);
void watchdog_init(void);




#endif
