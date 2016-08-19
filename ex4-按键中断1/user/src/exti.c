#include "stm32f4xx.h"
#include "led.h"

void exti_init(void)
{
	//�����ӦIO���������ط���ʼ�����ˣ��Ͳ�����
	
	
	EXTI->EMR &= ~(0x01<<0);  //��ֹ�¼��ж�
	EXTI->RTSR |= 0x01<<0; //ʹ�������ش���
	EXTI->FTSR &= ~(0x01<<0);//��ֹ�½��ش���
	EXTI->IMR |= 0x01<<0;//ʹ��exti0�ⲿ�ж�
	
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(7-2, 3, 3));
	NVIC_EnableIRQ(EXTI0_IRQn);  //�ⲿ�ж�ʹ��
	
	
}

void EXTI0_IRQHandler(void)
{
	//���־
	EXTI->PR |= 0x01;
	LED2 = !LED2;
}

