#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f4xx.h" 
#define SYSCLK 168   //sysʱ��Ϊ168M

void delay_us(u32 n); //��ʱn΢��
void delay_ms(u32 n); //��ʱn����

#endif


