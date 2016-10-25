#include "stm32f4xx.h"
#include "led.h"

void led_init(void)
{
//1）	端口时钟使能PC PF
	RCC->AHB1ENR |= (0X01<<2) | (0X01<<5);	
//2）	把相应管脚配置为 普通功能 推挽输出 不用上下拉
	GPIOC->MODER &= ~(0x03<<0);
	GPIOC->MODER |=0x01<<0;     //PC0配置为普通功能输出
	GPIOC->OTYPER &=~(0X01<<0);  //推挽输出
	GPIOC->OSPEEDR &=~(0X03<<0);  //输出最大速度2M
	GPIOC->PUPDR  &=~(0X03<<0);  //不用上下拉
	
	GPIOF->MODER &= ~(0x03<<12 |0x03<<18 |0x03<<20 );
	GPIOF->MODER |=(0x01<<12 |0x01<<18 |0x01<<20 );     //PC0配置为普通功能输出
	GPIOF->OTYPER &=~(0X01<<6 | 0X01<<9 | 0X01<<10);  //推挽输出
	GPIOF->OSPEEDR &=~(0x03<<12 |0x03<<18 |0x03<<20 );  //输出最大速度2M
	GPIOF->PUPDR  &=~(0x03<<12 |0x03<<18 |0x03<<20 );  //不用上下拉
	
	
//3）	置初始状态----------------------可以用，但是处于非工作状态
	GPIOC->ODR |=0X01<<0;    //灯灭
	GPIOF->ODR |=0X01<<6;    //灯灭
	GPIOF->ODR |=0X01<<9;    //灯灭
	GPIOF->ODR |=0X01<<10;    //灯灭
	
}

