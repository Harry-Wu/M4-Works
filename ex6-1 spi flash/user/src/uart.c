#include "stm32f4xx.h" 
#include "stdio.h"

void uart1_init(u32 bond)
{
	//端口时钟使能PA
	RCC->AHB1ENR |=0x01;
		//端口设置
	//pa9(txd)-----复用功能推挽输出
	GPIOA->MODER &=~(0X03<<18);   
	GPIOA->MODER |=0X02<<18;     //复用功能模式
	GPIOA->OTYPER &=~(0X01<<9);  //推挽输出
	GPIOA->OSPEEDR&=~(0X03<<18);  //输出速度最大为2M
	GPIOA->PUPDR &=~(0X03<<18);   //无上下拉
	GPIOA->AFR[1] &=0XFFFFFF0F;
	GPIOA->AFR[1] |=0X00000070;   //uart复用模式	
	//pa10(rxd)----复用功能浮空输入
	GPIOA->MODER &=~(0X03<<20);   
	GPIOA->MODER |=0X02<<20;     //复用功能模式
	GPIOA->PUPDR &=~(0X03<<20);   //无上下拉
	GPIOA->AFR[1] &=0XFFFFF0FF;
	GPIOA->AFR[1] |=0X00000700;   //uart复用模式	
	
	//模块时钟使能--UART1
	RCC->APB2ENR |=0X01<<4; 	
	RCC->APB2RSTR |= 0X01<<4;    //复位uart1
	RCC->APB2RSTR &= ~(0X01<<4);  //停止复位
	
	
	//模块初始化设置
	USART1->BRR=84000000/bond;
	USART1->CR1 &=~(0X01<<15);  //OVER8=0   16位采样
	USART1->CR1 &=~(0X01<<12);  //一个起始位，8位数据，N个停止位
	
	USART1->CR2 &=~(0X03<<12);   //一个停止位
	
	
	
	USART1->CR1 |=0X01<<5; //开串口接收中断
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,3,3));  //设置优先级
	NVIC_EnableIRQ(USART1_IRQn); //外部中断使能（系统中断没有这个使能）

	//开启模块
		USART1->CR1 |=0X01<<2;  //开串口接收
		USART1->CR1 |=0X01<<3;  //开串口发送
		USART1->CR1 |=0X01<<13;  //开串口
}




u8 rec_buf[100];   //接收缓冲器
u8 rec_sta=0;       //接收完成与否标志 ，rec_sta=1，接收完成    
u8 rec_num=0;       //接收计数
u8 cont_time=0;
u8 start_rec=0;     //start_rec=1开始到计时

void  USART1_IRQHandler(void)
{

	//判断是否是接收中断
	if((rec_sta==0)&&(USART1->SR &(0X01<<5)))//还没有收完，并且有接收到数据
	{
		cont_time=0;
		start_rec=1;
		rec_buf[rec_num++]=USART1->DR;
	
	}
	else
	{
		USART1->DR;		
	}
	
}






void uart_send_string(u8 *p)
{
	while(*p!='\0')
	{
		//等待上次发送完毕
		while((USART1->SR &(0X01<<7))==0);
		//把前面读到的数据回发给上位机
		USART1->DR=*p++;
	}
	
}


int fputc(int ch, FILE *f) 
{
	while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
  return (ch);
}

