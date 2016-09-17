#ifndef _KEY_H_
#define _KEY_H_
//#include "stm32f407xx.h"
#include "stm32f4xx.h" 
#include "io_bit.h"
//#include "MyTypeDef.h"
//#define KEY1   ((GPIOA->IDR>>0) &0X01)
//#define KEY2   ((GPIOE->IDR>>2) &0X01)
//#define KEY3   ((GPIOE->IDR>>3) &0X01)
//#define KEY4   ((GPIOE->IDR>>4) &0X01)
 
#define KEY1 PAin(0)
#define KEY2 PEin(2)
#define KEY3 PEin(3)
#define KEY4 PEin(4)
 
#define NO_KEY   0XFF
#define KEY1_OK     1
#define KEY2_OK 	2
#define KEY3_OK 	3
#define KEY4_OK	   4 





void key_init(void);
u8  key_scanf(void);

#endif


