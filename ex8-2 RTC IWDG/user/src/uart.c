#include "stm32f4xx.h" 
#include "stdio.h"
#include "LED.h"
//串口初始化
//bond：波特率
void uart_init(u32 bond)
{


	//端口时钟使能PA
	RCC->AHB1ENR |= 0X01<<0;
//端口模式设置 PA9(txd) PA10(rxd)
	//PA9(txd) PA10(rxd)设为复用功能模式
	GPIOA->MODER &=~(0x03<<18 |0x03<<20);
	GPIOA->MODER |=0x02<<18 |0x02<<20;
	//PA9(txd)设为推挽输出
	GPIOA->OTYPER &=~(0X01<<9);
	//PA9(txd)设为输出速度2M
	GPIOA->OSPEEDR &=~(0X03<<18);
	//PA9(txd) PA10(rxd)设为复用功能映射，映射到串口1
	GPIOA->AFR[1] &=~(0XFF<<4);
	GPIOA->AFR[1] |= 0X77<<4;
	

	
	
//UART1模块时钟使能
	RCC->APB2ENR |=0x01<<4;	
	
//设置波特率
//	brr=(float)84000000/(8*(2-0)*bond);
//	USART1->BRR=brr*16;
USART1->BRR=84000000/bond;



	//数据长度为8位，停止位为1位开接收、开发送、开串口
	USART1->CR1 &=~(0x01<<15);  //OVER8=0 
	USART1->CR1 &=~(0x01<<12);  //字长8位
	
	USART1->CR1 |=0x01<<3;  //开串口1发送
	USART1->CR1 |=0x01<<2;  //开串口1接收
	
	USART1->CR2 &=~(0X03<<12);  //设置停止位为1位
	

	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,3,3));  //设置优先级
	NVIC_EnableIRQ(USART1_IRQn); //外部中断使能（系统中断没有这个使能）
	

	USART1->CR1 |=0x01<<5;  //开串口接收中断
	USART1->CR1 |=0x01<<13;  //开串口1
	
}

u8 rec_buf[100];   //接收缓冲器
u8 rec_sta=0;       //接收完成与否标志 ，rec_sta=1，接收完成    
u8 rec_num=0;       //接收计数


void  USART1_IRQHandler(void)
{
	u8 rec_data;
	//判断是否是接收中断
	if((rec_sta==0)&&(USART1->SR &(0X01<<5)))//还没有收完，并且有接收到数据
	{
		rec_data =USART1->DR;
		if((rec_data=='\n') &&(	rec_buf[rec_num-1]=='\r') )  //接收完毕处理
		{
			rec_buf[rec_num-1]='\0'; //用\0替换\r,把收到的数据转换为一个字符串
			rec_sta=1;   //置接收完成标志
			rec_num=0; 
		}
		else
		{
			rec_buf[rec_num++]=rec_data;
		}
		
	}
	else
	{
		USART1->DR;		
	}
	
}





//发送字符串
void uart_send_string(char *p)
{
	while(*p!='\0')
	{
		while((USART1->SR &(0X01<<7))==0);
		USART1->DR=*p;
		p++;
	}
}







#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) {
	while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
  return (ch);
}
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}




