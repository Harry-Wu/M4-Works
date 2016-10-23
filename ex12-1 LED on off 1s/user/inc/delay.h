#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f4xx.h"

#define SYSCLK 168

void delay_us(u32 nus);
void delay_ms(u32 nms);
void systick_interrupt_init(u32 nms);
#endif


