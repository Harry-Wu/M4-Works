/*****************************************************************************
@File name:  
@Description: 闪烁LED 
@Author: Harry Wu
@Version: V1.1
@Date: 2016-11-7
@History: 
		V1.0: 最基本的操作寄存器
		V1.1: 增加位带操作, 项目格式升级为uvprojx

		
*****************************************************************************/
#include "stm32f4xx.h"
//#include "key.h"
//#include "delay.h"
#include "io_bit.h"
#define D5  PFout(9)
#define D6  PFout(10)
#define LS1 PFout(8)

void led_init(void)
{
	//端口时钟使能PF口
	RCC->AHB1ENR |= 1<<5;
	//设置管脚PF9----D5设置为普通功能、推挽、无上下拉功输出
	GPIOF->MODER &= ~(0x03<<18);  //PF9, 18,19位清零
	GPIOF->MODER |= 0x01<<18;  //18位置1
	GPIOF->OTYPER &= ~(0x01<<9);  //推挽输出
	GPIOF->OSPEEDR &= ~(0x03<<18);  //输出最大速度2M
	GPIOF->PUPDR &= ~(0x03<<18); //无上下拉
	
	//设置管脚PF10----D6设置为普通功能、推挽、无上下拉功输出
	GPIOF->MODER &= ~(0x03<<20);  //PF10, 20,21位清零
	GPIOF->MODER |= 0x01<<20;  //20位置1
	GPIOF->OTYPER &= ~(0x01<<10);
	GPIOF->OSPEEDR &= ~(0x03<<20);  //输出最大速度2M
	GPIOF->PUPDR &= ~(0x03<<20); //无上下拉
	
	//设定初始状态---非工作状态
	GPIOF->ODR |= 0x03<<9;  //控制PF9,PF10高电平,D5,D6灭
}

//延时函数
void delay()
{
	u32 i=0x1000000;
	while(i--);
}
	

int main(void)
{
	//硬件初始化
	led_init();
	
	while(1)
	{
		D5 = 0;  //低电平,LED ON
		D6 = 0;  //低点平LED ON
		delay();
		D5 = 1;  //LED OFF
		D6 = 1;  //LED OFF
		delay();
		
	}
}
