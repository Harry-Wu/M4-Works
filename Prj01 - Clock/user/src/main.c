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
	Week_Alarm_TYPEDEF week_alam;  //变量保存设置的alarm A数据
	week_alam.week = 4;
	week_alam.hour = 22;
	week_alam.min = 35;
	week_alam.sec = 0;
	
	//Stm32_Clock_Init(336, 8, 2, 7);  //系统时钟186MHz
	
	u8 t,key_value;
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
	RTC_Set_AlarmA(week_alam.week, week_alam.hour, week_alam.min, week_alam.sec);
	RTC_Set_WakeUp(4,0);
	
	touch_init();

	
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
	LCD_ShowString(20,160, "hello, how are you?",0);
	
	LCD_Show_CH(80, 160+16, 0, 0);
	LCD_Show_CH(80+16, 160+16, 1, 0);
	LCD_Show_CH(80+32, 160+16, 2, 0);
	BACK_COLOR = WHITE;
	
	delay_ms(100);
	LCD_ShowPic(0, 0, (u8 *)gImage_Wallpaper02);
	
	LCD_DrawSolidRectangle(10,10,50,50,RED);
	
	while(1)
	{
		if(t!=time_date.sec)
		{
			t=time_date.sec;
			//RTC_Get_Time(&hour,&min,&sec,&ampm);
			sprintf((char*)tbuf,"%02d:%02d:%02d",time_date.hour,time_date.min,time_date.sec); 
			LCD_ShowString(20,240,tbuf,0);			
			//RTC_Get_Date(&year,&month,&date,&week);
			sprintf((char*)tbuf,"20%02d-%02d-%02d",time_date.year,time_date.month,time_date.date); 
			LCD_ShowString(20,260,tbuf,0);	
			sprintf((char*)tbuf,"Week:%d",time_date.week); 
			LCD_ShowString(20,280,tbuf,0);
			sprintf((char*)tbuf,"SET_MODE:%02d",KEY2_MODE); 
			LCD_ShowString(70,300,tbuf,0);
			
			LCD_ShowString(165,240, "Alarm A:",0);
			sprintf((char*)tbuf,"%02d:%02d:%02d",week_alam.hour, week_alam.min, week_alam.sec); 
			LCD_ShowString(165,260,tbuf,0);
			sprintf((char*)tbuf,"Week:%d",week_alam.week); 
			LCD_ShowString(180,280,tbuf,0);
		} 
		
		touch_scanf(&touch_addr, 0);
		if(touch_addr.x>10 &&touch_addr.x<50 &&touch_addr.y>10 &&touch_addr.y<50 )
		{
			LED5=!LED5;
			if(LED5)  //如果灯是灭的
			{
				LCD_DrawSolidRectangle(10, 10,50,50,RED);
			}
			else
			{
				LCD_DrawSolidRectangle(10, 10,50,50,GREEN);
			}
		}
		
		
		key_value = key_scanf(0);
		if(KEY2_MODE != 0)
		{
			if(key_value !=NO_KEY)  //如果有按键按下
			{
				RTC_Get_Time(&time_date);
				RTC_Get_Date(&time_date);
			}
			switch(KEY2_MODE)
			{
				//设置时间,日期模式
				case 1 : 
					if(key_value==KEY1_OK)
					{
						RTC_Set_Time(time_date.hour+1, time_date.min, time_date.sec, time_date.ampm);
					}
					else if(key_value==KEY3_OK)
					{
						RTC_Set_Time(time_date.hour-1, time_date.min, time_date.sec, time_date.ampm);
					}
					break;
				case 2 :
					if(key_value==KEY1_OK)
					{
						RTC_Set_Time(time_date.hour, time_date.min+1, time_date.sec, time_date.ampm);
					}
					else if(key_value==KEY3_OK)
					{
						RTC_Set_Time(time_date.hour, time_date.min-1, time_date.sec, time_date.ampm);
					}
					break;
				case 3 :
					if(key_value==KEY1_OK)
					{
						RTC_Set_Time(time_date.hour, time_date.min, time_date.sec+1, time_date.ampm);
					}
					else if(key_value==KEY3_OK)
					{
						RTC_Set_Time(time_date.hour, time_date.min, time_date.sec-1, time_date.ampm);
					}
					break;
				case 4:
					if(key_value==KEY1_OK)
					{
						RTC_Set_Date(time_date.year+1, time_date.month, time_date.date, time_date.week);
					}
					else if(key_value==KEY3_OK)
					{
						RTC_Set_Date(time_date.year-1, time_date.month, time_date.date, time_date.week);
					}	
					break;
				case 5:
					if(key_value==KEY1_OK)
					{
						RTC_Set_Date(time_date.year, time_date.month+1, time_date.date, time_date.week);
					}
					else if(key_value==KEY3_OK)
					{
						RTC_Set_Date(time_date.year, time_date.month+1, time_date.date, time_date.week);
					}	
					break;
				case 6:
					if(key_value==KEY1_OK)
					{
						RTC_Set_Date(time_date.year, time_date.month, time_date.date+1, time_date.week);
					}
					else if(key_value==KEY3_OK)
					{
						RTC_Set_Date(time_date.year, time_date.month, time_date.date-1, time_date.week);
					}	
					break;
				case 7:
					if(key_value==KEY1_OK)
					{
						RTC_Set_Date(time_date.year, time_date.month, time_date.date, time_date.week+1);
					}
					else if(key_value==KEY3_OK)
					{
						RTC_Set_Date(time_date.year, time_date.month, time_date.date, time_date.week-1);
					}	
					break;
					
				//设置闹钟模式	
				case 8:
					if(key_value==KEY1_OK)
					{
						week_alam.hour++;
						RTC_Set_AlarmA(week_alam.week, week_alam.hour, week_alam.min, week_alam.sec);
					}
					else if(key_value==KEY3_OK)
					{
						week_alam.hour--;
						RTC_Set_AlarmA(week_alam.week, week_alam.hour, week_alam.min, week_alam.sec);
					}	
					break;
				case 9:
					if(key_value==KEY1_OK)
					{
						week_alam.min++;
						RTC_Set_AlarmA(week_alam.week, week_alam.hour, week_alam.min, week_alam.sec);
					}
					else if(key_value==KEY3_OK)
					{
						week_alam.min--;
						RTC_Set_AlarmA(week_alam.week, week_alam.hour, week_alam.min, week_alam.sec);
					}	
					break;
				case 10:
					if(key_value==KEY1_OK)
					{
						week_alam.sec++;
						RTC_Set_AlarmA(week_alam.week, week_alam.hour, week_alam.min, week_alam.sec);
					}
					else if(key_value==KEY3_OK)
					{
						week_alam.sec--;
						RTC_Set_AlarmA(week_alam.week, week_alam.hour, week_alam.min, week_alam.sec);
					}	
					break;
				case 11:
					if(key_value==KEY1_OK)
					{
						week_alam.week++;
						RTC_Set_AlarmA(week_alam.week, week_alam.hour, week_alam.min, week_alam.sec);
					}
					else if(key_value==KEY3_OK)
					{
						week_alam.week--;
						RTC_Set_AlarmA(week_alam.week, week_alam.hour, week_alam.min, week_alam.sec);
					}	
					break;
			}
		}
		else if(key_value != NO_KEY)  //如果有按键按下
		{
			switch(key_value)
			{
				case KEY1_OK:
					LED5 = !LED5;
					break;
				case KEY3_OK:
					LED6 = !LED6;
					break;
				case KEY4_OK:
					BEEP = !BEEP;					
					break;
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

