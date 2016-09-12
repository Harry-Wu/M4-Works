#ifndef _SPI_H_
#define _SPI_H_
#include "stm32f4xx.h" 
#include "io_bit.h"

//#define CS   PBout(12)
//#define SCK  PBout(3)
//#define MISO  PBin(4)
//#define MOSI  PBout(5)
void SPI1_Init(void);
u8 SPI1_ReadWriteByte(u8 _data);


#endif

