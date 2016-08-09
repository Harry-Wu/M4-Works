#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"
#include "io_bit.h"
#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"


//#define LS1 PFout(8)



int main(void)
{
	u8 buf[100];
	
	//硬件初始化
	NVIC_SetPriorityGrouping(7-2);//设置分组 
	led_init();
	key_init();
	uart1_init(115200);
	
	while(1)
	{
//		//等待接收到数据
//		while(!(USART1->SR & (0x01<<5)));
//		//读出数据
//		rec_data = USART1->DR;
//		//等待上次发完
//		while(!(USART1->SR & (0x01<<7)));
//		//把收到的数据回发
//		USART1->DR = rec_data;
		
		if(rec_sta==1)
		{
			if(0==strcmp((const char *)rec_buf, "open"))
			{
				D5 = 0;
				
			}
			else if(0==strcmp((const char *)rec_buf, "close"))
			{
				D5 = 1;
			}
			rec_sta = 0;
		}
								
	}	
}


