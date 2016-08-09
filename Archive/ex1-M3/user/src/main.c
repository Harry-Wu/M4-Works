#include "stm32f10x.h"
//#include "core_cm3.h"


void led_init(void)
{
	//端口时钟使能PF口
	RCC->APB2ENR |= 1<<3;
	RCC->APB2ENR |= 1<<6;
	//设置管脚--PF9,PF10设置为普通功能、推挽、无上下拉输出
	GPIOB->CRL &= ~(0x03<<20);  //PF9, 4,5位清零
	GPIOB->CRL |= 0x01<<21;  //5位置1. PF9 MODE设为10---输出模式，最大2MHz
	
	GPIOE->CRL &= ~(0x03<<20);  //PF10, 8,9位清零
	GPIOE->CRL |= 0x01<<21;  //9位置1
	
	GPIOB->CRL &= ~(0x03<<22);  //推挽输出
	GPIOE->CRL &= ~(0x03<<22);
	
	//GPIOF->OSPEEDR &= ~(0x03<<18);  //输出最大速度2M
	
	//GPIOF->PUPDR &= ~(0x03<<18); //无上下拉
	
	//设定初始状态---非工作状态
	GPIOB->ODR |= 0x01<<5;
	GPIOE->ODR |= 0x01<<5;
	
	
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
		GPIOB->ODR &= ~(0x01<<5);  //LED ON
		GPIOE->ODR &= ~(0x01<<5);  //LED ON
		delay();
		GPIOB->ODR |= 0x01<<5;  //LED OFF
		GPIOE->ODR |= 0x01<<5;  //LED OFF
		delay();
		
	}
}
