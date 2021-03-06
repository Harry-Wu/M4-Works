#include "stm32f4xx.h"


//par:外设地址

void dma_init(u32 par)
{
	//DMA时钟使能
	RCC->AHB1ENR |=0X01<<22;

	DMA2_Stream2->CR &=~(0x01);   		//关闭数据流
	DMA2_Stream2->CR &=~(0X07<<25);  
	DMA2_Stream2->CR |=0X04<<25;  		 //设为通道4
	
	DMA2_Stream2->CR &=~(0X03<<23); 	//存储器单次传输模式
	DMA2_Stream2->CR &=~(0X03<<21); 	//外设单次传输模式
	
	DMA2_Stream2->CR &=~(0X03<<16);		//设定为低优先级
	
	DMA2_Stream2->CR &=~(0X03<<11);  //外设数据宽度位8位
	
	DMA2_Stream2->CR  |=0x01<<10 ;  	//存储器地址增量模式
	DMA2_Stream2->CR &=~(0X01<<9);   	//外设地址非增量模式
	
	DMA2_Stream2->CR  &=~(0x01<<8) ;  //非循环模式
	
	DMA2_Stream2->CR &=~(0X03<<6);  //外设到存储器
	
	DMA2_Stream2->PAR  =par;

	DMA2_Stream2->FCR &=~(0X01<<2);  //直接传输模式,外设跟存储器数据宽度需要一致	
	//开串口DMA接收
	USART1->CR3|=0X01<<6;  
}



//mar:存储器地址
//ndtr:传输的数据量
void dma2_stream2_tranf(u32 mar,u16 ndtr)
{
	USART1->CR1|=0X01<<2;    //开接收
	USART1->DR;
	DMA2->LIFCR |=0X01<<21;
	DMA2_Stream2->CR &=~(0x01);   //关闭数据流
	while(DMA2_Stream2->CR &0x01);  //等待数据流关闭成功
	DMA2_Stream2->M0AR =mar;
	DMA2_Stream2->NDTR =ndtr;
	DMA2_Stream2->CR |=0x01;   //开启数据流	
	DMA2->LIFCR |=0X01<<21;
	
}




