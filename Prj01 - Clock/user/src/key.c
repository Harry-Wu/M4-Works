/*****************************************************************************
@File name:  
@Description: 
	长按KEY2进入或退出修改时间模式(KEY2_MODE==1), 进入修改模式时,相应的时间字段背景色变为绿色;
	时间修改模式下,短按KEY1增加数值, 短按KEY3减小数值;
	非修改模式下,KEY1开关D5, KEY3开关D6, KEY4开关闹钟BEEP
@Author: Harry Wu
@Version: V1.3
@Date: 
@History: 

		
*****************************************************************************/
#include "stm32f4xx.h" 
#include "key.h"
#include "delay.h"
#include "clock.h"
#include "led.h"

u8 KEY2_MODE=0;  //初始状态下, 不允许修改时间
u8 setting_mode=1;  //时间设置模式
u16 lngcounter=0; //按键按下时间测量变量  全局变量
u8 key_value, key2_value;  //保存key_scanf和key_scanf_longshort的返回值

void key_init(void)
{
	//端口时钟使能PA
	RCC->AHB1ENR |=0X01<<0;
	//端口配置PA0配置为普通功能浮空输入
	GPIOA->MODER &=~(0X03<<0); //普通功能输入模式
	GPIOA->PUPDR &=~(0X03<<0); //无上下拉
	
	
		//端口时钟使能PE
	RCC->AHB1ENR |=0X01<<4;
	//端口配置PE 2 3 4配置为普通功能浮空输入
	GPIOE->MODER &=~(0X03<<4 | 0X03<<6 |0X03<<8); //普通功能输入模式
	GPIOE->PUPDR &=~(0X03<<4 | 0X03<<6 |0X03<<8);  //无上下拉
	
	
}




//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0xFF，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY4按下
//注意此函数有响应优先级,KEY1>KEY2>KEY3>KEY4!!
u8 key_scanf(u8 mode)
{
	static u8  key_sta=1;    //key_sta=1,允许识别按键
	static u8 key_time=0;

	static u8 key1,key2,key3;
	u8 key=NO_KEY;
	
	if(mode)
	{
		key_sta=1;  //按键松开标志, 支持连按
	}
	
	key3=key2;
	key2=key1;	
	key_time++;
	
	if((key_sta==1)&& ((KEY1==1) || (KEY3==0) || (KEY4==0)))  //允许识别按键，并且有可能是按键按下了
	{
		//key_delay(); //延时消抖
		if(KEY1)   //再次判断是否为高电平
		{
			key1=KEY1_OK;   //识别高按键处理
		}		
		
		else if(!KEY3)  
		{
			key1=KEY3_OK;   
		}	
		else if(!KEY4)
		{
			key1=KEY4_OK;  
		}	
		
		
		if(key_time==3)   //多次识别消抖
		{
			key_time=0;
			
			if((key3==key1) && (key2==key1))
			{
				key=key1;
				
				key_sta=0;     //不允许识别按键
			}
			
		}
	}
	else if((0==KEY1) &&(KEY3==1) &&(KEY4==1) )   //没有按键按下
	{
		key_sta=1;     //允许识别按键
		key_time=0;
	}
	return key;
}

/***********************
** 函数名称: keyscanf_longshort()
** 功能描述: 检测到一次按键按下并且提起
** 输　 入: 按键信号
** 输 出: 0:无按键 1:一次单击 2：一次长按
** 全局变量: lngcounter
** 调用模块: 
** 说明： lngcounter是在中断时间为一毫秒的定时器里自加一的
** 注意：
********************/
u8 keyscanf_longshort(void)
{
	if(KEY2==0)
	{
		delay_ms(10);  //延时消抖
		if(KEY2==0)
		{
			TIM13->CR1 |=  0X01<<0;  //使能定时器
			while(KEY2==0) display_tim();
			TIM13->CR1 &=  ~(0x01<<0); //禁能定时器
			if(lngcounter>2000)  //如果按键时间超过2s,则为长按, 返回值2
			{
				lngcounter = 0;
				KEY2_MODE = !KEY2_MODE;  //长按在允许和禁止修改时间之间切换
				return 2;
			}
			else  //短按
			{
				lngcounter = 0;
				if(KEY2_MODE) //如果短按前是修改时间模式
				{
					setting_mode++;
					if(setting_mode==12) setting_mode=1;
				}
			}
			return 1;
		}
		else
		return 0;  //延时消抖后没有检测到按键
	}	
	else
	return 0;
}

void key_service(void)
{
	//key2_value = keyscanf_longshort();
	key_value = key_scanf(0);
	if(KEY2_MODE != 0)  //如果处于时间修改模式
	{
		if(key_value !=NO_KEY)  //如果有按键按下
		{
			RTC_Get_Time(&time_date);
			RTC_Get_Date(&time_date);
		}
		switch(setting_mode)
		{
		//设置时间,日期模式
		case 1 : 
			if(key_value==KEY1_OK)
			{
				time_date.hour++;
				if(time_date.hour==24) time_date.hour = 0;
				RTC_Set_Time(time_date.hour, time_date.min, time_date.sec, time_date.ampm);
			}
			else if(key_value==KEY3_OK)
			{
				if(time_date.hour==0) time_date.hour = 23;
				else time_date.hour--;
				
				RTC_Set_Time(time_date.hour, time_date.min, time_date.sec, time_date.ampm);
			}
			break;
		case 2 :
			if(key_value==KEY1_OK)
			{
				time_date.min++;
				if(time_date.min==60) time_date.min = 0;
				RTC_Set_Time(time_date.hour, time_date.min, time_date.sec, time_date.ampm);
			}
			else if(key_value==KEY3_OK)
			{
				if(time_date.min==0) time_date.min = 59;
				else time_date.min--;
				RTC_Set_Time(time_date.hour, time_date.min, time_date.sec, time_date.ampm);
			}
			break;
		case 3 :
			if(key_value==KEY1_OK)
			{
				time_date.sec++;
				if(time_date.sec==60) time_date.sec = 0;
				RTC_Set_Time(time_date.hour, time_date.min, time_date.sec, time_date.ampm);
			}
			else if(key_value==KEY3_OK)
			{
				if(time_date.sec==0) time_date.sec = 59;
				else time_date.sec--;
				RTC_Set_Time(time_date.hour, time_date.min, time_date.sec, time_date.ampm);
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
				time_date.week++;
				if(time_date.week==8) time_date.week = 1;
				RTC_Set_Date(time_date.year, time_date.month, time_date.date, time_date.week);
			}
			else if(key_value==KEY3_OK)
			{
				if(time_date.week==1) time_date.week = 7;
				else time_date.week--;
				RTC_Set_Date(time_date.year, time_date.month, time_date.date, time_date.week);
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
	else if(key_value != NO_KEY)  //如果处于非时间修改模式, 并且有按键按下
	{
		// switch (key2_value)
		// {
		//     case 1:

		// 		break;
		// }

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

}
