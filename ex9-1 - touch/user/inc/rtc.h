#ifndef _RTC_H_
#define _RTC_H_
#include "stm32f4xx.h"

typedef struct {
	u8 hour;
	u8 min;
	u8 sec;
	u8 ampm;
	u8 year;
	u8 month;
	u8 date;
	u8 week;
} TIME_DATE_TYPEDEF;

extern TIME_DATE_TYPEDEF time_date;

u8 RTC_Init(void);						//RTC初始化
u8 RTC_Wait_Synchro(void);				//等待同步
u8 RTC_Init_Mode(void);					//进入初始化模式
void RTC_Write_BKR(u32 BKRx,u32 data);	//写后备区域SRAM
u32 RTC_Read_BKR(u32 BKRx);				//读后备区域SRAM
u8 RTC_DEC2BCD(u8 val);					//十进制转换为BCD码
u8 RTC_BCD2DEC(u8 val);					//BCD码转换为十进制数据
u8 RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTC时间设置
u8 RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC日期设置
void RTC_Get_Time(TIME_DATE_TYPEDEF *p);	//获取RTC时间
void RTC_Get_Date(TIME_DATE_TYPEDEF *p);//获取RTC日期
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		//设置闹钟时间(按星期闹铃,24小时制)
void RTC_Set_WakeUp(u8 wksel,u16 cnt);					//周期性唤醒定时器设置
u8 Calcu_Week(u16 year,u8 month,u8 day);				//根据输入的年月日,计算当日所属星期几
//void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);	 
//void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);	


#endif
