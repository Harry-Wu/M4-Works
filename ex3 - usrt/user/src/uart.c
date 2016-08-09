#include "stm32f4xx.h"

void uart1_init(u32 bond)
{
	//�˿�ʱ��ʹ��
	RCC->AHB1ENR |= 0x01<<0;
	//PA9(TXD)--�������칦��
	//PA10(RXD)--���ø��չ���
	GPIOA->MODER &= ~(0x03<<18 | 0x03<<20);
	GPIOA->MODER |= (0x02<<18 | 0x02<<20); //PA9,10��Ϊ���ù���
	GPIOA->OTYPER &= ~(0x01<<9);  //PA9�������
	GPIOA->OSPEEDR &= ~(0x03<<18);  //pa9����ٶ�2M
	GPIOA->PUPDR &= ~(0x03<<18 | 0x03<<20); //pa9,10����������
	
	GPIOA->AFR[1] &= 0xfffff00f;
	GPIOA->AFR[1] |= 0x00000770; // //pa9 10 ���ù�����Ϊuart1��txd rxd, AF7--0b0111
	
	
	//uart1ʱ��ʹ��
	RCC->APB2ENR |=0X01<<4;
	//����������
	USART1->BRR =84000000/bond;  //AFB2��84M
	
	USART1->CR1 &=~(0X01<<15); //16��������
	USART1->CR1 &=~(0X01<<12);  //�ֳ�Ϊ8λ
	USART1->CR1 |=0x01<<3; //������
	USART1->CR1 |=0x01<<2; //������	
	USART1->CR2 &=~(0x03<<12); //һ��ֹͣλ
	//�ж�ʹ��, �ݲ���
	
	USART1->CR1 |= 0x01<<13;  //������
	
}
