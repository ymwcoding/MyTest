#include "usart3.h"
#include "app_init.h"
#include "delay.h"
#include "watch_dog.h"
#include "eye_led_control.h"
#include "top_ear_led_control.h"

#define Version  310

int main(void)
{
	app_init();
	while(1)
	{
		eye_led_control_main();
		top_ear_led_control_main(TOP);
		top_ear_led_control_main(EAR);
		watchdog_feed();
	}
}






