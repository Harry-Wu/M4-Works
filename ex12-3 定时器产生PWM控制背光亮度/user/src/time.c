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

/*****************************
PWM����=ARR*PSC/(84M��168M)
Dutyռ�ձ�=CCR/ARR;
************************/
void time12_pwm_init(void)
{
	//�˿�ʱ��ʹ��PB15
	RCC->AHB1ENR |= 0X01<<1;
	GPIOB->MODER &= ~(0X03<<30);
	GPIOB->MODER |= 0x02<<30; //���ù���
	GPIOB->OTYPER &= ~(0X01<<15);  //�������
	GPIOB->OSPEEDR &= ~(0X03<<30);  //����ٶ�2M
	GPIOB->PUPDR &= ~(0X03<<30);  //��������
	GPIOB->AFR[1] &= 0x0FFFFFFF;  //����15λ
	GPIOB->AFR[1] |= 0x90000000;  //AF9
	
	
//	1.	��ʱ��ʱ��ʹ��
	RCC->APB1ENR |=0X01<<6;
//2.	����ARR���Զ���װ��ֵ����PSC��Ԥ��Ƶֵ��
	TIM12->ARR =1000-1;  
	TIM12->PSC =84-1;  //1KHzƵ��
	TIM12->CCR2 = TIM12->ARR>>1; //50%ռ�ձ�
//3.	ʹ�ܸ��£���������UGλ�ܲ��������¼�
	TIM12->CR1 |=0X01<<7;	//ARR�л���
	TIM12->CR1 &=~(0X01<<3); //��ʹ�õ�����ģʽ
	TIM12->CR1 &=~(0X03<<1); //ʹ�ܸ��£�����UGλ���������
	
//ͨ��2����
	TIM12->CCMR1 &= 0X03<<8;  //���ģʽ
	TIM12->CCMR1 |= 0X01<<11;  //CCR2�л���
	TIM12->CCMR1 &= ~(0x07<<12);
	TIM12->CCMR1 |= 0x06<<12;  //PWM1ģʽ
	TIM12->CCER &= ~(0X01<<5);  //�ߵ�ƽ��Ч
	TIM12->CCER |= 0X01<<4; //����ͨ��2
	
	
//4.	����UGλ���������¼�
	TIM12->EGR |=0X01;
//5.	��������¼���־
	TIM12->SR &=~(0X01);
	//��ʱ���ж�ʹ��
	TIM12->DIER |= 0X01;
	
	//NVIC_SetPriority(TIM12_DAC_IRQn,NVIC_EncodePriority(7-2,1,1));
	//��NVIC��ʹ�ܶ�ʱ���ж�
	//NVIC_EnableIRQ(TIM12_DAC_IRQn);
	
//6.	ʹ�ܶ�ʱ��
	TIM12->CR1 |=0X01<<0;
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