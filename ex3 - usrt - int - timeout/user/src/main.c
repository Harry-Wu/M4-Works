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
		
		//在main函数中判断并置位接收完成标志位
		if(rec_start)  //开始接收数据
		{
			rec_count_time++;
			delay_ms(10);
			if(rec_count_time>100)  //超时
			{
				rec_sta = 1;
				rec_buf[rec_num] = '\0';
				rec_num = 0;
				rec_start = 0;
			}
		}
								
	}	
}


