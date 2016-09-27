#include "stm32f4xx.h"
#include "led.h"

void exti_init(void)
{
	//如果对应IO口在其它地方初始化过了，就不用了
	
	//使能PA0外部中断
	
	EXTI->EMR &= ~(0x01<<0);  //禁止事件中断
	EXTI->RTSR |= 0x01<<0; //使能上升沿触发
	EXTI->FTSR &= ~(0x01<<0);//禁止下降沿触发
	EXTI->IMR |= 0x01<<0;//使能exti0外部中断
	
	
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(7-2, 3, 3));
	NVIC_EnableIRQ(EXTI0_IRQn);  //外部中断使能
	
	
}

void exti2_init(void)
{
	//如果对应IO口在其它地方初始化过了，就不用了
	
	//使能PE2外部中断
	
	EXTI->EMR &= ~(0x01<<2);  //禁止exti2事件中断
	EXTI->RTSR &= ~(0x01<<2); //禁用上升沿触发
	EXTI->FTSR |= 0x01<<2;//使能下降沿触发
	EXTI->IMR |= 0x01<<2;//使能exti2中断
	
	
	RCC->APB2ENR |= 0x01<<14; //系统配置控制器时钟使能
	SYSCFG->EXTICR[0] &= ~(0x0F<<8);  //映射PE2到EXTI2
	//SYSCFG->EXTICR[0] |= 0x04<<8;
	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI2_PE;
	
	//SYSCFG_EXTICR1_EXTI2_PE;
	
	
	NVIC_SetPriority(EXTI2_IRQn,NVIC_EncodePriority(7-2, 3, 3));
	NVIC_EnableIRQ(EXTI2_IRQn);  //外部中断使能
	
	
}

//软件中断
void exti9_soft_init(void)
{
	EXTI->EMR &= ~(0x01<<9);  //禁止exti9事件中断
	EXTI->RTSR &= ~(0x01<<9); //禁用上升沿触发
	EXTI->FTSR &= ~(0x01<<9);//使能下降沿触发
	EXTI->IMR |= 0x01<<9;//使能exti9中断
	
	NVIC_SetPriority(EXTI9_5_IRQn,NVIC_EncodePriority(7-2, 3, 3));
	NVIC_EnableIRQ(EXTI9_5_IRQn);  //外部中断使能
}


void EXTI0_IRQHandler(void)
{
	//清exti0的标志
	EXTI->PR |= 0x01;
	LED5 = !LED5;
}

void EXTI2_IRQHandler(void)
{
	//清exti2的标志
	EXTI->PR |= 0x01<<2;
	LED6 = !LED6;
}

//软件中断
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & (0x01<<9))
	{
		//清exti2的标志
		EXTI->PR |= 0x01<<9;
		//BEEP = !BEEP;
		LED5 = !LED5;
	}
	
	
}

