/**************************
�������ʾʱ��,����ʱ����غ���
*************************/
#include "stm32f4xx.h"
#include "rtc.h"
#include "lcd.h"
#include "stdio.h"
#include "adc.h"
#include "key.h"


static u8 t;
u8 tbuf[40];
Week_Alarm_TYPEDEF week_alam;  //�����������õ�alarm A����
	
void display_tim(void)
{
	if(t!=time_date.sec)
	{
		t=time_date.sec;
		if(KEY2_MODE && setting_mode<=3 ) BACK_COLOR = GREEN;//�����ʱ���޸�ģʽ1-3
		sprintf((char*)tbuf,"%02d:%02d:%02d",time_date.hour,time_date.min,time_date.sec); 
		LCD_ShowString(20,240,tbuf,0);
		BACK_COLOR = WHITE;
		
		if(KEY2_MODE && setting_mode>3 && setting_mode<=6) BACK_COLOR = GREEN;//�����ʱ���޸�ģʽ4-6
		sprintf((char*)tbuf,"20%02d-%02d-%02d",time_date.year,time_date.month,time_date.date); 
		LCD_ShowString(20,260,tbuf,0);
		BACK_COLOR = WHITE;
		
		if(KEY2_MODE && setting_mode==7) BACK_COLOR = GREEN;//�����ʱ���޸�ģʽ7
		sprintf((char*)tbuf,"Week:%d",time_date.week); 
		LCD_ShowString(20,280,tbuf,0);
		BACK_COLOR = WHITE;
		sprintf((char*)tbuf,"KEY2_MODE:%d",KEY2_MODE); 
		LCD_ShowString(20,300,tbuf,0);
		sprintf((char*)tbuf,"setting_mode:%02d",setting_mode); 
		LCD_ShowString(120,300,tbuf,0);
		BACK_COLOR = WHITE;
		
		LCD_ShowString(165,240, "Alarm A:",0);
		if(KEY2_MODE && setting_mode>7 && setting_mode<=10) BACK_COLOR = GREEN;//�����ʱ���޸�ģʽ8-10
		sprintf((char*)tbuf,"%02d:%02d:%02d",week_alam.hour, week_alam.min, week_alam.sec); 
		LCD_ShowString(165,260,tbuf,0);
		BACK_COLOR = WHITE;
		if(KEY2_MODE && setting_mode==11) BACK_COLOR = GREEN;//�����ʱ���޸�ģʽ11
		sprintf((char*)tbuf,"Week:%d",week_alam.week); 
		LCD_ShowString(180,280,tbuf,0);
		BACK_COLOR = WHITE;
		
		sprintf((char*)tbuf,"Bright:%04d",get_adc()); 
		LCD_ShowString(100,10,tbuf,0);
	} 
		
}
