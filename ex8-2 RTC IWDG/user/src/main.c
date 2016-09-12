/*****************************************************************************
@File name:  
@Description: 通过初始化并设置看门狗溢出时间（默认1s），测试在时间内喂狗或超时喂狗会不会导致重启
@Author: Harry Wu
@Version: V1.0
@Date: 
@History: 
*****************************************************************************/
#include "stm32f4xx.h" 
#include "led.h"
#include "key.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "exti.h"
#include "delay.h"
#include "iic.h"
#include "at24c02.h"
#include "flash.h"
#include "lcd.h"
#include "rtc.h"
#include "wdg.h"

u8 *p="hello world123456789";




int main(void)
{
//	u8 t;

//	u8 tbuf[40];

	NVIC_SetPriorityGrouping(7-2);//设置分组
	led_init();
	key_init();
	uart_init(9600);
	at24c02_init( );
	SPI_Flash_Init();
	lcd_init( );

	
	RTC_Init();		 			//初始化RTC
	RTC_Set_WakeUp(4,0);
	
	LED1=0;
	delay_ms(500);
	LED1=1;
	iwdg_init(7,125);  //1/(32K/256)*125, 1s

	while(1)
	{
		delay_ms(500);
		delay_ms(400);
		//delay_ms(500);
		//delay_ms(500);
		//while(1);
		IWDG->KR=0XAAAA;  //喂狗防止重启
		
//		if(t!=time_date.sec)
//		{
//			t=time_date.sec;
//			//RTC_Get_Time(&hour,&min,&sec,&ampm);
//			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",time_date.hour,time_date.min,time_date.sec); 
//			show_ascii_string(30,140,tbuf,0);			
//			//RTC_Get_Date(&year,&month,&date,&week);
//			sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",time_date.year,time_date.month,time_date.date); 
//			show_ascii_string(30,160,tbuf,0);	
//			sprintf((char*)tbuf,"Week:%d",time_date.week); 
//			show_ascii_string(30,180,tbuf,0);
//			
//			IWDG->KR = 0XAAAA;
//		} 
		
	
	}	
		
}


