#ifndef _LED_H_
#define _LED_H_
#include "stm32f4xx.h"
#include "io_bit.h"

#define LED1 PFout(6)
#define LED2 PFout(9)
#define LED3 PFout(10)
#define LED4 PCout(0)

void led_init(void);
#endif

