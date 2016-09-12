#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f4xx.h" 
#define SYSCLK 168   //sys时钟为168M

void delay_us(u32 n); //延时n微秒
void delay_ms(u32 n); //延时n毫秒

#endif


