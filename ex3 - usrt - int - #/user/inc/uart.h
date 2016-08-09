#ifndef _USART_H_
#define _USART_H_

#include "stm32f4xx.h"

extern u8 rec_buf[100];   //接收缓冲区
extern u8 rec_sta;      //接收完成标志rec_sta=1表明接收完成
extern u8 rec_num;

void uart1_init(u32 bond);
void uart_send_string(u8 *p);

#endif
