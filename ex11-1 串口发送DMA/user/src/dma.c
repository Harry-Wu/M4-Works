#include "stm32f4xx.h"

//par:�����ַ
void dma_init(u32 par)
{
	//DMAʱ��ʹ��
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_EN);  //�ر�������
	
	
	
}