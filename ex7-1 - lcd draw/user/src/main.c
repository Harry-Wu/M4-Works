/*****************************************************************************
@File name:  
@Description: 操作ILI9341驱动的2.8寸显示屏，在屏幕上画线，圆和矩形 
@Author: Harry Wu
@Version: V1.0
@Date: 
@History: 
*****************************************************************************/

#include "stm32f4xx.h" 
#include "sys.h"
#include "key.h"
//#include "io_bit.h"
#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "at24c02.h"
#include "delay.h"
#include "flash.h"
#include "lcd.h"
#include "MyTypeDef.h"

void delay(u32 i)
{
	while(i--);
}

u8 *p="hello world1234567890\r\n";
u8 buf[100];
		u8 sta;
int main(void)
{
	Stm32_Clock_Init(336, 8, 2, 7);  //系统时钟186MHz

	NVIC_SetPriorityGrouping(7-2);//设置分组
	at24c02_init( );
	//硬件初始化
	led_init( );
	
	key_init();
	uart1_init(115200);
	SPI_Flash_Init();
	lcd_init();
	
	Draw_Circle(120,160,100);
	LCD_DrawLine(0,0,239,319);
	LCD_DrawRectangle(50,50,100,100);
	
	while(1)
	{
		
		printf("0x%x\r\n",SPI_FLASH_TYPE);
		delay_ms(500);
		printf("HELLO\r\n");
		delay_ms(500);
	}
}

