#include "stm32f4xx.h" 
#include "spi.h"

void SPI1_Init(void)
{
	//sck(pb3)-----复用功能推挽输出
	//mosi(pb5)-----复用功能推挽输出
	//miso(pb4)-----复用功能浮空输入
	
	//cs(pb14)-----普通功能推挽输出
	RCC->AHB1ENR |=0X01<<1;
	GPIOB->MODER &=~(0X03<<6 |0X03<<8 |0X03<<10 |0X03<<28 );
	GPIOB->MODER |=(0X02<<6 | 0X02<<8 |0X02<<10 |0X01<<28 );
	
	GPIOB->OTYPER &=~(0X01<<3 | 0X01<<5 | 0X01<<14);  //推挽输出
	GPIOB->OSPEEDR |=(0X03<<6 |0X03<<10 |0X03<<28 );  //输出速度100M
	GPIOB->PUPDR &=~(0X03<<6 |0X03<<8 | 0X03<<10 |0X03<<28 ); //无上下拉
	
	//复用功能设置
	GPIOB->AFR[0] &=0XFF000FFF;
	GPIOB->AFR[0] |=0X00555000;
	
	CS=1;
	
	//模块设置
	//模块时钟使能sp1
	RCC->APB2ENR |=0X01<<12;
	
	SPI1->CR1 &=~(0X01<<11);  //8位传输模式
	SPI1->CR1 |=0x01<<9;    //选择软件模式管理
	SPI1->CR1 |=0x01<<8;   //主模式
	SPI1->CR1 &=~(0X01<<7);  //高位在前
	
	SPI1->CR1 &=~(0X03<<3);  // f=fpclk2/2=42M
	
	SPI1->CR1 |=0x01<<2;   //主模式
	
	SPI1->CR1 &=~(0X03<<0);  //spi模式0
	
	SPI1->CR1 |=0x01<<6;   //开SPI
	
	
	
}

void spi_delay(void)
{
	u32 i=5;
	while(i--);
}

//参数：_data---待发送的数据
//返回值：读取到的一个字节数据
u8 SPI1_ReadWriteByte(u8 _data)
{
	while(!(SPI1->SR &(0X01<<1)));
	SPI1->DR=_data;
	while(!(SPI1->SR &(0X01<<0)));
	return SPI1->DR;
}




