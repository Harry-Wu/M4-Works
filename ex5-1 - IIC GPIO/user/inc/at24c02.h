#ifndef _AT24C02_H_
#define _AT24C02_H_
#include "stm32f4xx.h"

#define AT24C02_ADDR 0xA0
#define ACK_OK 0;  //����
#define ACK_NO_SLAVE 1; //û�д��豸��Ӧ
#define ACK_ADDR_ERROR 2; //�����ڲ���ַ��Ч
#define ACK_WRITE_ERROR 3;  //д���ݴ���
#define ACK_NO_SLAVE2 4; //�ظ�����ʼ��û�д��豸��Ӧ

void at24c02_init(void);
u8 at24c02_write_byte(u16 addr,u8 _data);
u8 at24c02_read_byte(u16 addr,u8 *p);
u8 at24c02_read_bytes(u16 addr, u8 num, u8 *p);

#endif
