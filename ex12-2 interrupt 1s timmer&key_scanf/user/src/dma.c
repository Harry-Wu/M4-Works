#include "stm32f4xx.h"


//par:�����ַ

void dma_init(u32 par)
{
	//DMAʱ��ʹ��
	RCC->AHB1ENR |=0X01<<22;

	DMA2_Stream2->CR &=~(0x01);   		//�ر�������
	DMA2_Stream2->CR &=~(0X07<<25);  
	DMA2_Stream2->CR |=0X04<<25;  		 //��Ϊͨ��4
	
	DMA2_Stream2->CR &=~(0X03<<23); 	//�洢�����δ���ģʽ
	DMA2_Stream2->CR &=~(0X03<<21); 	//���赥�δ���ģʽ
	
	DMA2_Stream2->CR &=~(0X03<<16);		//�趨Ϊ�����ȼ�
	
	DMA2_Stream2->CR &=~(0X03<<11);  //�������ݿ��λ8λ
	
	DMA2_Stream2->CR  |=0x01<<10 ;  	//�洢����ַ����ģʽ
	DMA2_Stream2->CR &=~(0X01<<9);   	//�����ַ������ģʽ
	
	DMA2_Stream2->CR  &=~(0x01<<8) ;  //��ѭ��ģʽ
	
	DMA2_Stream2->CR &=~(0X03<<6);  //���赽�洢��
	
	DMA2_Stream2->PAR  =par;

	DMA2_Stream2->FCR &=~(0X01<<2);  //ֱ�Ӵ���ģʽ,������洢�����ݿ����Ҫһ��	
	//������DMA����
	USART1->CR3|=0X01<<6;  
}



//mar:�洢����ַ
//ndtr:�����������
void dma2_stream2_tranf(u32 mar,u16 ndtr)
{
	USART1->CR1|=0X01<<2;    //������
	USART1->DR;
	DMA2->LIFCR |=0X01<<21;
	DMA2_Stream2->CR &=~(0x01);   //�ر�������
	while(DMA2_Stream2->CR &0x01);  //�ȴ��������رճɹ�
	DMA2_Stream2->M0AR =mar;
	DMA2_Stream2->NDTR =ndtr;
	DMA2_Stream2->CR |=0x01;   //����������	
	DMA2->LIFCR |=0X01<<21;
	
}




