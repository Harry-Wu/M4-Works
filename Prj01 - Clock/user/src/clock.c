/**************************
这里放显示时钟,或处理时钟相关函数
*************************/
#include "stm32f4xx.h"
#include "rtc.h"
#include "lcd.h"
#include "stdio.h"
#include "adc.h"
#include "key.h"


static u8 t;
u8 tbuf[40];
Week_Alarm_TYPEDEF week_alam;  //变量保存设置的alarm A数据
	
void display_tim(void)
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
		
		sprintf((char*)tbuf,"Bright:%04d",get_adc()); 
		LCD_ShowString(100,10,tbuf,0);
	} 
		
}
