#ifndef _AT24C02_H_
#define _AT24C02_H_
#include "stm32f4xx.h" 
#define AT24C02_ADDR    0XA1    //Æ÷¼þµØÖ·

#define AT24C02_NOERR   0
#define AT24C02_ERR1    1
#define AT24C02_ERR2    2
#define AT24C02_ERR3    3
#define AT24C02_ERR4    4
#define AT24C02_ERR5    5
#define AT24C02_ERR6    6

#define   START_TIME     0X90


void at24c04_init(void);
u8 at24c02_write_byte(u16 inner_addr,u8 _data);
u8 at24c02_read_byte(u16 inner_addr,u8 *p);
u8 at24c02_read_bytes(u16 inner_addr,u8 *p,u16 num);
u8 at24c02_write_bytes(u16 inner_addr,u8 *p,u16 num);
#endif


