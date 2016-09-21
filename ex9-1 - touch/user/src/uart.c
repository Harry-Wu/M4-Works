#include "stm32f4xx.h" 
#include "stdio.h"
//#include "MyTypeDef.h"

void uart1_init(u32 bond)
{
	//�˿�ʱ��ʹ��PA
	RCC->AHB1ENR |=0X01<<0;
	//pa9(txd)--�������칦��
  //pa10(rxd)--���ø��չ���
	GPIOA->MODER &=~(0X03<<18 | 0X03<<20);
	GPIOA->MODER |=(0X02<<18 | 0X02<<20);  //pa9 10 ��Ϊ���ù���
	GPIOA->OTYPER &=~(0X01<<9); //pa9�������
	GPIOA->OSPEEDR &=~(0X03<<18); //pa9����ٶ�2M
	GPIOA->PUPDR &=~(0X03<<18 | 0X03<<20); //pa9 10 ����������
	
	GPIOA->AFR[1] &= 0xfffff00f;
	GPIOA->AFR[1] |= 0x00000770;  //pa9 10 ���ù�����Ϊuart1��txd rxd
	
	//uart1ʱ��ʹ��
	RCC->APB2ENR |=0X01<<4;
	//����������
	USART1->BRR =84000000/bond;
	
	USART1->CR1 &=~(0X01<<15); //16��������
	USART1->CR1 &=~(0X01<<12);  //�ֳ�Ϊ8λ
	USART1->CR1 |=0x01<<3; //������
	USART1->CR1 |=0x01<<2; //������	
	USART1->CR2 &=~(0x03<<12); //һ��ֹͣλ
	//�ж�ʹ��
	USART1->CR1 |=0x01<<5; //�������ж�
	
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,3,3));  //�������ȼ�
	NVIC_EnableIRQ(USART1_IRQn); //�ⲿ�ж�ʹ�ܣ�ϵͳ�ж�û�����ʹ�ܣ�
	USART1->CR1 |=0x01<<13; //������	
}


u8 rec_buf[100];   //���ջ�����
u8 rec_sta=0;      //������ɱ�־rec_sta=1�����������
u8 rec_num=0;     //��������������
u32 rec_count_time=0;  //��ʱ����
u8 rec_start=0;    //��ʼ�����ַ�����־
void USART1_IRQHandler(void)
{
	
	if(USART1->SR &(0X01<<5))
	{
		rec_start=1;
		rec_count_time=0;
	
		rec_buf[rec_num++]=USART1->DR;
		
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
int fputc(int ch, FILE *f) {
	while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
  return (ch);
}


