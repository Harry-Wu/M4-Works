#include "stm32f4xx.h"

//par:外设地址
void dma_init(u32 par)
{
	//DMA时钟使能
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_EN);  //关闭数据流
	
	
	
}