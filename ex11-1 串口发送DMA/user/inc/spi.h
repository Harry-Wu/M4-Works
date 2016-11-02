#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f4xx.h" 
//#include "MyTypeDef.h"

#define SPI_MOSI    PBout(5)
#define SPI_MISO	PBin(4)
#define SPI_SCK		PBout(3)



void spi_init(void);
u8 SPI1_ReadWriteByte(u8 _data);
#endif

