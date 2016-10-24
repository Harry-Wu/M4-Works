#include "stm32f4xx.h"
#include "spi.h"
void spi_init(void)
{
	RCC->AHB1ENR |=0x01<<1;
	GPIOB->MODER &=~(0X03<<6 |0X03<<8 |0X03<<10);
	GPIOB->MODER |=0X02<<6 |0X02<<8 |0X02<<10;
	GPIOB->OTYPER &=~(0X01<<3 | 0X01<<5);
	GPIOB->OSPEEDR |=0X03<<6 |0X03<<10;
	GPIOB->PUPDR&=~(0X03<<6 |0X03<<8 |0X03<<10);
	
	GPIOB->AFR[0] &=0XFF000FFF;
	GPIOB->AFR[0] |=0X00555000;
	
		//spi模块时钟使能
	RCC->APB2ENR |=0X01<<12;
	
	RCC->APB2RSTR |=0X01<<12; //复位模块
	RCC->APB2RSTR &=~(0X01<<12);//停止复位
	
	SPI1->CR1 |=0X03<<8;  //软件从模式管理，配置为主模式
	SPI1->CR1 &=~(0X07<<3);  //波特率设为bond=pclk2/2=42M
	SPI1->CR1 |=0X01<<2;  //主模式
	
	SPI1->CR1 &=~(0X03<<0);  //模式0
	
	SPI1->CR1 |=0X01<<6;    //开SPI控制器
		
	//SPI_SCK=0;
}

u8 SPI1_ReadWriteByte(u8 _data)
{
	while(!(SPI1->SR &0X01<<1));  //等待发送缓冲器为空
	SPI1->DR =_data;
	while(!(SPI1->SR &0X01<<0)); //等待接收缓冲器非空
	return SPI1->DR ;
}



