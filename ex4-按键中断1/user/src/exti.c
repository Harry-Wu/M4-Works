#include "stm32f4xx.h"
#include "led.h"

void exti_init(void)
{
	//如果对应IO口在其它地方初始化过了，就不用了
	
	
	EXTI->EMR &= ~(0x01<<0);  //禁止事件中断
	EXTI->RTSR |= 0x01<<0; //使能上升沿触发
	EXTI->FTSR &= ~(0x01<<0);//禁止下降沿触发
	EXTI->IMR |= 0x01<<0;//使能exti0外部中断
	
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(7-2, 3, 3));
	NVIC_EnableIRQ(EXTI0_IRQn);  //外部中断使能
	
	
}

void EXTI0_IRQHandler(void)
{
	//清标志
	EXTI->PR |= 0x01;
	LED2 = !LED2;
}

