#include "spi.h"
//spi接口初始化
void SPI1_Init(void)
{

	RCC->AHB1ENR |=0X01<<1;
	//SCK(pb3) MOSI(pb5)配置为复用功能推挽输出
	//端口模式设置
	//模式----复用模式（01）
	GPIOB->MODER &=~(0x03<<6 |0x03<<10);
	GPIOB->MODER |=0X02<<6 | 0X02<<10;
	//输出类型----推挽
	GPIOB->OTYPER &= ~(0X01<<3 | 0X01<<5);	
	//输出速度----100M
	GPIOB->OSPEEDR |=0x03<<6 | 0x03<<10;
	//上下拉-----无上拉或下拉
	GPIOB->PUPDR &=~(0x03<<6 |0x03<<10); 
	
	//MISO(pb4) 配置为复用功能（输入）
	GPIOB->MODER &=~(0X03<<8);  //pB4
	GPIOB->MODER |=(0X02<<8);
	
	GPIOB->AFR[0] &=0XFF000FFF;
	GPIOB->AFR[0] |=0X00555000;
	
	
	
	//SPI1时钟使能
	RCC->APB2ENR |=0X01<<12;
	
	SPI1->CR1 &=~(0x03<<14);  //3线模式
	SPI1->CR1 &=~(0X01<<11); //8位数据模式
	SPI1->CR1 |=0X03<<8;   //软件管理NSS，主模式
	SPI1->CR1 &=~(0X01<<7); //高位在前
	
	//波特率默认
	
	SPI1->CR1 |=0X01<<2;   //主模式
	SPI1->CR1 &=~(0X03<<0); //mode 0 
	
	SPI1->CR2 &=~(0X01<<4); //摩托罗拉格式
	
	
	SPI1->CR1 |=0X01<<6;    //开SPI控制器
}
//spi读写函数--模式0
//_data:待发送的数据，如果是接收模式，发送的数据则应该是0xff
//返回值：接收到的数据。如果是发送模式，返回值可以丢弃

u8 SPI1_ReadWriteByte(u8 _data)
{

	while(!(SPI1->SR &(0X01<<1)));
	SPI1->DR=_data;
	while(!(SPI1->SR &(0X01<<0)));
	return SPI1->DR;
	
}



