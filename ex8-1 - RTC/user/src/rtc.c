#include "rtc.h"
#include "delay.h"

TIME_DATE_TYPEDEF time_date;  //定义一个自定义类型的变量


//RTC进入初始化模式
//返回值:0,成功;1,失败;
u8 RTC_Init_Mode(void)
{ 
	u32 retry=0X10000; 
	if(RTC->ISR&(1<<6))return 0; 
	RTC->ISR|=1<<7;	//进入RTC初始化模式
	while(retry&&((RTC->ISR&(1<<6))==0x00))//等待进入RTC初始化模式成功
	{
		retry--;
	}
    if(retry==0)return 1;	//同步失败
	else return 0; 			//同步成功 
}

/***************************************************
//RTC时间设置
//hour,min,sec:小时,分钟,秒钟
//ampm:AM/PM,0=AM/24H,1=PM.
//返回值:0,成功
//       1,进入初始化模式失败 
***************************************************/
void RTC_Set_Time(u8 hour, u8 min, u8 sec, u8 ampm);
{
	
}

/***************************************************
//RTC日期设置
//year,month,date:年(0~99),月(1~12),日(0~31)
//week:星期(1~7,0,非法!)
//返回值:0,成功
//       1,进入初始化模式失败 
***************************************************/
void RTC_Set_Date(u8 year, u8 month, u8 date, u8 week);
{
	
}

/***********************************
//初始化RTC
//返回值： 0--初始化成功
		   1--LSE开启失败
		   2--进入初始化模式失败
***********************************/
u8 RTC_Init(void)
{
	u16 retry = 0x1FFF;
	RCC->APB1ENR |= 1<<28;  //使能电源接口时钟
	PWR->CR |= 1<<8;  //后备区域访问使能(RTC+SRAM)
	
	if(RTC->BKP0R != 0X5151)  //是否第一次配置， 0x5151是自己设置的意义，代表是否第一次配置的标志
	{
		RCC->BDCR |= 1<<0;  //LSE开启
		while(retry&&((RCC->BDCR&0x02)==0))  //等待LSE准备好
		{
			retry--;
			delay_ms(5);
		}
		if(retry==0) return 1;  //LSE开启失败
		
		RCC->BDCR |= 1<<8;  //选择LSE作为RTC的时钟
		RCC->BDCR |= 1<<15;  //使能RTC时钟
		
		//关闭RTC寄存器写保护
		RTC->WPR=0xCA;
		RTC->WPR=0x53;
		if(RTC_Init_Mode()) return 2;  //进入RTC初始化模式失败
		RTC->PRER = 0XFF;  //RTC同步分频器系数(0~7FFF), [14:0]
		RTC->PRER = 0X7F<<16;  ////RTC异步分频器系数(1~0x7F), [14:0]
		RTC->CR &= ~(1<<6);  //rtc设置为24小时格式
		RTC->ISR &= ~(1<<7);  //退出rtc初始化模式
		RTC->WPR = 0XFF;  //写入任意值可使能rtc寄存器写保护
		
		RTC_Set_Time(22,50,0,0);  //设置时间
		RTC_Set_Date(16,9,13,2);  //设置日期
		RTC_Set_AlarmA(7,0,0,10);  //设置闹钟时间
		RTC->BKP0R = 0x5151;  //标记一下已经配置过了
					
	}
	return 0;
}

