#ifndef _USART_H_
#define _USART_H_

#include "stm32f4xx.h"

void uart1_init(u32 bond);
void uart_send_string(u8 *p);


#endif
