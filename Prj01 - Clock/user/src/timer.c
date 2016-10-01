#include "stm32f4xx.h"
#include "led.h"

void timer14_init(u16 arr, u16 psc)
{
	//timer14������PF9(D5)���ù���, ���Ǹ�������жϲ���ҪIO���ø��ù���
    //�ڲ�ʱ��ѡ��
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    //������ģʽ, tim14Ĭ������
	//TIM14->CR1 |=  
    //Ԥ��Ƶϵ��
	TIM14->PSC = psc;
	//�Զ�����ֵ
	TIM14->ARR = arr;
	//��������ж�
	TIM14->DIER |= 0X01<<0;
    //ʹ�ܼ�����
	TIM14->CR1 |=  0X01<<0;
	
	NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn,NVIC_EncodePriority(7-2, 3, 3));
	NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);  //TIM14�ж�ʹ��
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	if(TIM14->SR & 0X01)  //��������жϱ�־
	{
		LED6 = !LED6;
	}
	TIM14->SR &= ~(0X01<<0);  //���ж�
}
