#include "stm32f4xx.h"

//par:�����ַ
void dma_init(u32 par)
{
	//DMAʱ��ʹ��
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;


	//������7
	DMA2_Stream7->CR &= ~(DMA_SxCR_EN);  //�ر�������
	DMA2_Stream7->CR &= ~(DMA_SxCR_CHSEL);  //����
	DMA2_Stream7->CR |= DMA_SxCR_CHSEL_2; //��Ϊͨ��4
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_MBURST); //�洢�����δ���ģʽ
	DMA2_Stream7->CR &= ~(DMA_SxCR_PBURST);  //���赥�δ���ģʽ
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_PL);  //�����ȼ�
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_MSIZE); //�������ݿ��8λ
	
	DMA2_Stream7->CR |= DMA_SxCR_MINC;  //�洢����ַ����ģʽ
	DMA2_Stream7->CR &= ~(DMA_SxCR_PINC);  //�����ַ������ģʽ
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_CIRC);  //��ѭ��ģʽ
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_DIR);
	DMA2_Stream7->CR |= DMA_SxCR_DIR_0;  //01:�洢��������
	
	DMA2_Stream7->PAR = par;  //�����ַ
	
	DMA2_Stream7->FCR &= ~(DMA_SxFCR_DMDIS);  //ʹ��ֱ��ģʽ, ��ģʽ��������Ĵ������ݿ����һ��
	
	//ʹ�ܴ��ڷ���DMA
	USART1->CR3 |= USART_CR3_DMAT;			
}

//mar:�洢����ַ
//ndtr:�����������
void dma2_stream7_tranf(u32 mar, u16 ndtr)
{
	DMA2->HIFCR |= DMA_HIFCR_CTCIF7;  //�崫������жϱ�־
	DMA2_Stream7->CR &= ~(DMA_SxCR_EN);  //�ر�������
	while(DMA2_Stream7->CR & DMA_SxCR_EN);  //�ȴ��������رճɹ�
	DMA2_Stream7->M0AR = mar;
	DMA2_Stream7->NDTR = ndtr;
	DMA2_Stream7->CR |= DMA_SxCR_EN;  //����������	

}
