/*****************************************************************************
@File name:  
@Description: 显示RTC时间,按一个按键开关LED5 
@Author: Harry Wu
@Version: V1.3
@Date: 
@History: 
		V1.0: 开机校准, 添加绘图板程序
		V1.2: 修正合并引起的错误
		V1.3: 将校准数据保存到24c02里, 这样不用每次开机都校准; 加上触摸屏按下扫描处理函数,按一次反转一次LED5


		
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
#include "led.h"


u8 *p="hello world1234567890\r\n";
u8 buf[100];
//u8 sta;
int main(void)
{
	_TOUCH_CSYS_TYPEDEF touch_addr;
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
	//touch_adj();

	
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
	LCD_ShowString(80,160, "hello, how are you?",0);
	
	LCD_Show_CH(80, 160+16, 0, 0);
	LCD_Show_CH(80+16, 160+16, 1, 0);
	LCD_Show_CH(80+32, 160+16, 2, 0);
	BACK_COLOR = WHITE;
	
	delay_ms(100);
	LCD_ShowPic(0, 0, (u8 *)gImage_Betty2014);
	
	LCD_DrawSolidRectangle(50, 50,100,100,RED);
	
	while(1)
	{
		if(t!=time_date.sec)
		{
			t=time_date.sec;
			//RTC_Get_Time(&hour,&min,&sec,&ampm);
			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",time_date.hour,time_date.min,time_date.sec); 
			LCD_ShowString(30,240,tbuf,0);			
			//RTC_Get_Date(&year,&month,&date,&week);
			sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",time_date.year,time_date.month,time_date.date); 
			LCD_ShowString(30,260,tbuf,0);	
			sprintf((char*)tbuf,"Week:%d",time_date.week); 
			LCD_ShowString(30,280,tbuf,0);
		} 
		
		touch_scanf(&touch_addr, 0);
		if(touch_addr.x>50 &&touch_addr.x<100 &&touch_addr.y>50 &&touch_addr.y<100 )
		{
			LED5=!LED5;
			if(LED5)  //如果灯是灭的
			{
				LCD_DrawSolidRectangle(50, 50,100,100,RED);
			}
			else
			{
				LCD_DrawSolidRectangle(50, 50,100,100,GREEN);
			}
		}
		
		
//		CNV_touch2lcd(&touch_add);
//		if(touch_add.x!=0xffff)
//		{
//			LCD_DrawPoint(touch_add.x,touch_add.y);
//			LCD_DrawPoint(touch_add.x+1,touch_add.y);
//			LCD_DrawPoint(touch_add.x,touch_add.y+1);
//			LCD_DrawPoint(touch_add.x+1,touch_add.y+1);
//		}
		

	

	
	
//		get_touch_ad_filled(&touch_addr);
//		printf("x = %4d y = %4d\r\n", touch_addr.x, touch_addr.y);
		//delay_ms(100);

//		printf("0x%x\r\n",SPI_FLASH_TYPE);
//		delay_ms(500);
//		printf("HELLO\r\n");
//		delay_ms(500);
	}
}

