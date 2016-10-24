#include "stm32f4xx.h"


void adc1_init(void)
{
	//端口时钟使能
	RCC->AHB1ENR |=0X01<<0;
	GPIOA->MODER |=(0X03<<8); //pa配置为模拟模式
	//ADC模块时钟使能
	RCC->APB2ENR|=0X01<<8;
	
	ADC1->CR1 &=~(0x03<<24);  //分辨率12位
	ADC1->CR1 &=~(0x01<<8);   //非扫描模式
	
	ADC1->CR2 &=~(0x01<<11);   //右对齐
	ADC1->CR2 |=(0x01<<10);   //每个规则转换结束EOC被置1
	ADC1->CR2 &=~(0x01<<1);   //单次转换
	
	ADC->CCR &=~(0X03<<16);
	ADC->CCR |=0X01<<16;    //adcclk=84M/4=21M
	ADC1->SMPR2 &=~(0X07<<12); 
	ADC1->SMPR2 |=0X07<<12;    //采样480个ADCCLK   转换时间23.4us
	
	ADC1->SQR1 &=~(0X0F<<20);   //规则组一个转换
	ADC1->SQR3 &=~(0X1F<<0);
	ADC1->SQR3 |=0X04<<0;     //转换的通道号为4
	
	ADC1->CR2 |=0X01<<0;    //开启ADC
}



u16 get_adc1_ch4(void)
{
	u16 buf[10],i,j,a;
	for(i=0;i<10;i++)
	{
		//开始转换
		ADC1->CR2 |=0X01<<30;
		//等待转换结束
		while(!(ADC1->SR &(0X01<<1)));
		//读取转换结果
		buf[i]=ADC1->DR;
	}

	for(i=0;i<9;i++)
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				a=buf[i];
				buf[i]=buf[j];
				buf[j]=a;
			}
		}
			a=0;
		for(j=1;j<9;j++)
		{
			a+=buf[j];
		}
		return a/8;
}






