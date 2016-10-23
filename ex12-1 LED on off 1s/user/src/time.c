#include "stm32f4xx.h"


void time6_init(void)
{
//	1.	模块时钟使能--TIME6
	RCC->APB1ENR |=0X01<<4;
//2.	设置预分频和自动重装载值
	TIM6->PSC =8400-1;  
	TIM6->ARR =5000-1;  //84M/(5000*8400)=2Hz
//3.	设置UG会产生更新事件
	
//4.	设置UG位，产生软件更新
	TIM6->EGR |=0X01;
//5.	清更新标志
	TIM6->SR &=~(0X01);
//6.	开定时器
	TIM6->CR1 |=0X01<<0;
}