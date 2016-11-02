#include "stm32f4xx.h"
#include "led.h"

void exti_init(void)
{
	//�����ӦIO���������ط���ʼ�����ˣ��Ͳ�����
	
	//ʹ��PA0�ⲿ�ж�
	
	EXTI->EMR &= ~(0x01<<0);  //��ֹ�¼��ж�
	EXTI->RTSR |= 0x01<<0; //ʹ�������ش���
	EXTI->FTSR &= ~(0x01<<0);//��ֹ�½��ش���
	EXTI->IMR |= 0x01<<0;//ʹ��exti0�ⲿ�ж�
	
	
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(7-2, 3, 3));
	NVIC_EnableIRQ(EXTI0_IRQn);  //�ⲿ�ж�ʹ��
	
	
}

void exti2_init(void)
{
	//�����ӦIO���������ط���ʼ�����ˣ��Ͳ�����
	
	//ʹ��PE2�ⲿ�ж�
	
	EXTI->EMR &= ~(0x01<<2);  //��ֹexti2�¼��ж�
	EXTI->RTSR &= ~(0x01<<2); //���������ش���
	EXTI->FTSR |= 0x01<<2;//ʹ���½��ش���
	EXTI->IMR |= 0x01<<2;//ʹ��exti2�ж�
	
	
	RCC->APB2ENR |= 0x01<<14; //ϵͳ���ÿ�����ʱ��ʹ��
	SYSCFG->EXTICR[0] &= ~(0x0F<<8);  //ӳ��PE2��EXTI2
	//SYSCFG->EXTICR[0] |= 0x04<<8;
	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI2_PE;
	
	//SYSCFG_EXTICR1_EXTI2_PE;
	
	
	NVIC_SetPriority(EXTI2_IRQn,NVIC_EncodePriority(7-2, 3, 3));
	NVIC_EnableIRQ(EXTI2_IRQn);  //�ⲿ�ж�ʹ��
	
	
}

//����ж�
void exti9_soft_init(void)
{
	EXTI->EMR &= ~(0x01<<9);  //��ֹexti9�¼��ж�
	EXTI->RTSR &= ~(0x01<<9); //���������ش���
	EXTI->FTSR &= ~(0x01<<9);//ʹ���½��ش���
	EXTI->IMR |= 0x01<<9;//ʹ��exti9�ж�
	
	NVIC_SetPriority(EXTI9_5_IRQn,NVIC_EncodePriority(7-2, 3, 3));
	NVIC_EnableIRQ(EXTI9_5_IRQn);  //�ⲿ�ж�ʹ��
}


void EXTI0_IRQHandler(void)
{
	//��exti0�ı�־
	EXTI->PR |= 0x01;
	LED5 = !LED5;
}

void EXTI2_IRQHandler(void)
{
	//��exti2�ı�־
	EXTI->PR |= 0x01<<2;
	LED6 = !LED6;
}

//����ж�
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & (0x01<<9))
	{
		//��exti2�ı�־
		EXTI->PR |= 0x01<<9;
		//BEEP = !BEEP;
		LED5 = !LED5;
	}
	
	
}

