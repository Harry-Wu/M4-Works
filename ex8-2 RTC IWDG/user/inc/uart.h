#ifndef _USART_H_
#define _USART_H_
#include "stm32f4xx.h" 

extern u8 rec_buf[];   
extern u8 rec_sta;    


void uart_init(u32 bond);
void uart_send_string(char *p);


#endif
