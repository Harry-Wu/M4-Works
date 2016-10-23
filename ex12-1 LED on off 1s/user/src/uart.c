#include "stm32f4xx.h"
#include "stdio.h"

void uart_init(u32 bond)
{
//	1.	�˿�ʱ��ʹ��PA
	RCC->AHB1ENR |=0X01<<0;	
//2.	����Ӧ�˿�����Ϊ���ù���ģʽ�����ø��ù��ܼĴ���
	GPIOA->MODER &=~(0X03<<18 | 0X03 <<20);
	GPIOA->MODER |=0X02<<18 |0X02<<20;//pa9 10����Ϊ���ù���
	GPIOA->OTYPER &=~(0X01<<9);   //pa9(txd) �������
	GPIOA->OSPEEDR &=~(0X03<<18);  //pa9�������ٶ�2M
	GPIOA->PUPDR &=~(0X03<<18 | 0X03 <<20);  //pa9 10 ��ʹ��������
	
	GPIOA->AFR[1] &=0XFFFFF00F;
	GPIOA->AFR[1] |=0X00000770;	
	
	//3.	����ʱ��ʹ��uart1
	RCC->APB2ENR |=0X01<<4;
//4.	���ò����ʡ��������ݳ��ȡ�ֹͣλ���Ƿ�����żУ��
	USART1->BRR =84000000/bond;     //��һ��ǰ�ᣨOVER8=0��
	
	USART1->CR1 &=~(0X01<<15);    //over8=0; 16��������
	USART1->CR1 &=~(0X01<<12);    //���ݳ���Ϊ8λ
	
	
	//5.	�����ա������͡����������Ļ��������ж�
	USART1->CR1|=0X01<<3;   //������
	USART1->CR1|=0X01<<2;    //������
	
	//�����ڽ����ж�
		USART1->CR1|=0X01<<5;    //����������ж�
	//���ô����жϵ����ȼ�
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,1,1));
	//��NVIC��ʹ�ܴ����ж�
	NVIC_EnableIRQ(USART1_IRQn);
//6.	������
	USART1->CR1|=0X01<<13;
}

u8 rec_buf[1000];
static u16 rec_num=0;    //�������ݼ���
u8 rec_sta=0;     //������ϱ�־ rec_sta=1���������

void USART1_IRQHandler(void)
{
	u8 _data;
	if(USART1->SR &(0X01<<5))  //����ǽ����ж�
	{
		_data=USART1->DR ;
		if((_data=='\n') && (rec_buf[rec_num-1]=='\r'))  //�����յ�����\N�ϴ��յ�����\r
		{
			rec_buf[rec_num-1]='\0';//�����յ�������ת�����ַ���
			rec_num=0;    //�����´ν���
			rec_sta=1;    //�ý�����ɱ�־
			
		}
		else
		{
			rec_buf[rec_num++]=_data;
		}
	}
}













void uart_send_string(s8 *p)
{
	while(*p!='\0')
	{
			while(!(USART1->SR &(0X01<<7)));  //�����ͻ�������Ϊ����ȴ�
			USART1->DR=*p++;   
		
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


