#ifndef _AT24C02_H_
#define _AT24C02_H_
#include "stm32f4xx.h" 
#define AT24C02_ADDR  0XA0

#define  NO_ERR       0
#define  NO_SLAVER_1  1
#define  NO_SLAVER_2  2
#define  NO_SLAVER_3  3

#define  NO_SLAVER_4  4
#define  NO_SLAVER_5  5
#define  NO_SLAVER_6  6


void at24c02_init(void);
u8 at24c02_write_byte(u16 inner_addr,u8 _data );
u8 at24c02_read_byte(u16 inner_addr,u8 *p );


u8 at24c02_write_bytes(u16 inner_addr,u8 *p,u16 num);
u8 at24c02_read_bytes(u16 inner_addr,u8 *p, u16 num);

#endif
