#include "stm32f4xx.h" 
#include "stdio.h"
//#include "MyTypeDef.h"

void uart1_init(u32 bond)
{
	//端口时钟使能PA
	RCC->AHB1ENR |=0X01<<0;
	//pa9(txd)--复用推挽功能
  //pa10(rxd)--复用浮空功能
	GPIOA->MODER &=~(0X03<<18 | 0X03<<20);
	GPIOA->MODER |=(0X02<<18 | 0X02<<20);  //pa9 10 设为复用功能
	GPIOA->OTYPER &=~(0X01<<9); //pa9推挽输出
	GPIOA->OSPEEDR &=~(0X03<<18); //pa9输出速度2M
	GPIOA->PUPDR &=~(0X03<<18 | 0X03<<20); //pa9 10 不用上下拉
	
	GPIOA->AFR[1] &= 0xfffff00f;
	GPIOA->AFR[1] |= 0x00000770;  //pa9 10 复用功能设为uart1的txd rxd
	
	//uart1时钟使能
	RCC->APB2ENR |=0X01<<4;
	//波特率设置
	USART1->BRR =84000000/bond;
	
	USART1->CR1 &=~(0X01<<15); //16倍过采样
	USART1->CR1 &=~(0X01<<12);  //字长为8位
	USART1->CR1 |=0x01<<3; //开发送
	USART1->CR1 |=0x01<<2; //开接收	
	USART1->CR2 &=~(0x03<<12); //一个停止位
	//中断使能
	USART1->CR1 |=0x01<<5; //开接收中断
	
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,3,3));  //设置优先级
	NVIC_EnableIRQ(USART1_IRQn); //外部中断使能（系统中断没有这个使能）
	USART1->CR1 |=0x01<<13; //开串口	
}


u8 rec_buf[100];   //接收缓冲区
u8 rec_sta=0;      //接收完成标志rec_sta=1表明接收完成
u8 rec_num=0;     //接收数据量计数
u32 rec_count_time=0;  //超时计数
u8 rec_start=0;    //开始接收字符串标志
void USART1_IRQHandler(void)
{
	
	if(USART1->SR &(0X01<<5))
	{
		rec_start=1;
		rec_count_time=0;
	
		rec_buf[rec_num++]=USART1->DR;
		
	}
	
}





void uart_send_string(u8 *p)
{
	while(*p!='\0')
	{
		//等待上次发完
		while(!(USART1->SR &(0X01<<7)));
		//把收到的数据回发
		USART1->DR =*p++;
	}
	
}




//重写fputc函数
int fputc(int ch, FILE *f) {
	while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
  return (ch);
}


