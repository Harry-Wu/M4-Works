#include "stm32f4xx.h"
#include "stdio.h"

void uart_init(u32 bond)
{
//	1.	端口时钟使能PA
	RCC->AHB1ENR |=0X01<<0;	
//2.	将对应端口设置为复用功能模式、设置复用功能寄存器
	GPIOA->MODER &=~(0X03<<18 | 0X03 <<20);
	GPIOA->MODER |=0X02<<18 |0X02<<20;//pa9 10设置为复用功能
	GPIOA->OTYPER &=~(0X01<<9);   //pa9(txd) 推挽输出
	GPIOA->OSPEEDR &=~(0X03<<18);  //pa9输出最大速度2M
	GPIOA->PUPDR &=~(0X03<<18 | 0X03 <<20);  //pa9 10 不使用上下拉
	
	GPIOA->AFR[1] &=0XFFFFF00F;
	GPIOA->AFR[1] |=0X00000770;	
	
	//3.	串口时钟使能uart1
	RCC->APB2ENR |=0X01<<4;
//4.	设置波特率、设置数据长度、停止位、是否用奇偶校验
	USART1->BRR =84000000/bond;     //有一个前提（OVER8=0）
	
	USART1->CR1 &=~(0X01<<15);    //over8=0; 16倍过采样
	USART1->CR1 &=~(0X01<<12);    //数据长度为8位
	
	
	//5.	开接收、开发送、如果有需求的话开接收中断
	USART1->CR1|=0X01<<3;   //开发送
	USART1->CR1|=0X01<<2;    //开接收
	
	//开串口接收中断
		USART1->CR1|=0X01<<5;    //开接收完成中断
	//设置串口中断的优先级
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,1,1));
	//在NVIC中使能串口中断
	NVIC_EnableIRQ(USART1_IRQn);
//6.	开串口
	USART1->CR1|=0X01<<13;
}

u8 rec_buf[1000];
static u16 rec_num=0;    //接收数据计数
u8 rec_sta=0;     //接收完毕标志 rec_sta=1，接收完毕

void USART1_IRQHandler(void)
{
	u8 _data;
	if(USART1->SR &(0X01<<5))  //如果是接收中断
	{
		_data=USART1->DR ;
		if((_data=='\n') && (rec_buf[rec_num-1]=='\r'))  //本次收到的是\N上次收到的是\r
		{
			rec_buf[rec_num-1]='\0';//将接收到的数据转换成字符串
			rec_num=0;    //方便下次接收
			rec_sta=1;    //置接收完成标志
			
		}
		else
		{
			rec_buf[rec_num++]=_data;
		}
	}
}













void uart_send_string(s8 *p)
{
	while(*p!='\0')
	{
			while(!(USART1->SR &(0X01<<7)));  //当发送缓冲器不为空则等待
			USART1->DR=*p++;   
		
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


