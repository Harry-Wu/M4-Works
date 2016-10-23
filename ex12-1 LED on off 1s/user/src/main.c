#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "string.h"
#include "delay.h"
#include "at24c02.h"
#include "stdio.h"
#include "flash.h"
#include "lcd.h"
#include "adc.h"
#include "DMA.h"
#include "string.h"
#include "time.h"
char *p="hello world\r\n";

int main(void)
{
	u16 adc;
	NVIC_SetPriorityGrouping(7-2);
	led_init( );    //led��ʼ��
	key_init( );
	uart_init(115200);
	at24c02_init();
	SPI_Flash_Init( );

	lcd_init();
	lcd_clear(BACK_COLOR);
	time6_init();
	LED1=0;
	while(1)
	{
		if(TIM6->SR &0x01)//��ʱʱ�䵽
		{
			TIM6->SR &=~(0X01);
			LED2=!LED2;
		}
	}
	
}

