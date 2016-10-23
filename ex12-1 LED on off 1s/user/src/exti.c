#include "stm32f4xx.h"
#include "led.h"
void exti0_init(void)
{
//	1. PA口时钟使能
	RCC->AHB1ENR |=0X01<<0;
//2. 将PA0配置为普通功能浮空输入
	GPIOA->MODER &=~(0X03<<0);
	GPIOA->PUPDR &=~(0X03<<0);
//3. 开启外部中断线0上升沿触发,禁止下降沿触发
	EXTI->RTSR |=0X01<<0;
	EXTI->FTSR &=~(0X01<<0);	
//4. 事件屏蔽寄存器对应位清0
	EXTI->EMR  &=~(0X01<<0);	
//5. 清挂起标志。
	EXTI->PR  |=(0X01<<0);	
//6. 计算优先级、设置优先级
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(7-2,1,1));
//7. 在NVIC中使能外部中断线0中断
	NVIC_EnableIRQ(EXTI0_IRQn);
//5. 中断屏蔽寄存器对应位置1
	EXTI->IMR |=0X01<<0;
}

void exti6_soft_init(void)
{

//3. 屏蔽边沿触发
	EXTI->RTSR &=~(0X01<<6);
	EXTI->FTSR &=~(0X01<<6);	
//4. 事件屏蔽寄存器对应位清0
	EXTI->EMR  &=~(0X01<<6);	
//5. 清挂起标志。
	EXTI->PR  |=(0X01<<6);	
//6. 计算优先级、设置优先级
	NVIC_SetPriority(EXTI9_5_IRQn,NVIC_EncodePriority(7-2,1,1));
//7. 在NVIC中使能外部中断线0中断
	NVIC_EnableIRQ(EXTI9_5_IRQn);
//5. 中断屏蔽寄存器对应位置1
	EXTI->IMR |=0X01<<6;
}


void exti2_3_4_init(void)
{
//	1. PE口时钟使能
	RCC->AHB1ENR |=0X01<<4;
//2. 将PE2 3 4配置为普通功能浮空输入
	GPIOE->MODER &=~(0X03<<4 |0X03<<6 |0X03<<8);
	GPIOE->PUPDR &=~(0X03<<4 |0X03<<6 |0X03<<8);
//使能系统配置时钟
	RCC->APB2ENR |=0X01<<14;
	SYSCFG->EXTICR[0] &=~(0XFF<<8);
	SYSCFG->EXTICR[0] |=0X44<<8;  
	
	SYSCFG->EXTICR[1] &=~(0XF<<0);
	SYSCFG->EXTICR[1] |=0X4<<0;  
//3. 开启外部中断线0上升沿触发,禁止下降沿触发
	EXTI->RTSR &=~(0X07<<2);
	EXTI->FTSR |=0x07<<2;	
//4. 事件屏蔽寄存器对应位清0
	EXTI->EMR  &=~(0X07<<2);	
//5. 清挂起标志。
	EXTI->PR  |=(0X07<<2);	
//6. 计算优先级、设置优先级
	NVIC_SetPriority(EXTI2_IRQn,NVIC_EncodePriority(7-2,1,1));
	NVIC_SetPriority(EXTI3_IRQn,NVIC_EncodePriority(7-2,1,1));
	NVIC_SetPriority(EXTI4_IRQn,NVIC_EncodePriority(7-2,1,1));
	
//7. 在NVIC中使能外部中断线中断
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
//5. 中断屏蔽寄存器对应位置1
	EXTI->IMR |=0X07<<2;
}






void EXTI0_IRQHandler(void)
{
	//清中断标志
	EXTI->PR  |=(0X01<<0);	
	LED1=!LED1;
}

void EXTI2_IRQHandler(void)
{
	//清中断标志
	EXTI->PR  |=(0X01<<2);	
	LED2=!LED2;
}

void EXTI3_IRQHandler(void)
{
	//清中断标志
	EXTI->PR  |=(0X01<<3);	
	LED3=!LED3;
}

void EXTI4_IRQHandler(void)
{
	//清中断标志
	EXTI->PR  |=(0X01<<4);	
	LED4=!LED4;
}


void EXTI9_5_IRQHandler(void)
{
	//判断是哪个中断线上的中断
	if(EXTI->PR &(0X01<<6))
	{
		EXTI->PR |=0X01<<6;
		LED2=!LED2;
	}
}




