#ifndef _AT24C02_H_
#define _AT24C02_H_
#include "stm32f4xx.h"

#define AT24C02_ADDR 0XA0

#define  NO_ERROR   0
#define  ERROR_1    1
#define  ERROR_2    2
#define  ERROR_3    3
#define  ERROR_4    4
#define  ERROR_5    5
#define  ERROR_6    6

void at24c02_init(void);
u8 at24c02_write_byte(u16 innter_addr,u8 _data);
u8 at24c02_read_byte(u16 innter_addr,u8 *p);
u8 at24c02_read_bytes(u16 innter_addr,u16 num,u8 *p);
u8 at24c02_write_bytes(u16 innter_addr,u16 num,u8 *p);
#endif


