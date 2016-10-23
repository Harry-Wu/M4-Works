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


