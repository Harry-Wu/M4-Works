#include "stm32f4xx.h" 
#include "stdio.h"

void uart1_init(u32 bond)
{
	//�˿�ʱ��ʹ��PA
	RCC->AHB1ENR |=0x01;
		//�˿�����
	//pa9(txd)-----���ù����������
	GPIOA->MODER &=~(0X03<<18);   
	GPIOA->MODER |=0X02<<18;     //���ù���ģʽ
	GPIOA->OTYPER &=~(0X01<<9);  //�������
	GPIOA->OSPEEDR&=~(0X03<<18);  //����ٶ����Ϊ2M
	GPIOA->PUPDR &=~(0X03<<18);   //��������
	GPIOA->AFR[1] &=0XFFFFFF0F;
	GPIOA->AFR[1] |=0X00000070;   //uart����ģʽ	
	//pa10(rxd)----���ù��ܸ�������
	GPIOA->MODER &=~(0X03<<20);   
	GPIOA->MODER |=0X02<<20;     //���ù���ģʽ
	GPIOA->PUPDR &=~(0X03<<20);   //��������
	GPIOA->AFR[1] &=0XFFFFF0FF;
	GPIOA->AFR[1] |=0X00000700;   //uart����ģʽ	
	
	//ģ��ʱ��ʹ��--UART1
	RCC->APB2ENR |=0X01<<4; 	
	RCC->APB2RSTR |= 0X01<<4;    //��λuart1
	RCC->APB2RSTR &= ~(0X01<<4);  //ֹͣ��λ
	
	
	//ģ���ʼ������
	USART1->BRR=84000000/bond;
	USART1->CR1 &=~(0X01<<15);  //OVER8=0   16λ����
	USART1->CR1 &=~(0X01<<12);  //һ����ʼλ��8λ���ݣ�N��ֹͣλ
	
	USART1->CR2 &=~(0X03<<12);   //һ��ֹͣλ
	
	
	
	USART1->CR1 |=0X01<<5; //�����ڽ����ж�
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,3,3));  //�������ȼ�
	NVIC_EnableIRQ(USART1_IRQn); //�ⲿ�ж�ʹ�ܣ�ϵͳ�ж�û�����ʹ�ܣ�

	//����ģ��
		USART1->CR1 |=0X01<<2;  //�����ڽ���
		USART1->CR1 |=0X01<<3;  //�����ڷ���
		USART1->CR1 |=0X01<<13;  //������
}




u8 rec_buf[100];   //���ջ�����
u8 rec_sta=0;       //�����������־ ��rec_sta=1���������    
u8 rec_num=0;       //���ռ���
u8 cont_time=0;
u8 start_rec=0;     //start_rec=1��ʼ����ʱ

void  USART1_IRQHandler(void)
{

	//�ж��Ƿ��ǽ����ж�
	if((rec_sta==0)&&(USART1->SR &(0X01<<5)))//��û�����꣬�����н��յ�����
	{
		cont_time=0;
		start_rec=1;
		rec_buf[rec_num++]=USART1->DR;
	
	}
	else
	{
		USART1->DR;		
	}
	
}






void uart_send_string(u8 *p)
{
	while(*p!='\0')
	{
		//�ȴ��ϴη������
		while((USART1->SR &(0X01<<7))==0);
		//��ǰ����������ݻط�����λ��
		USART1->DR=*p++;
	}
	
}


int fputc(int ch, FILE *f) 
{
	while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
  return (ch);
}

