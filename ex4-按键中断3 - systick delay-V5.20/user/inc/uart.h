#ifndef _UART_H_
#define _UART_H_
#include "stm32f4xx.h"
#include "io_bit.h"


extern uint8_t rec_buf[100];   //接收缓冲器
extern uint8_t rec_sta;       //接收完成与否标志 ，rec_sta=1，接收完成    
extern uint8_t rec_num;       //接收计数


void uart1_init(uint32_t bond);
void uart_send_string(uint8_t *p);

#endif



