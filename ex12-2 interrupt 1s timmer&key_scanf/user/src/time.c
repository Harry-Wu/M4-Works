#include "stm32f4xx.h"
#include "led.h"
#include "key.h"

void time6_init(void)
{
//	1.	��ʱ��ʱ��ʹ��
	RCC->APB1ENR |=0X01<<4;
//2.	����ARR���Զ���װ��ֵ����PSC��Ԥ��Ƶֵ��
	TIM6->ARR =100-1;
	TIM6->PSC =8400-1;
//3.	ʹ�ܸ��£���������UGλ�ܲ��������¼�
	TIM6->CR1 |=0X01<<7;	//ARR�л���
	TIM6->CR1 &=~(0X01<<3); //��ʹ�õ�����ģʽ
	TIM6->CR1 &=~(0X03<<1); //ʹ�ܸ��£�����UGλ���������
//4.	����UGλ���������¼�
	TIM6->EGR |=0X01;
//5.	��������¼���־
	TIM6->SR &=~(0X01);
	//��ʱ���ж�ʹ��
	TIM6->DIER |= 0X01;
	NVIC_SetPriority(TIM6_DAC_IRQn,NVIC_EncodePriority(7-2,1,1));
	//��NVIC��ʹ�ܶ�ʱ���ж�
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	
//6.	ʹ�ܶ�ʱ��
	TIM6->CR1 |=0X01<<0;
}

void time7_init(void)
{
//	1.	��ʱ��ʱ��ʹ��
	RCC->APB1ENR |=0X01<<5;
//2.	����ARR���Զ���װ��ֵ����PSC��Ԥ��Ƶֵ��
	TIM7->ARR =10000-1;
	TIM7->PSC =8400-1;  //1Hz
//3.	ʹ�ܸ��£���������UGλ�ܲ��������¼�
	TIM7->CR1 |=0X01<<7;	//ARR�л���
	TIM7->CR1 &=~(0X01<<3); //��ʹ�õ�����ģʽ
	TIM7->CR1 &=~(0X03<<1); //ʹ�ܸ��£�����UGλ���������
//4.	����UGλ���������¼�
	TIM7->EGR |=0X01;
//5.	��������¼���־
	TIM7->SR &=~(0X01);
	//��ʱ���ж�ʹ��
	TIM7->DIER |= 0X01;
	NVIC_SetPriority(TIM7_IRQn,NVIC_EncodePriority(7-2,1,1));
	//��NVIC��ʹ�ܶ�ʱ���ж�
	NVIC_EnableIRQ(TIM7_IRQn);
	
//6.	ʹ�ܶ�ʱ��
	TIM7->CR1 |=0X01<<0;
}

u8 key=NO_KEY;
void TIM6_DAC_IRQHandler(void)
{
	u8 key_rec;
	TIM6->SR &= ~(0X01);
	key_rec = key_scanf();
	if(key_rec!=NO_KEY)  //����а�������
	{
		key = key_rec;
	}
}

void TIM7_IRQHandler(void)
{
	TIM7->SR &= ~(0X01);
	LED3 = !LED3;
}