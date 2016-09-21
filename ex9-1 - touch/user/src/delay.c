#include "stm32f4xx.h" 
#include "delay.h"
//#include "MyTypeDef.h"

//��ʱn΢��
//����n: us
void delay_us(uint32_t n)
{
	SysTick->LOAD =n*SYSCLK/8; //�趨��������
	SysTick->VAL =0;     //��װ�أ���װ�ؼĴ�����ֵ���Ƶ���������
	SysTick->CTRL |=0x01; //��systick��ʱ��
	while(!(SysTick->CTRL&(0x01<<16)));
	SysTick->CTRL &=~(0x01); //��systick��ʱ��
}

//��ʱn����
//����n: ms  (�����ʱ798.9ms)
void delay_ms(u32 n)
{
	SysTick->LOAD =n*SYSCLK*1000/8; //�趨��������
	SysTick->VAL =0;     //��װ�أ���װ�ؼĴ�����ֵ���Ƶ���������
	SysTick->CTRL |=0x01; //��systick��ʱ��
	while(!(SysTick->CTRL&(0x01<<16)));
	SysTick->CTRL &=~(0x01); //��systick��ʱ��
}
