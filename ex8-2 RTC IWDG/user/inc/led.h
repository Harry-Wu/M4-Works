#ifndef _LED_H_
#define _LED_H_
#include "stm32f4xx.h"
#include "io_bit.h"
//#define LED1OFF (GPIOF->ODR |=0X01<<9)
//#define LED1ON  (GPIOF->ODR &=~(0X01<<9))

//#define LED2OFF (GPIOF->ODR |=0X01<<10)
//#define LED2ON  (GPIOF->ODR &=~(0X01<<10))


#define LED1 PFout(9)
#define LED2 PFout(10)

void led_init(void);

#endif


