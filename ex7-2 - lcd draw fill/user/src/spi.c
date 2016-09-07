#include "stm32f4xx.h" 
#include "spi.h"
void spi_init(void)
{
	//mosi(pb5) miso(pb4) sck(PB3)
	//mosi sck����Ϊ���ù������������������������
	//miso����Ϊ���ù������루������������
	//�˿�ʱ��ʹ��
	RCC->AHB1ENR |=0X01<<1;
	//�˿�ģʽ
	GPIOB->MODER &=~(0X03<<6 | 0X03<<8 |0X03<<10);
	GPIOB->MODER |=0X02<<6 | 0X02<<8 |0X02<<10;
	
	GPIOB->OTYPER &=~(0X01<<3 | 0X01<<5);
	GPIOB->OSPEEDR|=0X03<<6 | 0X03<<10; 
	GPIOB->PUPDR &=~(0X03<<6 | 0X03<<8 |0X03<<10);
	
	GPIOB->AFR[0] &=0XFF000FFF;
	GPIOB->AFR[0] |=0X00555000;
	
	//SPI������ʱ��ʹ��
		RCC->APB2ENR |=0X01<<12;
	//SPI��������ʼ��
	SPI1->CR1 &=~(0X01<<11);      //8λ���ݸ�ʽ
	SPI1->CR1 &=~(0X01<<10);      //ȫ˫ģʽ
	SPI1->CR1 |=0x03<<8;    //�������ģʽ����--��ģʽ
	SPI1->CR1 &=~(0X01<<7);      //��λ��ǰ
	SPI1->CR1 &=~(0x07<<3);    //spi������=84M/2
	SPI1->CR1 |=0x01<<2;     //��ģʽ
	SPI1->CR1 &=~(0x03<<0);   //ģʽ0	
	SPI1->CR1 |=0X01<<6;  //��SpI������
}

u8 SPI1_ReadWriteByte(u8 _data)
{
	while(!(SPI1->SR &(0X01<<1)));
	SPI1->DR=_data;
	
	while(!(SPI1->SR &(0X01<<0)));
	return SPI1->DR;
}



