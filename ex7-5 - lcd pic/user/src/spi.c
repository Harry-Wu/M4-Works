#include "stm32f4xx.h" 
#include "spi.h"
void spi_init(void)
{
	//mosi(pb5) miso(pb4) sck(PB3)
	//mosi sck配置为复用功能推挽输出（不用上下拉）
	//miso配置为复用功能输入（不用上下拉）
	//端口时钟使能
	RCC->AHB1ENR |=0X01<<1;
	//端口模式
	GPIOB->MODER &=~(0X03<<6 | 0X03<<8 |0X03<<10);
	GPIOB->MODER |=0X02<<6 | 0X02<<8 |0X02<<10;
	
	GPIOB->OTYPER &=~(0X01<<3 | 0X01<<5);
	GPIOB->OSPEEDR|=0X03<<6 | 0X03<<10; 
	GPIOB->PUPDR &=~(0X03<<6 | 0X03<<8 |0X03<<10);
	
	GPIOB->AFR[0] &=0XFF000FFF;
	GPIOB->AFR[0] |=0X00555000;
	
	//SPI控制器时钟使能
		RCC->APB2ENR |=0X01<<12;
	//SPI控制器初始化
	SPI1->CR1 &=~(0X01<<11);      //8位数据格式
	SPI1->CR1 &=~(0X01<<10);      //全双模式
	SPI1->CR1 |=0x03<<8;    //软件主从模式管理--主模式
	SPI1->CR1 &=~(0X01<<7);      //高位在前
	SPI1->CR1 &=~(0x07<<3);    //spi波特率=84M/2
	SPI1->CR1 |=0x01<<2;     //主模式
	SPI1->CR1 &=~(0x03<<0);   //模式0	
	SPI1->CR1 |=0X01<<6;  //开SpI控制器
}

u8 SPI1_ReadWriteByte(u8 _data)
{
	while(!(SPI1->SR &(0X01<<1)));
	SPI1->DR=_data;
	
	while(!(SPI1->SR &(0X01<<0)));
	return SPI1->DR;
}



