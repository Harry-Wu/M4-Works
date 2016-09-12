#include "stm32f4xx.h" 
#include "LED.h" 

void exti0_init(void)
{
	//u8 prior;
	//����IO������Ϊ����
	
	//ϵͳ���ÿ�����ʱ��ʹ�� 
	RCC->APB2ENR |=0X01<<14;
	
	SYSCFG->EXTICR[0] &=~(0X0F<<0);
	
	
	
	EXTI->IMR |=0X01<<0;   //ʹ��exti0�ж�
	EXTI->EMR &=~(0X01<<0);  //����exti0�¼�����
	EXTI->RTSR |=(0X01<<0); //���� exti0�����ش���
	EXTI->FTSR &=~(0X01<<0);     // ��ֹexti0�½��ش���
	
	//prior=NVIC_EncodePriority(7-2,1,1);  //�������ȼ�
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(7-2,1,1));  //�������ȼ�
	NVIC_EnableIRQ(EXTI0_IRQn); //�ⲿ�ж�ʹ�ܣ�ϵͳ�ж�û�����ʹ�ܣ�
	
}

void exti2_init(void)
{
	//u8 prior;
	//����IO������Ϊ����
	
	//ϵͳ���ÿ�����ʱ��ʹ�� 
	RCC->APB2ENR |=0X01<<14;
	
	SYSCFG->EXTICR[0] &=~(0X0F<<8);
	SYSCFG->EXTICR[0] |=0X04<<8;    //exti2ӳ�䵽PE2
	
	
	EXTI->IMR |=0X01<<2;   //ʹ��exti2�ж�
	EXTI->EMR &=~(0X01<<2);  //����exti2�¼�����
	EXTI->RTSR &=~(0X01<<2); //��ֹ exti2�����ش���
	EXTI->FTSR |=0X01<<2;     // ����exti2�½��ش���
	
	//prior=NVIC_EncodePriority(7-2,1,1);  //�������ȼ�
	NVIC_SetPriority(EXTI2_IRQn,NVIC_EncodePriority(7-2,1,1));  //�������ȼ�
	NVIC_EnableIRQ(EXTI2_IRQn); //�ⲿ�ж�ʹ�ܣ�ϵͳ�ж�û�����ʹ�ܣ�
	
}


void EXTI0_IRQHandler(void)
{
	EXTI->PR |=0X01<<0;  //������־
	LED2=!LED2;
}



void EXTI2_IRQHandler(void)
{
	EXTI->PR |=0X01<<2;  //������־
	LED1=!LED1;
}

