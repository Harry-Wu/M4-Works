#include "stm32f4xx.h"
#include "led.h"
void exti0_init(void)
{
//	1. PA��ʱ��ʹ��
	RCC->AHB1ENR |=0X01<<0;
//2. ��PA0����Ϊ��ͨ���ܸ�������
	GPIOA->MODER &=~(0X03<<0);
	GPIOA->PUPDR &=~(0X03<<0);
//3. �����ⲿ�ж���0�����ش���,��ֹ�½��ش���
	EXTI->RTSR |=0X01<<0;
	EXTI->FTSR &=~(0X01<<0);	
//4. �¼����μĴ�����Ӧλ��0
	EXTI->EMR  &=~(0X01<<0);	
//5. ������־��
	EXTI->PR  |=(0X01<<0);	
//6. �������ȼ����������ȼ�
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(7-2,1,1));
//7. ��NVIC��ʹ���ⲿ�ж���0�ж�
	NVIC_EnableIRQ(EXTI0_IRQn);
//5. �ж����μĴ�����Ӧλ��1
	EXTI->IMR |=0X01<<0;
}

void exti6_soft_init(void)
{

//3. ���α��ش���
	EXTI->RTSR &=~(0X01<<6);
	EXTI->FTSR &=~(0X01<<6);	
//4. �¼����μĴ�����Ӧλ��0
	EXTI->EMR  &=~(0X01<<6);	
//5. ������־��
	EXTI->PR  |=(0X01<<6);	
//6. �������ȼ����������ȼ�
	NVIC_SetPriority(EXTI9_5_IRQn,NVIC_EncodePriority(7-2,1,1));
//7. ��NVIC��ʹ���ⲿ�ж���0�ж�
	NVIC_EnableIRQ(EXTI9_5_IRQn);
//5. �ж����μĴ�����Ӧλ��1
	EXTI->IMR |=0X01<<6;
}


void exti2_3_4_init(void)
{
//	1. PE��ʱ��ʹ��
	RCC->AHB1ENR |=0X01<<4;
//2. ��PE2 3 4����Ϊ��ͨ���ܸ�������
	GPIOE->MODER &=~(0X03<<4 |0X03<<6 |0X03<<8);
	GPIOE->PUPDR &=~(0X03<<4 |0X03<<6 |0X03<<8);
//ʹ��ϵͳ����ʱ��
	RCC->APB2ENR |=0X01<<14;
	SYSCFG->EXTICR[0] &=~(0XFF<<8);
	SYSCFG->EXTICR[0] |=0X44<<8;  
	
	SYSCFG->EXTICR[1] &=~(0XF<<0);
	SYSCFG->EXTICR[1] |=0X4<<0;  
//3. �����ⲿ�ж���0�����ش���,��ֹ�½��ش���
	EXTI->RTSR &=~(0X07<<2);
	EXTI->FTSR |=0x07<<2;	
//4. �¼����μĴ�����Ӧλ��0
	EXTI->EMR  &=~(0X07<<2);	
//5. ������־��
	EXTI->PR  |=(0X07<<2);	
//6. �������ȼ����������ȼ�
	NVIC_SetPriority(EXTI2_IRQn,NVIC_EncodePriority(7-2,1,1));
	NVIC_SetPriority(EXTI3_IRQn,NVIC_EncodePriority(7-2,1,1));
	NVIC_SetPriority(EXTI4_IRQn,NVIC_EncodePriority(7-2,1,1));
	
//7. ��NVIC��ʹ���ⲿ�ж����ж�
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
//5. �ж����μĴ�����Ӧλ��1
	EXTI->IMR |=0X07<<2;
}






void EXTI0_IRQHandler(void)
{
	//���жϱ�־
	EXTI->PR  |=(0X01<<0);	
	LED1=!LED1;
}

void EXTI2_IRQHandler(void)
{
	//���жϱ�־
	EXTI->PR  |=(0X01<<2);	
	LED2=!LED2;
}

void EXTI3_IRQHandler(void)
{
	//���жϱ�־
	EXTI->PR  |=(0X01<<3);	
	LED3=!LED3;
}

void EXTI4_IRQHandler(void)
{
	//���жϱ�־
	EXTI->PR  |=(0X01<<4);	
	LED4=!LED4;
}


void EXTI9_5_IRQHandler(void)
{
	//�ж����ĸ��ж����ϵ��ж�
	if(EXTI->PR &(0X01<<6))
	{
		EXTI->PR |=0X01<<6;
		LED2=!LED2;
	}
}




