#include "stm32f4xx.h"
#include "led.h"

void timer14_init(u16 arr, u16 psc)
{
	//timer14可连到PF9(D5)复用功能, 但是更新溢出中断不需要IO设置复用功能
    //内部时钟选择
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    //计数器模式, tim14默认向上
	//TIM14->CR1 |=  
    //预分频系数
	TIM14->PSC = psc;
	//自动加载值
	TIM14->ARR = arr;
	//允许更新中断
	TIM14->DIER |= 0X01<<0;
    //使能计数器
	TIM14->CR1 |=  0X01<<0;
	
	NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn,NVIC_EncodePriority(7-2, 3, 3));
	NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);  //TIM14中断使能
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	if(TIM14->SR & 0X01)  //更新溢出中断标志
	{
		LED6 = !LED6;
	}
	TIM14->SR &= ~(0X01<<0);  //清中断
}
