#ifndef _UART_H_
#define _UART_H_
#include "stm32f4xx.h"
#include "io_bit.h"
#include "MyTypeDef.h"


extern u8 rec_buf[100];   //���ջ�����
extern u8 rec_sta;       //�����������־ ��rec_sta=1���������    
extern u8 rec_num;       //���ռ���


void uart1_init(u32 bond);
void uart_send_string(u8 *p);

#endif



