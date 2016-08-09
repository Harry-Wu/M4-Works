#include "stm32f4xx.h"

void uart1_init(u32 bond)
{
	//端口时钟使能
	RCC->AHB1ENR |= 0x01<<0;
	//PA9(TXD)--复用推挽功能
	//PA10(RXD)--复用浮空功能
	GPIOA->MODER &= ~(0x03<<18 | 0x03<<20);
	GPIOA->MODER |= (0x02<<18 | 0x02<<20); //PA9,10设为复用功能
	GPIOA->OTYPER &= ~(0x01<<9);  //PA9推挽输出
	GPIOA->OSPEEDR &= ~(0x03<<18);  //pa9输出速度2M
	GPIOA->PUPDR &= ~(0x03<<18 | 0x03<<20); //pa9,10不用上下拉
	
	GPIOA->AFR[1] &= 0xfffff00f;
	GPIOA->AFR[1] |= 0x00000770; // //pa9 10 复用功能设为uart1的txd rxd, AF7--0b0111
	
	
	//uart1时钟使能
	RCC->APB2ENR |=0X01<<4;
	//波特率设置
	USART1->BRR =84000000/bond;  //AFB2是84M
	
	USART1->CR1 &=~(0X01<<15); //16倍过采样
	USART1->CR1 &=~(0X01<<12);  //字长为8位
	USART1->CR1 |=0x01<<3; //开发送
	USART1->CR1 |=0x01<<2; //开接收	
	USART1->CR2 &=~(0x03<<12); //一个停止位
	//中断使能, 暂不加
	
	USART1->CR1 |= 0x01<<13;  //开串口
	
}
