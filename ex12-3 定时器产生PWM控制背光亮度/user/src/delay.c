#include "stm32f4xx.h"
#include "delay.h"

void delay_us(u32 nus)
{
	u32 sta;
//	1.	ѡ��ʱ��Դsysclk/8
	SysTick->CTRL &=~(0x01<<2);
//2.	����load�Ĵ���-----�趨������ֵ
	SysTick->LOAD=nus*SYSCLK/8;
//3.	��VALд����------VAL�ᱻ����?��װ��
	SysTick->VAL=0;
//4.	����SYSTICK
	SysTick->CTRL |=0x01<<0;
//5.	�ȴ���ʱʱ�䵽����ѯCTRL.16�Ƿ�Ϊ1��
	do{
		sta=SysTick->CTRL;
	}
	while((sta & 0x01)&&!(sta &(0x01<<16))); //systick�ǿ����ģ����Ҷ�ʱʱ�仹û��
//6.	��SYSTICK��ʱ��
	SysTick->CTRL &=~(0x01<<0);
}

void delya_us_timer(u32 us)
{
//	1.	��ʱ��ʱ��ʹ��
	RCC->APB1ENR |=0X01<<5;
//2.	����ARR���Զ���װ��ֵ����PSC��Ԥ��Ƶֵ��
	//Tout=((arr+1)*(psc+1))/Ft us.  Ft=84MHz
	TIM7->ARR =84-1;
	TIM7->PSC =1*us-1;
//3.	ʹ�ܸ��£���������UGλ�ܲ��������¼�
	TIM7->CR1 |=0X01<<7;	//ARR�л���
	TIM7->CR1 &=~(0X01<<3); //��ʹ�õ�����ģʽ
	TIM7->CR1 &=~(0X03<<1); //ʹ�ܸ��£�����UGλ���������
//4.	����UGλ���������¼�
	TIM7->EGR |=0X01;
//5.	��������¼���־
	TIM7->SR &=~(0X01);
//6.	ʹ�ܶ�ʱ��
	TIM7->CR1 |=0X01<<0;
	while(!(TIM7->SR &(0X01)));//���û���жϱ�־ʱ����������һ������
	TIM7->SR &=~(0X01);   //������±�־
	TIM7->CR1 &=~(0X01<<0); //���ܶ�ʱ��
}

void delya_ms_timer(u32 ms)
{
//	1.	��ʱ��ʱ��ʹ��
	RCC->APB1ENR |=0X01<<5;
//2.	����ARR���Զ���װ��ֵ����PSC��Ԥ��Ƶֵ��
	//Tout=((arr+1)*(psc+1))/Ft us.  Ft=84MHz
	TIM7->ARR =84*ms-1;
	TIM7->PSC =1000-1;
//3.	ʹ�ܸ��£���������UGλ�ܲ��������¼�
	TIM7->CR1 |=0X01<<7;	//ARR�л���
	TIM7->CR1 &=~(0X01<<3); //��ʹ�õ�����ģʽ
	TIM7->CR1 &=~(0X03<<1); //ʹ�ܸ��£�����UGλ���������
//4.	����UGλ���������¼�
	TIM7->EGR |=0X01;
//5.	��������¼���־
	TIM7->SR &=~(0X01);
//6.	ʹ�ܶ�ʱ��
	TIM7->CR1 |=0X01<<0;
	while(!(TIM7->SR &(0X01)));//���û���жϱ�־ʱ����������һ������
	TIM7->SR &=~(0X01);   //������±�־
	TIM7->CR1 &=~(0X01<<0); //���ܶ�ʱ��
}


//�����ʱ798.9ms
void delay_ms(u32 nms)
{
	u32 sta;
//	1.	ѡ��ʱ��Դsysclk/8
	SysTick->CTRL &=~(0x01<<2);
//2.	����load�Ĵ���-----�趨������ֵ
	SysTick->LOAD=nms*1000*SYSCLK/8;
//3.	��VALд����------VAL�ᱻ����?��װ��
	SysTick->VAL=0;
//4.	����SYSTICK
	SysTick->CTRL |=0x01<<0;
//5.	�ȴ���ʱʱ�䵽����ѯCTRL.16�Ƿ�Ϊ1��
	do{
		sta=SysTick->CTRL;
	}
	while((sta & 0x01)&&!(sta &(0x01<<16))); //systick�ǿ����ģ����Ҷ�ʱʱ�仹û��
//6.	��SYSTICK��ʱ��
	SysTick->CTRL &=~(0x01<<0);
}



void systick_interrupt_init(u32 nms)
{
	u32 sta;
//	1.	ѡ��ʱ��Դsysclk/8
	SysTick->CTRL &=~(0x01<<2);
//2.	����load�Ĵ���-----�趨������ֵ
	SysTick->LOAD=nms*1000*SYSCLK/8;
//3.	��VALд����------VAL�ᱻ����?��װ��
	SysTick->VAL=0;
//���ж�
	SysTick->CTRL |=0x01<<1;
	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(7-2,1,1));
	
//4.	����SYSTICK
	SysTick->CTRL |=0x01<<0;

}


