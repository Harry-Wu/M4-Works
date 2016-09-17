#ifndef _AT24C02_H_
#define _AT24C02_H_
#include "stm32f4xx.h" 
//#include "MyTypeDef.h"

#define AT24C02_ADDR   0XA0
#define ACK_OK  0    //正常
#define ACK1		1   //没有从设备响应
#define ACK2		2   //器件内部地址无效
#define ACK3		3   //写数据错误
#define ACK4		4   //重复起始后发器件地址无响应


void at24c02_init(void);
u8 at24c02_write_byte(u16 innter,u8 _data);
u8 at24c02_read_byte(u16 innter,u8 *p);
u8 at24c02_read_bytes(u16 innter,u8 num,u8 *p);
u8 at24c02_write_bytes(u16 innter,u8 num,u8 *p);
#endif



