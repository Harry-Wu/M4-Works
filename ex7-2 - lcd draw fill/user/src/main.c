/*****************************************************************************
@File name:  
@Description: ����ILI9341������2.8����ʾ��������Ļ�ϻ�ʵ��Բ��ʵ�ľ��� 
@Author: Harry Wu
@Version: V1.0
@Date: 
@History: ����֮ǰ������Բ�Ϳ��ľ��εĹ���
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
	Stm32_Clock_Init(336, 8, 2, 7);  //ϵͳʱ��186MHz

	NVIC_SetPriorityGrouping(7-2);//���÷���
	at24c02_init( );
	//Ӳ����ʼ��
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
	
	while(1)
	{
		
		printf("0x%x\r\n",SPI_FLASH_TYPE);
		delay_ms(500);
		printf("HELLO\r\n");
		delay_ms(500);
	}
}

