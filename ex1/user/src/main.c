#include "stm32f4xx.h"

void led_init(void)
{
	//端口时钟使能PF口
	RCC->AHB1ENR |= 1<<5;
	//设置管脚--PF9,PF10设置为普通功能、推挽、无上下拉功输出
	GPIOF->MODER &= ~(0x03<<18);  //PF9, 18,19位清零
	GPIOF->MODER |= 0x01<<18;  //18位置1
	
	GPIOF->MODER &= ~(0x03<<20);  //PF10, 20,21位清零
	GPIOF->MODER |= 0x01<<20;  //20位置1
	
	GPIOF->OTYPER &= ~(0x01<<9);  //推挽输出
	GPIOF->OTYPER &= ~(0x01<<10);
	
	GPIOF->OSPEEDR &= ~(0x03<<18);  //输出最大速度2M
	
	GPIOF->PUPDR &= ~(0x03<<18); //无上下拉
	
	//设定初始状态---非工作状态
	GPIOF->ODR |= 0x03<<9;
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
		GPIOF->ODR &= ~(0x01<<9);  //LED ON
		GPIOF->ODR &= ~(0x01<<10);  //LED ON
		delay();
		GPIOF->ODR |= 0x01<<9;  //LED OFF
		GPIOF->ODR |= 0x01<<10;  //LED OFF
		delay();
		
	}
}
