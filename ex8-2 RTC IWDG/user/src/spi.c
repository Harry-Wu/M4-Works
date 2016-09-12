#include "spi.h"
//spi�ӿڳ�ʼ��
void SPI1_Init(void)
{

	RCC->AHB1ENR |=0X01<<1;
	//SCK(pb3) MOSI(pb5)����Ϊ���ù����������
	//�˿�ģʽ����
	//ģʽ----����ģʽ��01��
	GPIOB->MODER &=~(0x03<<6 |0x03<<10);
	GPIOB->MODER |=0X02<<6 | 0X02<<10;
	//�������----����
	GPIOB->OTYPER &= ~(0X01<<3 | 0X01<<5);	
	//����ٶ�----100M
	GPIOB->OSPEEDR |=0x03<<6 | 0x03<<10;
	//������-----������������
	GPIOB->PUPDR &=~(0x03<<6 |0x03<<10); 
	
	//MISO(pb4) ����Ϊ���ù��ܣ����룩
	GPIOB->MODER &=~(0X03<<8);  //pB4
	GPIOB->MODER |=(0X02<<8);
	
	GPIOB->AFR[0] &=0XFF000FFF;
	GPIOB->AFR[0] |=0X00555000;
	
	
	
	//SPI1ʱ��ʹ��
	RCC->APB2ENR |=0X01<<12;
	
	SPI1->CR1 &=~(0x03<<14);  //3��ģʽ
	SPI1->CR1 &=~(0X01<<11); //8λ����ģʽ
	SPI1->CR1 |=0X03<<8;   //�������NSS����ģʽ
	SPI1->CR1 &=~(0X01<<7); //��λ��ǰ
	
	//������Ĭ��
	
	SPI1->CR1 |=0X01<<2;   //��ģʽ
	SPI1->CR1 &=~(0X03<<0); //mode 0 
	
	SPI1->CR2 &=~(0X01<<4); //Ħ��������ʽ
	
	
	SPI1->CR1 |=0X01<<6;    //��SPI������
}
//spi��д����--ģʽ0
//_data:�����͵����ݣ�����ǽ���ģʽ�����͵�������Ӧ����0xff
//����ֵ�����յ������ݡ�����Ƿ���ģʽ������ֵ���Զ���

u8 SPI1_ReadWriteByte(u8 _data)
{

	while(!(SPI1->SR &(0X01<<1)));
	SPI1->DR=_data;
	while(!(SPI1->SR &(0X01<<0)));
	return SPI1->DR;
	
}



