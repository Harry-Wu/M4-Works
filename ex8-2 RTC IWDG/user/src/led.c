#include "stm32f4xx.h"
#include "led.h"





void led_init(void)
{
	//�˿�ʱ��ʹ��pf��
	//RCC->AHB1ENR=RCC->AHB1ENR|(0X01<<5);
	RCC->AHB1ENR |= 0X01<<5;
	
	//�˿�ģʽ����
	//ģʽ----ͨ�����ģʽ��01��
	GPIOF->MODER &=~(0x03<<18 |0x03<<20);
	GPIOF->MODER |=0X01<<18 | 0X01<<20;
	//�������----����
	GPIOF->OTYPER &= ~(0X01<<9 | 0X01<<10);	
	//����ٶ�----50M
	GPIOF->OSPEEDR &=~(0x03<<18 |0x03<<20);
	GPIOF->OSPEEDR |=0x02<<18 | 0x02<<20;
	//������-----������������
	GPIOF->PUPDR &=~(0x03<<18 |0x03<<20); 
	//�˿ڳ�ʼ״̬����
	LED1=1;
	LED2=1;
}


