#include "stm32f4xx.h" 
#include "led.h"



void led_init(void)
{
	//�˿�ʱ��PF
	RCC->AHB1ENR |=0X01<<5;
	//PF6 9 10��ͨ�����������������������
	GPIOF->MODER &=~((0X3<<16)| (0X3<<18)|(0X3<<20));
	GPIOF->MODER |=((0X1<<16)| (0X1<<18)|(0X1<<20)) ;  //��ͨ�������
	GPIOF->OTYPER &=~((0X01<<8)| (0X01<<9)| (0X01<<10));//�������
	GPIOF->OSPEEDR &=~((0X3<<16)| (0X3<<18)|(0X3<<20));  //����ٶ�2M
	GPIOF->PUPDR &=~((0X3<<16)| (0X3<<18)|(0X3<<20));    //����/����
	
	//�˿�ʱ��Pc
	RCC->AHB1ENR |=0X01<<2;
	//Pc0��ͨ�����������������������
	GPIOC->MODER &=~(0X3<<0);
	GPIOC->MODER |=(0X1<<0) ;  //��ͨ�������
	GPIOC->OTYPER &=~(0X01<<0);//�������
	GPIOC->OSPEEDR &=~(0X3<<0);  //����ٶ�2M
	GPIOC->PUPDR &=~(0X3<<0);    //����/����
	
	BEEP=0;
	LED5=1;
	LED6=1;
}
