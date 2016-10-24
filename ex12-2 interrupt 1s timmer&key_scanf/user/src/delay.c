#include "stm32f4xx.h"
#include "delay.h"

void delay_us(u32 nus)
{
	u32 sta;
//	1.	选择时钟源sysclk/8
	SysTick->CTRL &=~(0x01<<2);
//2.	设置load寄存器-----设定计数初值
	SysTick->LOAD=nus*SYSCLK/8;
//3.	对VAL写操作------VAL会被清零?重装载
	SysTick->VAL=0;
//4.	开启SYSTICK
	SysTick->CTRL |=0x01<<0;
//5.	等待定时时间到（查询CTRL.16是否为1）
	do{
		sta=SysTick->CTRL;
	}
	while((sta & 0x01)&&!(sta &(0x01<<16))); //systick是开启的，并且定时时间还没到
//6.	关SYSTICK定时器
	SysTick->CTRL &=~(0x01<<0);
}

void delya_us_timer(u32 us)
{
//	1.	定时器时钟使能
	RCC->APB1ENR |=0X01<<5;
//2.	设置ARR（自动重装载值）、PSC（预分频值）
	//Tout=((arr+1)*(psc+1))/Ft us.  Ft=84MHz
	TIM7->ARR =84-1;
	TIM7->PSC =1*us-1;
//3.	使能更新，并且设置UG位能产生更新事件
	TIM7->CR1 |=0X01<<7;	//ARR有缓存
	TIM7->CR1 &=~(0X01<<3); //不使用单脉冲模式
	TIM7->CR1 &=~(0X03<<1); //使能更新，设置UG位会产生更新
//4.	设置UG位产生更新事件
	TIM7->EGR |=0X01;
//5.	清除更新事件标志
	TIM7->SR &=~(0X01);
//6.	使能定时器
	TIM7->CR1 |=0X01<<0;
	while(!(TIM7->SR &(0X01)));//检测没有中断标志时，不进行下一步动作
	TIM7->SR &=~(0X01);   //清除更新标志
	TIM7->CR1 &=~(0X01<<0); //禁能定时器
}

void delya_ms_timer(u32 ms)
{
//	1.	定时器时钟使能
	RCC->APB1ENR |=0X01<<5;
//2.	设置ARR（自动重装载值）、PSC（预分频值）
	//Tout=((arr+1)*(psc+1))/Ft us.  Ft=84MHz
	TIM7->ARR =84*ms-1;
	TIM7->PSC =1000-1;
//3.	使能更新，并且设置UG位能产生更新事件
	TIM7->CR1 |=0X01<<7;	//ARR有缓存
	TIM7->CR1 &=~(0X01<<3); //不使用单脉冲模式
	TIM7->CR1 &=~(0X03<<1); //使能更新，设置UG位会产生更新
//4.	设置UG位产生更新事件
	TIM7->EGR |=0X01;
//5.	清除更新事件标志
	TIM7->SR &=~(0X01);
//6.	使能定时器
	TIM7->CR1 |=0X01<<0;
	while(!(TIM7->SR &(0X01)));//检测没有中断标志时，不进行下一步动作
	TIM7->SR &=~(0X01);   //清除更新标志
	TIM7->CR1 &=~(0X01<<0); //禁能定时器
}


//最大延时798.9ms
void delay_ms(u32 nms)
{
	u32 sta;
//	1.	选择时钟源sysclk/8
	SysTick->CTRL &=~(0x01<<2);
//2.	设置load寄存器-----设定计数初值
	SysTick->LOAD=nms*1000*SYSCLK/8;
//3.	对VAL写操作------VAL会被清零?重装载
	SysTick->VAL=0;
//4.	开启SYSTICK
	SysTick->CTRL |=0x01<<0;
//5.	等待定时时间到（查询CTRL.16是否为1）
	do{
		sta=SysTick->CTRL;
	}
	while((sta & 0x01)&&!(sta &(0x01<<16))); //systick是开启的，并且定时时间还没到
//6.	关SYSTICK定时器
	SysTick->CTRL &=~(0x01<<0);
}



void systick_interrupt_init(u32 nms)
{
	u32 sta;
//	1.	选择时钟源sysclk/8
	SysTick->CTRL &=~(0x01<<2);
//2.	设置load寄存器-----设定计数初值
	SysTick->LOAD=nms*1000*SYSCLK/8;
//3.	对VAL写操作------VAL会被清零?重装载
	SysTick->VAL=0;
//开中断
	SysTick->CTRL |=0x01<<1;
	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(7-2,1,1));
	
//4.	开启SYSTICK
	SysTick->CTRL |=0x01<<0;

}


