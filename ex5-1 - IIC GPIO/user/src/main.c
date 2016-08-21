#include "stm32f4xx.h" 
#include "key.h" 
#include "led.h"
#include "io_bit.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"
#include "exti.h"
#include "at24c02.h"


//u8 buf[100];
int main(void)
{
	u8 led_sta;
	u8 sta;
	//硬件初始化
	NVIC_SetPriorityGrouping(7-2);//设置分组
	led_init();
	key_init();
	uart1_init(9600);
	exti_init();
	exti2_init();
	exti9_soft_init();
	
	sta = at24c02_read_byte(100,&led_sta);
	while(sta != ACK_OK)
	{
		printf("at24c02 read error:%d/n", sta);
		delay_ms(500);
	}
	if((led_sta != '0') && (led_sta != '1'))  //第一次操作
	{
		D5 = 1;
		sta = at24c02_write_byte(100, led_sta);
		while(sta != ACK_OK)
		{
			printf("at24c02 read error:%d/n", sta);
			delay_ms(500);
		}
	}
	else if(led_sta == '0')
	{
		D5 = 0;
	}
	else if(led_sta == '1')
	{
		D5 = 1;
	}
	
	
	while(1)
	{
		delay_ms(100);
		EXTI->SWIER |= 0x01<<9; //软件中断事件寄存器置1
	}
	
}


