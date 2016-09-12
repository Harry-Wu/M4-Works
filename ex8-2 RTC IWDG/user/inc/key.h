#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f4xx.h" 
#include "io_bit.h"

#define NO_KEY 0Xff
#define KEY0_OK   		0
#define KEY1_OK   		1
#define KEY2_OK  			2
#define WKUP_KEY_OK   3

#define KEY2 			PEin(2)
#define KEY1 			PEin(3)
#define KEY0 			PEin(4)
#define WKUP_KEY 	PAin(0)

void key_init(void);
u8 key_scanf(void);

#endif


