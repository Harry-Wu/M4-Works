#ifndef _AT24C02_H_
#define _AT24C02_H_
#include "stm32f4xx.h" 
//#include "MyTypeDef.h"

#define AT24C02_ADDR   0XA0
#define ACK_OK  0    //����
#define ACK1		1   //û�д��豸��Ӧ
#define ACK2		2   //�����ڲ���ַ��Ч
#define ACK3		3   //д���ݴ���
#define ACK4		4   //�ظ���ʼ��������ַ����Ӧ


void at24c02_init(void);
u8 at24c02_write_byte(u16 innter,u8 _data);
u8 at24c02_read_byte(u16 innter,u8 *p);
u8 at24c02_read_bytes(u16 innter,u8 num,u8 *p);
u8 at24c02_write_bytes(u16 innter,u8 num,u8 *p);
#endif



