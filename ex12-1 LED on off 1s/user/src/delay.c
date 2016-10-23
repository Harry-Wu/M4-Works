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


