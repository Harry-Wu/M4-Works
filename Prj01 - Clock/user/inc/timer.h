#ifndef _TIMER_H_
#define _TIMER_H_
#include "stm32f4xx.h"

void timer14_init(u16 arr, u16 psc);
void TIM8_TRG_COM_TIM14_IRQHandler(void);

#endif
