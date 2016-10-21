#ifndef _LED_H_
#define _LED_H_
#include "io_bit.h"


#define BEEP PFout(8)
#define LED5 PFout(9)
#define LED6 PFout(10)
//#define LED4 PCout(0)
void led_init(void);
#endif

