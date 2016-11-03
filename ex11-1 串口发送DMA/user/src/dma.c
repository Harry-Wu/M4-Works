#include "stm32f4xx.h"

//par:外设地址
void dma_init(u32 par)
{
	//DMA时钟使能
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;


	//数据流7
	DMA2_Stream7->CR &= ~(DMA_SxCR_EN);  //关闭数据流
	DMA2_Stream7->CR &= ~(DMA_SxCR_CHSEL);  //清零
	DMA2_Stream7->CR |= DMA_SxCR_CHSEL_2; //设为通道4
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_MBURST); //存储器单次传输模式
	DMA2_Stream7->CR &= ~(DMA_SxCR_PBURST);  //外设单次传输模式
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_PL);  //低优先级
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_MSIZE); //外设数据宽度8位
	
	DMA2_Stream7->CR |= DMA_SxCR_MINC;  //存储器地址递增模式
	DMA2_Stream7->CR &= ~(DMA_SxCR_PINC);  //外设地址非增量模式
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_CIRC);  //非循环模式
	
	DMA2_Stream7->CR &= ~(DMA_SxCR_DIR);
	DMA2_Stream7->CR |= DMA_SxCR_DIR_0;  //01:存储器到外设
	
	DMA2_Stream7->PAR = par;  //外设地址
	
	DMA2_Stream7->FCR &= ~(DMA_SxFCR_DMDIS);  //使能直接模式, 此模式下外设跟寄存器数据宽度需一致
	
	//使能串口发送DMA
	USART1->CR3 |= USART_CR3_DMAT;			
}

//mar:存储器地址
//ndtr:传输的数据量
void dma2_stream7_tranf(u32 mar, u16 ndtr)
{
	DMA2->HIFCR |= DMA_HIFCR_CTCIF7;  //清传输完成中断标志
	DMA2_Stream7->CR &= ~(DMA_SxCR_EN);  //关闭数据流
	while(DMA2_Stream7->CR & DMA_SxCR_EN);  //等待数据流关闭成功
	DMA2_Stream7->M0AR = mar;
	DMA2_Stream7->NDTR = ndtr;
	DMA2_Stream7->CR |= DMA_SxCR_EN;  //开启数据流	

}
