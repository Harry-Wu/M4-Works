#ifndef _UART_H_
#define _UART_H_


#include "stm32f4xx.h"
//#include "MyTypeDef.h"


extern u8 rec_buf[100];   //���ջ�����
extern u8 rec_sta;      //������ɱ�־rec_sta=1�����������
extern u8 rec_num;
extern u32 rec_count_time;
extern u8 rec_start;


void uart1_init(u32 bond);
void uart_send_string(u8 *p);
	
#endif
