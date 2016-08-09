#include "stm32f4xx.h"

void led_init(void)
{
	//�˿�ʱ��ʹ��PF��
	RCC->AHB1ENR |= 1<<5;
	//���ùܽ�--PF9,PF10����Ϊ��ͨ���ܡ����졢�������������
	GPIOF->MODER &= ~(0x03<<18);  //PF9, 18,19λ����
	GPIOF->MODER |= 0x01<<18;  //18λ��1
	
	GPIOF->MODER &= ~(0x03<<20);  //PF10, 20,21λ����
	GPIOF->MODER |= 0x01<<20;  //20λ��1
	
	GPIOF->OTYPER &= ~(0x01<<9);  //�������
	GPIOF->OTYPER &= ~(0x01<<10);
	
	GPIOF->OSPEEDR &= ~(0x03<<18);  //�������ٶ�2M
	
	GPIOF->PUPDR &= ~(0x03<<18); //��������
	
	//�趨��ʼ״̬---�ǹ���״̬
	GPIOF->ODR |= 0x03<<9;
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
		GPIOF->ODR &= ~(0x01<<9);  //LED ON
		GPIOF->ODR &= ~(0x01<<10);  //LED ON
		delay();
		GPIOF->ODR |= 0x01<<9;  //LED OFF
		GPIOF->ODR |= 0x01<<10;  //LED OFF
		delay();
		
	}
}
