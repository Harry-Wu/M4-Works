#include "stm32f4xx.h"
#include "stdio.h"

void uart1_init(u32 bond)
{
	//端口时钟使能
	RCC->AHB1ENR |= 0x01<<0;
	//PA9(TXD)--复用推挽功能
	//PA10(RXD)--复用浮空功能
	GPIOA->MODER &= ~(0x03<<18 | 0x03<<20);
	GPIOA->MODER |= (0x02<<18 | 0x02<<20); //PA9,10设为复用功能
	GPIOA->OTYPER &= ~(0x01<<9);  //PA9推挽输出
	GPIOA->OSPEEDR &= ~(0x03<<18);  //pa9输出速度2M
	GPIOA->PUPDR &= ~(0x03<<18 | 0x03<<20); //pa9,10不用上下拉
	
	GPIOA->AFR[1] &= 0xfffff00f;
	GPIOA->AFR[1] |= 0x00000770; // //pa9 10 复用功能设为uart1的txd rxd, AF7--0b0111
	
	
	//uart1时钟使能
	RCC->APB2ENR |=0X01<<4;
	//波特率设置
	USART1->BRR =84000000/bond;  //AFB2是84M
	
	USART1->CR1 &=~(0X01<<15); //16倍过采样
	USART1->CR1 &=~(0X01<<12);  //字长为8位
	USART1->CR1 |=0x01<<3; //开发送
	USART1->CR1 |=0x01<<2; //开接收	
	USART1->CR2 &=~(0x03<<12); //一个停止位
	//中断使能
	USART1->CR1 |= 0x01<<5;
	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(7-2,3,3));  //设置优先级
	NVIC_EnableIRQ(USART1_IRQn);  //外部中断使能(系统中断没有这个使能)
	
	USART1->CR1 |= 0x01<<13;  //开串口
	
}

u8 rec_buf[100];  //接收缓冲区, 如果不声明申请的空间,就必须赋值
u8 rec_sta=0; //接收完成标志, rec_sta=1表示接收完成
u8 rec_num=0;  //接收字节数计数
u32 rec_count_time=0;  //超时计数
u8 rec_start=0; //开始接收字符串标志

void USART1_IRQHandler(void)
{
	//u8 rec_data;
	
	if(USART1->SR & (0x01<<5))  //判断是否收到数据
	{	
		rec_start = 1; //开始接收数据了
		rec_count_time=0; //超时清零
	
		rec_buf[rec_num++] = USART1->DR;
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
int fputc(int ch, FILE *f) 
{
	while((USART1->SR &(0X01<<7))==0);
	USART1->DR=ch;
	return (ch);
}

