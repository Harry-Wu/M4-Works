#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"
#include "io_bit.h"
#include "led.h"
#include "uart.h"

//#define LS1 PFout(8)



int main(void)
{
	u8 rec_data;
	
	//硬件初始化
	led_init();
	key_init();
	uart1_init(115200);
	
	while(1)
	{
		//等待接收到数据
		while(!(USART1->SR & (0x01<<5)));
		//读出数据
		rec_data = USART1->DR;
		//等待上次发完
		while(!(USART1->SR & (0x01<<7)));
		//把收到的数据回发
		USART1->DR = rec_data;
								
	}	
}


