#include "stm32f4xx.h"

void adc1_init(void)
{
	//端口时钟使能
	RCC->AHB1ENR |= 0X01<<0;
	//PA4管脚配置为模拟模式
	GPIOA->MODER &= ~(0X03<<8);
	GPIOA->MODER |= 0X03<<8;
	//adc1模块时钟使能
	RCC->APB2ENR |= 0X01<<8;
	//adc模块初始化
	ADC1->CR1 &= ~(0X03<<24);  //分辨率12位
	ADC1->CR1 &= ~(0X01<<8);  //非扫描模式
	
	ADC1->CR2 &= ~(0X01<<11);  //转换结果右对齐
	ADC1->CR2 &= ~(0X01<<1);  //单次转换模式
	
	
	
	ADC1->SMPR2 &= ~(0x07<<12); 
	ADC1->SMPR2 |= 0x07<<12; //通道4采样时间选择480个周期
	
	ADC1->SQR1 &= ~(0X0F<<20);  //规则通道一个转换
	ADC1->SQR3 &= ~(0X1F<<0);  //
	ADC1->SQR3 |= 0X04<<0;  //第一个转换为通道4
	
	//ADC通用寄存器设置, 适用于所有ADC模块
	ADC->CCR &= ~(0X03<<16);
	ADC->CCR |= 0X01<<16;  //4分频, ADCCLK=84M/4=21MHz
	
	ADC1->CR2 |= 0X01<<0;  //开启ADC1
}

//多次采样求平均值
u16 get_adc(void)
{
	u16 sum=0, i;
	for(i=0; i<10; i++)
	{
		ADC1->CR2 |= 0X01<<30;  //软件触发开
		while(!(ADC1->SR & (0X01<<1)));  //等待转换结束
		sum += ADC1->DR;
	}
	return sum/10;  //标志位自动清除
}
