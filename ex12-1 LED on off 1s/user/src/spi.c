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
	
		//spiģ��ʱ��ʹ��
	RCC->APB2ENR |=0X01<<12;
	
	RCC->APB2RSTR |=0X01<<12; //��λģ��
	RCC->APB2RSTR &=~(0X01<<12);//ֹͣ��λ
	
	SPI1->CR1 |=0X03<<8;  //�����ģʽ��������Ϊ��ģʽ
	SPI1->CR1 &=~(0X07<<3);  //��������Ϊbond=pclk2/2=42M
	SPI1->CR1 |=0X01<<2;  //��ģʽ
	
	SPI1->CR1 &=~(0X03<<0);  //ģʽ0
	
	SPI1->CR1 |=0X01<<6;    //��SPI������
		
	//SPI_SCK=0;
}

u8 SPI1_ReadWriteByte(u8 _data)
{
	while(!(SPI1->SR &0X01<<1));  //�ȴ����ͻ�����Ϊ��
	SPI1->DR =_data;
	while(!(SPI1->SR &0X01<<0)); //�ȴ����ջ������ǿ�
	return SPI1->DR ;
}



