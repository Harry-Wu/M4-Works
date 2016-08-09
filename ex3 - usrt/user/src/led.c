#include "stm32f4xx.h"


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
	
	//���ùܽ�PF10----D6
	GPIOF->MODER &= ~(0x03<<20);  //PF10, 20,21λ����
	GPIOF->MODER |= 0x01<<20;  //20λ��1
	GPIOF->OTYPER &= ~(0x01<<10);
	GPIOF->OSPEEDR &= ~(0x03<<20);  
	GPIOF->PUPDR &= ~(0x03<<20); //��������
	
	//���ùܽ�PF8----LS1
	GPIOF->MODER &= ~(0x03<<16);  //PF8, 20,21λ����
	GPIOF->MODER |= 0x01<<16;  //20λ��1
	GPIOF->OTYPER &= ~(0x01<<8);
	GPIOF->OSPEEDR &= ~(0x03<<16);  
	GPIOF->PUPDR &= ~(0x03<<16); //��������
	
	//�趨��ʼ״̬---�ǹ���״̬
	GPIOF->ODR |= 0x03<<9;  //����PF9,PF10�ߵ�ƽ,D5,D6��
	GPIOF->ODR &= ~(0x01<<8);  //�͵�ƽ,�ر�LS1
}
