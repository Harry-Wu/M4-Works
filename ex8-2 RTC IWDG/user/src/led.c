#include "stm32f4xx.h"
#include "led.h"





void led_init(void)
{
	//端口时钟使能pf口
	//RCC->AHB1ENR=RCC->AHB1ENR|(0X01<<5);
	RCC->AHB1ENR |= 0X01<<5;
	
	//端口模式设置
	//模式----通用输出模式（01）
	GPIOF->MODER &=~(0x03<<18 |0x03<<20);
	GPIOF->MODER |=0X01<<18 | 0X01<<20;
	//输出类型----推挽
	GPIOF->OTYPER &= ~(0X01<<9 | 0X01<<10);	
	//输出速度----50M
	GPIOF->OSPEEDR &=~(0x03<<18 |0x03<<20);
	GPIOF->OSPEEDR |=0x02<<18 | 0x02<<20;
	//上下拉-----无上拉或下拉
	GPIOF->PUPDR &=~(0x03<<18 |0x03<<20); 
	//端口初始状态设置
	LED1=1;
	LED2=1;
}


