#include "stm32f4xx.h" 
#include "spi.h"

void SPI1_Init(void)
{
	//sck(pb3)-----���ù����������
	//mosi(pb5)-----���ù����������
	//miso(pb4)-----���ù��ܸ�������
	
	//cs(pb14)-----��ͨ�����������
	RCC->AHB1ENR |=0X01<<1;
	GPIOB->MODER &=~(0X03<<6 |0X03<<8 |0X03<<10 |0X03<<28 );
	GPIOB->MODER |=(0X02<<6 | 0X02<<8 |0X02<<10 |0X01<<28 );
	
	GPIOB->OTYPER &=~(0X01<<3 | 0X01<<5 | 0X01<<14);  //�������
	GPIOB->OSPEEDR |=(0X03<<6 |0X03<<10 |0X03<<28 );  //����ٶ�100M
	GPIOB->PUPDR &=~(0X03<<6 |0X03<<8 | 0X03<<10 |0X03<<28 ); //��������
	
	//���ù�������
	GPIOB->AFR[0] &=0XFF000FFF;
	GPIOB->AFR[0] |=0X00555000;
	
	CS=1;
	
	//ģ������
	//ģ��ʱ��ʹ��sp1
	RCC->APB2ENR |=0X01<<12;
	
	SPI1->CR1 &=~(0X01<<11);  //8λ����ģʽ
	SPI1->CR1 |=0x01<<9;    //ѡ�����ģʽ����
	SPI1->CR1 |=0x01<<8;   //��ģʽ
	SPI1->CR1 &=~(0X01<<7);  //��λ��ǰ
	
	SPI1->CR1 &=~(0X03<<3);  // f=fpclk2/2=42M
	
	SPI1->CR1 |=0x01<<2;   //��ģʽ
	
	SPI1->CR1 &=~(0X03<<0);  //spiģʽ0
	
	SPI1->CR1 |=0x01<<6;   //��SPI
	
	
	
}

void spi_delay(void)
{
	u32 i=5;
	while(i--);
}

//������_data---�����͵�����
//����ֵ����ȡ����һ���ֽ�����
u8 SPI1_ReadWriteByte(u8 _data)
{
	while(!(SPI1->SR &(0X01<<1)));
	SPI1->DR=_data;
	while(!(SPI1->SR &(0X01<<0)));
	return SPI1->DR;
}




