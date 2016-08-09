#include "stm32f4xx.h"
#include "stdio.h"

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
	//�ж�ʹ��
	USART1->CR1 |= 0x01<<5;
	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(7-2,3,3));  //�������ȼ�
	NVIC_EnableIRQ(USART1_IRQn);  //�ⲿ�ж�ʹ��(ϵͳ�ж�û�����ʹ��)
	
	USART1->CR1 |= 0x01<<13;  //������
	
}

u8 rec_buf[100];  //���ջ�����, �������������Ŀռ�,�ͱ��븳ֵ
u8 rec_sta=0; //������ɱ�־, rec_sta=1��ʾ�������
u8 rec_num=0;  //�����ֽ���

void USART1_IRQHandler(void)
{
	u8 rec_data;
	
	if(USART1->SR & (0x01<<5))  //�ж��Ƿ��յ�����
	{
		rec_data = USART1->DR;
		if((rec_data=='\n') && (rec_buf[rec_num-1]=='\r'))  //�ûس����з�\r\n\���ж��Ƿ�������
		{
			rec_sta = 1;
			rec_buf[rec_num-1] = '\0'; //�ַ���������'\0'��Ϊ������־
			rec_num=0; //Ϊ��һ�����ݽ�����׼��
		}
		else
		{
			rec_buf[rec_num++] = rec_data;
		}
	}
}

void uart_send_string(u8 *p)
{
	while(*p!='\0')
	{
		//�ȴ��ϴη���
		while(!(USART1->SR &(0X01<<7)));
		//���յ������ݻط�
		USART1->DR =*p++;
	}
	
}



//��дfputc����
int fputc(int ch, FILE *f) 
{
	while((USART1->SR &(0X01<<7))==0);
	USART1->DR=ch;
	return (ch);
}

