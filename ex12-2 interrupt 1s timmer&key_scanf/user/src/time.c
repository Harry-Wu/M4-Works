#include "stm32f4xx.h"
#include "led.h"
#include "key.h"

void time6_init(void)
{
//	1.	定时器时钟使能
	RCC->APB1ENR |=0X01<<4;
//2.	设置ARR（自动重装载值）、PSC（预分频值）
	TIM6->ARR =100-1;
	TIM6->PSC =8400-1;
//3.	使能更新，并且设置UG位能产生更新事件
	TIM6->CR1 |=0X01<<7;	//ARR有缓存
	TIM6->CR1 &=~(0X01<<3); //不使用单脉冲模式
	TIM6->CR1 &=~(0X03<<1); //使能更新，设置UG位会产生更新
//4.	设置UG位产生更新事件
	TIM6->EGR |=0X01;
//5.	清除更新事件标志
	TIM6->SR &=~(0X01);
	//定时器中断使能
	TIM6->DIER |= 0X01;
	NVIC_SetPriority(TIM6_DAC_IRQn,NVIC_EncodePriority(7-2,1,1));
	//在NVIC中使能定时器中断
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	
//6.	使能定时器
	TIM6->CR1 |=0X01<<0;
}

void time7_init(void)
{
//	1.	定时器时钟使能
	RCC->APB1ENR |=0X01<<5;
//2.	设置ARR（自动重装载值）、PSC（预分频值）
	TIM7->ARR =10000-1;
	TIM7->PSC =8400-1;  //1Hz
//3.	使能更新，并且设置UG位能产生更新事件
	TIM7->CR1 |=0X01<<7;	//ARR有缓存
	TIM7->CR1 &=~(0X01<<3); //不使用单脉冲模式
	TIM7->CR1 &=~(0X03<<1); //使能更新，设置UG位会产生更新
//4.	设置UG位产生更新事件
	TIM7->EGR |=0X01;
//5.	清除更新事件标志
	TIM7->SR &=~(0X01);
	//定时器中断使能
	TIM7->DIER |= 0X01;
	NVIC_SetPriority(TIM7_IRQn,NVIC_EncodePriority(7-2,1,1));
	//在NVIC中使能定时器中断
	NVIC_EnableIRQ(TIM7_IRQn);
	
//6.	使能定时器
	TIM7->CR1 |=0X01<<0;
}

u8 key=NO_KEY;
void TIM6_DAC_IRQHandler(void)
{
	u8 key_rec;
	TIM6->SR &= ~(0X01);
	key_rec = key_scanf();
	if(key_rec!=NO_KEY)  //如果有按键按下
	{
		key = key_rec;
	}
}

void TIM7_IRQHandler(void)
{
	TIM7->SR &= ~(0X01);
	LED3 = !LED3;
}