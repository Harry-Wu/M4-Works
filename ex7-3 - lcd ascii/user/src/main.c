/*****************************************************************************
@File name:  
@Description: 操作ILI9341驱动的2.8寸显示屏，在屏幕上画实心圆和实心矩形 
@Author: Harry Wu
@Version: V1.0
@Date: 
@History: 基于之前画空心圆和空心矩形的工程
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
	LCD_DrawRectangle(10,10,60,60);
	LCD_DrawSolidRectangle(180, 10, 230, 60, RED);
	Draw_SolidCircle(120,160,50);
	
	show_ascii(100, 10, 's');
	delay_ms(500);
	show_ascii(108, 10, 'o');
	delay_ms(500);
	show_ascii(116, 10, 'l');
	delay_ms(500);
	show_ascii(124, 10, 'v');
	delay_ms(500);
	show_ascii(132, 10, 'e');
	
	show_ascii(100, 30, 'L');
	delay_ms(500);
	show_ascii(108, 30, 'O');
	delay_ms(500);
	show_ascii(116, 30, 'V');
	delay_ms(500);
	show_ascii(124, 30, 'E');
	delay_ms(500);
	show_ascii(132, 30, '!');
	
	while(1)
	{
		
		printf("0x%x\r\n",SPI_FLASH_TYPE);
		delay_ms(500);
		printf("HELLO\r\n");
		delay_ms(500);
	}
}

