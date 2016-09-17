/*****************************************************************************
@File name:  
@Description: 开机进行校准, 校准后执行其他程序, 添加绘画板程序验证校准是否OK 
@Author: Harry Wu
@Version: V1.0
@Date: 
@History: 
		V1.0:画板还无法正常使用, while(1)中其他程序导致了延时
		
*****************************************************************************/

#include "stm32f4xx.h" 
//#include "sys.h"
#include "key.h"
#include "io_bit.h"
#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "at24c02.h"
#include "delay.h"
#include "flash.h"
#include "lcd.h"
#include "rtc.h"
#include "touch.h"


u8 *p="hello world1234567890\r\n";
u8 buf[100];
//u8 sta;
int main(void)
{
	_TOUCH_CSYS_TYPEDEF touch_add;
	//Stm32_Clock_Init(336, 8, 2, 7);  //系统时钟186MHz
	
	u8 t;
	u8 tbuf[40];
	
	NVIC_SetPriorityGrouping(7-2);//设置分组
	at24c02_init( );
	//硬件初始化
	led_init( );
	
	key_init();
	uart1_init(115200);
	SPI_Flash_Init();
	lcd_init();
	
	RTC_Init();
	RTC_Set_WakeUp(4,0);
	
	touch_init();
	touch_adj();
	
	Draw_Circle(120,160,100);
	LCD_DrawLine(0,0,239,319);
	LCD_DrawRectangle(10,10,60,60);
	LCD_DrawSolidRectangle(10, 10, 230, 60, RED);
	Draw_SolidCircle(120,160,50);
	
	show_ascii(100, 10, 's', 1);
	delay_ms(100);
	show_ascii(108, 10, 'o', 1);
	delay_ms(100);
	show_ascii(116, 10, 'l', 1);
	delay_ms(100);
	show_ascii(124, 10, 'v', 1);
	delay_ms(100);
	show_ascii(132, 10, 'e', 1);
	
	show_ascii(100, 30, 'L', 0);
	delay_ms(100);
	show_ascii(108, 30, 'O', 0);
	delay_ms(100);
	show_ascii(116, 30, 'V', 0);
	delay_ms(100);
	show_ascii(124, 30, 'E', 0);
	delay_ms(100);
	show_ascii(132, 30, '!', 0);
	
	delay_ms(100);
	BACK_COLOR = GREEN;
	LCD_ShowString(80,160,"hello, how are you?",0);
	
	LCD_Show_CH(80, 160+16, 0, 0);
	LCD_Show_CH(80+16, 160+16, 1, 0);
	LCD_Show_CH(80+32, 160+16, 2, 0);
	
	delay_ms(100);
	LCD_ShowPic(0, 0, (u8 *)gImage_Betty2014);
	
	while(1)
	{
		if(t!=time_date.sec)
		{
			t=time_date.sec;
			//RTC_Get_Time(&hour,&min,&sec,&ampm);
			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",time_date.hour,time_date.min,time_date.sec); 
			LCD_ShowString(30,140,tbuf,0);			
			//RTC_Get_Date(&year,&month,&date,&week);
			sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",time_date.year,time_date.month,time_date.date); 
			LCD_ShowString(30,160,tbuf,0);	
			sprintf((char*)tbuf,"Week:%d",time_date.week); 
			LCD_ShowString(30,180,tbuf,0);
		} 
		
		CNV_touch2lcd(&touch_add);
		if(touch_add.x!=0xffff)
		{
			LCD_DrawPoint(touch_add.x,touch_add.y);
			LCD_DrawPoint(touch_add.x+1,touch_add.y);
			LCD_DrawPoint(touch_add.x,touch_add.y+1);
			LCD_DrawPoint(touch_add.x+1,touch_add.y+1);
		}
		
		get_touch_ad_filled(&touch_add);
		printf("x = %4d y = %4d\r\n", touch_add.x, touch_add.y);
		//delay_ms(500);
//		printf("0x%x\r\n",SPI_FLASH_TYPE);
//		delay_ms(500);
//		printf("HELLO\r\n");
//		delay_ms(500);
	}
}

