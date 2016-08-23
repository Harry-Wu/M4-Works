#ifndef _UART_H_
#define _UART_H_
#include "stm32f4xx.h"
#include "io_bit.h"
#include "MyTypeDef.h"


extern u8 rec_buf[100];   //接收缓冲器
extern u8 rec_sta;       //接收完成与否标志 ，rec_sta=1，接收完成    
extern u8 rec_num;       //接收计数


void uart1_init(u32 bond);
void uart_send_string(u8 *p);

#endif



