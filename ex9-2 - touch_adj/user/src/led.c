#include "stm32f4xx.h" 
#include "led.h"



void led_init(void)
{
	//端口时钟PF
	RCC->AHB1ENR |=0X01<<5;
	//PF6 9 10普通功能推挽输出（无上下拉）
	GPIOF->MODER &=~((0X3<<16)| (0X3<<18)|(0X3<<20));
	GPIOF->MODER |=((0X1<<16)| (0X1<<18)|(0X1<<20)) ;  //普通功能输出
	GPIOF->OTYPER &=~((0X01<<8)| (0X01<<9)| (0X01<<10));//推挽输出
	GPIOF->OSPEEDR &=~((0X3<<16)| (0X3<<18)|(0X3<<20));  //输出速度2M
	GPIOF->PUPDR &=~((0X3<<16)| (0X3<<18)|(0X3<<20));    //无上/下拉
	
	//端口时钟Pc
	RCC->AHB1ENR |=0X01<<2;
	//Pc0普通功能推挽输出（无上下拉）
	GPIOC->MODER &=~(0X3<<0);
	GPIOC->MODER |=(0X1<<0) ;  //普通功能输出
	GPIOC->OTYPER &=~(0X01<<0);//推挽输出
	GPIOC->OSPEEDR &=~(0X3<<0);  //输出速度2M
	GPIOC->PUPDR &=~(0X3<<0);    //无上/下拉
	
	BEEP=0;
	LED5=1;
	LED6=1;
}
