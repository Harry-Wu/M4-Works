/*****************************************************************************
@File name:  
@Description: ��˸LED 
@Author: Harry Wu
@Version: V1.1
@Date: 2016-11-7
@History: 
		V1.0: ������Ĳ����Ĵ���
		V1.1: ����λ������, ��Ŀ��ʽ����Ϊuvprojx

		
*****************************************************************************/
#include "stm32f4xx.h"
//#include "key.h"
//#include "delay.h"
#include "io_bit.h"
#define D5  PFout(9)
#define D6  PFout(10)
#define LS1 PFout(8)

void led_init(void)
{
	//�˿�ʱ��ʹ��PF��
	RCC->AHB1ENR |= 1<<5;
	//���ùܽ�PF9----D5����Ϊ��ͨ���ܡ����졢�������������
	GPIOF->MODER &= ~(0x03<<18);  //PF9, 18,19λ����
	GPIOF->MODER |= 0x01<<18;  //18λ��1
	GPIOF->OTYPER &= ~(0x01<<9);  //�������
	GPIOF->OSPEEDR &= ~(0x03<<18);  //�������ٶ�2M
	GPIOF->PUPDR &= ~(0x03<<18); //��������
	
	//���ùܽ�PF10----D6����Ϊ��ͨ���ܡ����졢�������������
	GPIOF->MODER &= ~(0x03<<20);  //PF10, 20,21λ����
	GPIOF->MODER |= 0x01<<20;  //20λ��1
	GPIOF->OTYPER &= ~(0x01<<10);
	GPIOF->OSPEEDR &= ~(0x03<<20);  //�������ٶ�2M
	GPIOF->PUPDR &= ~(0x03<<20); //��������
	
	//�趨��ʼ״̬---�ǹ���״̬
	GPIOF->ODR |= 0x03<<9;  //����PF9,PF10�ߵ�ƽ,D5,D6��
}

//��ʱ����
void delay()
{
	u32 i=0x1000000;
	while(i--);
}
	

int main(void)
{
	//Ӳ����ʼ��
	led_init();
	
	while(1)
	{
		D5 = 0;  //�͵�ƽ,LED ON
		D6 = 0;  //�͵�ƽLED ON
		delay();
		D5 = 1;  //LED OFF
		D6 = 1;  //LED OFF
		delay();
		
	}
}
