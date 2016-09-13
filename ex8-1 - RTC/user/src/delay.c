#include "stm32f4xx.h" 
#include "delay.h"
//#include "MyTypeDef.h"

//延时n微秒
//参数n: us
void delay_us(uint32_t n)
{
	SysTick->LOAD =n*SYSCLK/8; //设定计数次数
	SysTick->VAL =0;     //重装载（重装载寄存器的值复制到计数器）
	SysTick->CTRL |=0x01; //开systick定时器
	while(!(SysTick->CTRL&(0x01<<16)));
	SysTick->CTRL &=~(0x01); //关systick定时器
}

//延时n毫秒
//参数n: ms  (最大延时798.9ms)
void delay_ms(u32 n)
{
	SysTick->LOAD =n*SYSCLK*1000/8; //设定计数次数
	SysTick->VAL =0;     //重装载（重装载寄存器的值复制到计数器）
	SysTick->CTRL |=0x01; //开systick定时器
	while(!(SysTick->CTRL&(0x01<<16)));
	SysTick->CTRL &=~(0x01); //关systick定时器
}
