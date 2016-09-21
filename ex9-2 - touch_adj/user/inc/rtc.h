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

u8 RTC_Init(void);						//RTC��ʼ��
u8 RTC_Wait_Synchro(void);				//�ȴ�ͬ��
u8 RTC_Init_Mode(void);					//�����ʼ��ģʽ
void RTC_Write_BKR(u32 BKRx,u32 data);	//д������SRAM
u32 RTC_Read_BKR(u32 BKRx);				//��������SRAM
u8 RTC_DEC2BCD(u8 val);					//ʮ����ת��ΪBCD��
u8 RTC_BCD2DEC(u8 val);					//BCD��ת��Ϊʮ��������
u8 RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTCʱ������
u8 RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC��������
void RTC_Get_Time(TIME_DATE_TYPEDEF *p);	//��ȡRTCʱ��
void RTC_Get_Date(TIME_DATE_TYPEDEF *p);//��ȡRTC����
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		//��������ʱ��(����������,24Сʱ��)
void RTC_Set_WakeUp(u8 wksel,u16 cnt);					//�����Ի��Ѷ�ʱ������
u8 Calcu_Week(u16 year,u8 month,u8 day);				//���������������,���㵱���������ڼ�
//void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);	 
//void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);	


#endif
