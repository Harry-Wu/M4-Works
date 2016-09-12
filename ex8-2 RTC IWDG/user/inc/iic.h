#ifndef _IIC_H_
#define _IIC_H_
#include "io_bit.h" 
#include "stm32f4xx.h"

#define  SDAOUT		PBout(9)
#define  SDAIN		PBin(9)
#define  SCL  		PBout(8)

void iic_init(void);
void iic_start(void);
void iic_stop(void);
void iic_write_byte(u8 send_data);
u8 iic_read_byte(void);
u8 iic_get_ack(void);
void iic_put_ack(u8 ack);

#endif


