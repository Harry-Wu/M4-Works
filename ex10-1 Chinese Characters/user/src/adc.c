#include "stm32f4xx.h"

void adc1_init(void)
{
	//�˿�ʱ��ʹ��
	RCC->AHB1ENR |= 0X01<<0;
	//PA4�ܽ�����Ϊģ��ģʽ
	GPIOA->MODER &= ~(0X03<<8);
	GPIOA->MODER |= 0X03<<8;
	//adc1ģ��ʱ��ʹ��
	RCC->APB2ENR |= 0X01<<8;
	//adcģ���ʼ��
	ADC1->CR1 &= ~(0X03<<24);  //�ֱ���12λ
	ADC1->CR1 &= ~(0X01<<8);  //��ɨ��ģʽ
	
	ADC1->CR2 &= ~(0X01<<11);  //ת������Ҷ���
	ADC1->CR2 &= ~(0X01<<1);  //����ת��ģʽ
	
	
	
	ADC1->SMPR2 &= ~(0x07<<12); 
	ADC1->SMPR2 |= 0x07<<12; //ͨ��4����ʱ��ѡ��480������
	
	ADC1->SQR1 &= ~(0X0F<<20);  //����ͨ��һ��ת��
	ADC1->SQR3 &= ~(0X1F<<0);  //
	ADC1->SQR3 |= 0X04<<0;  //��һ��ת��Ϊͨ��4
	
	//ADCͨ�üĴ�������, ����������ADCģ��
	ADC->CCR &= ~(0X03<<16);
	ADC->CCR |= 0X01<<16;  //4��Ƶ, ADCCLK=84M/4=21MHz
	
	ADC1->CR2 |= 0X01<<0;  //����ADC1
}

//��β�����ƽ��ֵ
u16 get_adc(void)
{
	u16 sum=0, i;
	for(i=0; i<10; i++)
	{
		ADC1->CR2 |= 0X01<<30;  //����������
		while(!(ADC1->SR & (0X01<<1)));  //�ȴ�ת������
		sum += ADC1->DR;
	}
	return sum/10;  //��־λ�Զ����
}