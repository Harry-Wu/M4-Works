#include "stm32f4xx.h"


void adc1_init(void)
{
	//�˿�ʱ��ʹ��
	RCC->AHB1ENR |=0X01<<0;
	GPIOA->MODER |=(0X03<<8); //pa����Ϊģ��ģʽ
	//ADCģ��ʱ��ʹ��
	RCC->APB2ENR|=0X01<<8;
	
	ADC1->CR1 &=~(0x03<<24);  //�ֱ���12λ
	ADC1->CR1 &=~(0x01<<8);   //��ɨ��ģʽ
	
	ADC1->CR2 &=~(0x01<<11);   //�Ҷ���
	ADC1->CR2 |=(0x01<<10);   //ÿ������ת������EOC����1
	ADC1->CR2 &=~(0x01<<1);   //����ת��
	
	ADC->CCR &=~(0X03<<16);
	ADC->CCR |=0X01<<16;    //adcclk=84M/4=21M
	ADC1->SMPR2 &=~(0X07<<12); 
	ADC1->SMPR2 |=0X07<<12;    //����480��ADCCLK   ת��ʱ��23.4us
	
	ADC1->SQR1 &=~(0X0F<<20);   //������һ��ת��
	ADC1->SQR3 &=~(0X1F<<0);
	ADC1->SQR3 |=0X04<<0;     //ת����ͨ����Ϊ4
	
	ADC1->CR2 |=0X01<<0;    //����ADC
}



u16 get_adc1_ch4(void)
{
	u16 buf[10],i,j,a;
	for(i=0;i<10;i++)
	{
		//��ʼת��
		ADC1->CR2 |=0X01<<30;
		//�ȴ�ת������
		while(!(ADC1->SR &(0X01<<1)));
		//��ȡת�����
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






