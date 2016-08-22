#ifndef _UART_H_
#define _UART_H_
#include "stm32f4xx.h"
#include "io_bit.h"


extern uint8_t rec_buf[100];   //���ջ�����
extern uint8_t rec_sta;       //�����������־ ��rec_sta=1���������    
extern uint8_t rec_num;       //���ռ���


void uart1_init(uint32_t bond);
void uart_send_string(uint8_t *p);

#endif



