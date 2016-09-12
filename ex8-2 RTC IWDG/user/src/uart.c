#include "stm32f4xx.h" 
#include "stdio.h"
#include "LED.h"
//���ڳ�ʼ��
//bond��������
void uart_init(u32 bond)
{


	//�˿�ʱ��ʹ��PA
	RCC->AHB1ENR |= 0X01<<0;
//�˿�ģʽ���� PA9(txd) PA10(rxd)
	//PA9(txd) PA10(rxd)��Ϊ���ù���ģʽ
	GPIOA->MODER &=~(0x03<<18 |0x03<<20);
	GPIOA->MODER |=0x02<<18 |0x02<<20;
	//PA9(txd)��Ϊ�������
	GPIOA->OTYPER &=~(0X01<<9);
	//PA9(txd)��Ϊ����ٶ�2M
	GPIOA->OSPEEDR &=~(0X03<<18);
	//PA9(txd) PA10(rxd)��Ϊ���ù���ӳ�䣬ӳ�䵽����1
	GPIOA->AFR[1] &=~(0XFF<<4);
	GPIOA->AFR[1] |= 0X77<<4;
	

	
	
//UART1ģ��ʱ��ʹ��
	RCC->APB2ENR |=0x01<<4;	
	
//���ò�����
//	brr=(float)84000000/(8*(2-0)*bond);
//	USART1->BRR=brr*16;
USART1->BRR=84000000/bond;



	//���ݳ���Ϊ8λ��ֹͣλΪ1λ�����ա������͡�������
	USART1->CR1 &=~(0x01<<15);  //OVER8=0 
	USART1->CR1 &=~(0x01<<12);  //�ֳ�8λ
	
	USART1->CR1 |=0x01<<3;  //������1����
	USART1->CR1 |=0x01<<2;  //������1����
	
	USART1->CR2 &=~(0X03<<12);  //����ֹͣλΪ1λ
	

	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,3,3));  //�������ȼ�
	NVIC_EnableIRQ(USART1_IRQn); //�ⲿ�ж�ʹ�ܣ�ϵͳ�ж�û�����ʹ�ܣ�
	

	USART1->CR1 |=0x01<<5;  //�����ڽ����ж�
	USART1->CR1 |=0x01<<13;  //������1
	
}

u8 rec_buf[100];   //���ջ�����
u8 rec_sta=0;       //�����������־ ��rec_sta=1���������    
u8 rec_num=0;       //���ռ���


void  USART1_IRQHandler(void)
{
	u8 rec_data;
	//�ж��Ƿ��ǽ����ж�
	if((rec_sta==0)&&(USART1->SR &(0X01<<5)))//��û�����꣬�����н��յ�����
	{
		rec_data =USART1->DR;
		if((rec_data=='\n') &&(	rec_buf[rec_num-1]=='\r') )  //������ϴ���
		{
			rec_buf[rec_num-1]='\0'; //��\0�滻\r,���յ�������ת��Ϊһ���ַ���
			rec_sta=1;   //�ý�����ɱ�־
			rec_num=0; 
		}
		else
		{
			rec_buf[rec_num++]=rec_data;
		}
		
	}
	else
	{
		USART1->DR;		
	}
	
}





//�����ַ���
void uart_send_string(char *p)
{
	while(*p!='\0')
	{
		while((USART1->SR &(0X01<<7))==0);
		USART1->DR=*p;
		p++;
	}
}







#pragma import(__use_no_semihosting_swi) //ȡ��������״̬

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) {
	while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
  return (ch);
}
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}




