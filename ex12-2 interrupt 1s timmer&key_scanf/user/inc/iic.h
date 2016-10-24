#ifndef _IIC_H_
#define _IIC_H_
#include "stm32f4xx.h"
#include "io_bit.h"

#define IIC_SCL 		PBout(8)
#define IIC_SDAIN 	PBin(9)
#define IIC_SDAOUT 	PBout(9)

void iic_init(void);

void iic_start(void);
void iic_stop(void);
void iic_write_byte(u8 _data);
u8 	 iic_read_byte(void);
void iic_put_ack(u8 ack);
u8   iic_get_ack(void);

#endif


