/**************************
这里放显示电子钟,或处理电子钟相关函数
*************************/
#include "stm32f4xx.h"
#include "rtc.h"
#include "lcd.h"
#include "stdio.h"
#include "adc.h"
#include "key.h"
#include "delay.h"


static u8 t;
u8 tbuf[40];
//Week_Alarm_TYPEDEF week_alam;  //变量保存设置的alarm A数据

void display_init(void)
{
	Draw_Circle(120,160,100);
	LCD_DrawLine(0,0,239,319);
	LCD_DrawRectangle(10,10,60,60);
	LCD_DrawSolidRectangle(10, 10, 230, 60, RED);
	Draw_SolidCircle(120,160,50);
	delay_ms(500);
	delay_ms(500);
	LCD_FillCircle_Bresenham(120,160,70);
	delay_ms(500);
	delay_ms(500);
	
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
	LCD_Show_CH(80+16, 160+16, "吴", 0);
	LCD_Show_CH(80+32, 160+16, "许", 0);
	//BACK_COLOR = WHITE;
	
	delay_ms(100);
	LCD_ShowPic(0, 0, (u8 *)gImage_Wallpaper01);
	delay_ms(500);
	LCD_ShowPic(0, 0, (u8 *)gImage_240x320_71023_106);
	LCD_FillCircle_Bresenham(48+60+8,146+24,80);
	
	POINT_COLOR = WHITE;
	LCD_DrawSolidRectangle(10,10,50,50,RED);
	BACK_COLOR = RED;
	LCD_ShowString(22, 22, "关",0);
	BACK_COLOR = BLACK;

	
//	show_ascii_24x48(10, 60, '1', 0);
//	show_ascii_24x48(10+24, 60, '2', 0);
//	show_ascii_24x48(10+24+24, 60, ':', 0);
//	show_ascii_24x48(10+24+24+24, 60, '4', 0);
//	show_ascii_24x48(10+24+24+24+24, 60, '5', 0);
//	show_ascii_24x48(10+24+24+24+24+24, 60, ':', 0);
//	show_ascii_24x48(10+24+24+24+24+24+24, 60, '3', 0);
//	show_ascii_24x48(10+24+24+24+24+24+24+24, 60, '7', 0);
	
	
	
	//下面显示电子钟的固定字符
	sprintf((char*)tbuf,"%02d:%02d",time_date.hour,time_date.min); 
	LCD_ShowString_24x48(48,146,tbuf,0);
	sprintf((char*)tbuf,"%02d",time_date.sec); 
	LCD_ShowString(48+24*5,146+48-24,tbuf,0);
	sprintf((char*)tbuf,"20%02d-%02d-%02d",time_date.year,time_date.month,time_date.date); 
	LCD_ShowString(20,280,tbuf,0);
	sprintf((char*)tbuf,"Week:%d",time_date.week); 
	LCD_ShowString(20,300,tbuf,0);
	LCD_ShowString(165,260, "Alarm A:",0);
	sprintf((char*)tbuf,"%02d:%02d:%02d",week_alam.hour, week_alam.min, week_alam.sec); 
	LCD_ShowString(165,280,tbuf,0);
	sprintf((char*)tbuf,"Week:%d",week_alam.week); 
	LCD_ShowString(180,300,tbuf,0);
}

void display_time(void)
{
	if(t!=time_date.sec)
	{
		t=time_date.sec;
		if(SET_EN_FLAG && setting_mode==1 ) BACK_COLOR = GREEN;//如果是时间修改模式1
		sprintf((char*)tbuf,"%02d",time_date.hour); 
		LCD_ShowString_24x48(48,146,tbuf,0);
		BACK_COLOR = BLACK;
		//show_ascii(20+16,240,':',0);
		if(SET_EN_FLAG && setting_mode==2 ) BACK_COLOR = GREEN;//如果是时间修改模式2
		sprintf((char*)tbuf,"%02d",time_date.min); 
		LCD_ShowString_24x48(48+24*3,146,tbuf,0);
		BACK_COLOR = BLACK;
		//show_ascii(20+24+16*sizeof(time_date.min),240,':',0);
		if(SET_EN_FLAG && setting_mode==3 ) BACK_COLOR = GREEN;//如果是时间修改模式3
		sprintf((char*)tbuf,"%02d",time_date.sec); 
		LCD_ShowString(48+24*3+24*2,146+48-24,tbuf,0);
		BACK_COLOR = BLACK;
		
		if(SET_EN_FLAG && setting_mode==4) BACK_COLOR = GREEN;//如果是时间修改模式4
		sprintf((char*)tbuf,"20%02d",time_date.year); 
		LCD_ShowString(20,280,tbuf,0);
		BACK_COLOR = BLACK;
		if(SET_EN_FLAG && setting_mode==5) BACK_COLOR = GREEN;//如果是时间修改模式5
		sprintf((char*)tbuf,"%02d",time_date.month); 
		LCD_ShowString(20+40,280,tbuf,0);
		BACK_COLOR = BLACK;
		if(SET_EN_FLAG && setting_mode==6) BACK_COLOR = GREEN;//如果是时间修改模式5
		sprintf((char*)tbuf,"%02d",time_date.date); 
		LCD_ShowString(20+40+24,280,tbuf,0);
		BACK_COLOR = BLACK;
		
		if(SET_EN_FLAG && setting_mode==7) BACK_COLOR = GREEN;//如果是时间修改模式7
		sprintf((char*)tbuf,"%d",time_date.week); 
		LCD_ShowString(20+40,300,tbuf,0);
		BACK_COLOR = BLACK;
//		sprintf((char*)tbuf,"SET_EN:%d",SET_EN_FLAG); 
//		LCD_ShowString(20,300,tbuf,0);
//		sprintf((char*)tbuf,"setting_mode:%02d",setting_mode); 
//		LCD_ShowString(120,300,tbuf,0);
		
		//LCD_ShowString(165,240, "Alarm A:",0);
		if(SET_EN_FLAG && setting_mode==8) BACK_COLOR = GREEN;//如果是时间修改模式8
		sprintf((char*)tbuf,"%02d",week_alam.hour); 
		LCD_ShowString(165,280,tbuf,0);
		BACK_COLOR = BLACK;
		if(SET_EN_FLAG && setting_mode==9) BACK_COLOR = GREEN;//如果是时间修改模式9
		sprintf((char*)tbuf,"%02d",week_alam.min); 
		LCD_ShowString(165+24,280,tbuf,0);
		BACK_COLOR = BLACK;
		if(SET_EN_FLAG && setting_mode==10) BACK_COLOR = GREEN;//如果是时间修改模式10
		sprintf((char*)tbuf,"%02d",week_alam.sec); 
		LCD_ShowString(165+24+24,280,tbuf,0);
		BACK_COLOR = BLACK;
		if(SET_EN_FLAG && setting_mode==11) BACK_COLOR = GREEN;//如果是时间修改模式11
		sprintf((char*)tbuf,"%d",week_alam.week); 
		LCD_ShowString(180+40,300,tbuf,0);
		BACK_COLOR = BLACK;
		
		sprintf((char*)tbuf,"亮度:%04d",get_adc()); 
		LCD_ShowString(100,10,tbuf,0);
	} 
		
}
