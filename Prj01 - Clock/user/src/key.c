/*****************************************************************************
@File name:  
@Description: 
	����KEY2������˳��޸�ʱ��ģʽ(KEY2_MODE==1), �����޸�ģʽʱ,��Ӧ��ʱ���ֶα���ɫ��Ϊ��ɫ;
	ʱ���޸�ģʽ��,�̰�KEY1������ֵ, �̰�KEY3��С��ֵ;
	���޸�ģʽ��,KEY1����D5, KEY3����D6, KEY4��������BEEP
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

u8 KEY2_MODE=0;  //��ʼ״̬��, �������޸�ʱ��
u8 setting_mode=1;  //ʱ������ģʽ
u16 lngcounter=0; //��������ʱ���������  ȫ�ֱ���
u8 key_value, key2_value;  //����key_scanf��key_scanf_longshort�ķ���ֵ

void key_init(void)
{
	//�˿�ʱ��ʹ��PA
	RCC->AHB1ENR |=0X01<<0;
	//�˿�����PA0����Ϊ��ͨ���ܸ�������
	GPIOA->MODER &=~(0X03<<0); //��ͨ��������ģʽ
	GPIOA->PUPDR &=~(0X03<<0); //��������
	
	
		//�˿�ʱ��ʹ��PE
	RCC->AHB1ENR |=0X01<<4;
	//�˿�����PE 2 3 4����Ϊ��ͨ���ܸ�������
	GPIOE->MODER &=~(0X03<<4 | 0X03<<6 |0X03<<8); //��ͨ��������ģʽ
	GPIOE->PUPDR &=~(0X03<<4 | 0X03<<6 |0X03<<8);  //��������
	
	
}




//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0xFF��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY4����
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2>KEY3>KEY4!!
u8 key_scanf(u8 mode)
{
	static u8  key_sta=1;    //key_sta=1,����ʶ�𰴼�
	static u8 key_time=0;

	static u8 key1,key2,key3;
	u8 key=NO_KEY;
	
	if(mode)
	{
		key_sta=1;  //�����ɿ���־, ֧������
	}
	
	key3=key2;
	key2=key1;	
	key_time++;
	
	if((key_sta==1)&& ((KEY1==1) || (KEY3==0) || (KEY4==0)))  //����ʶ�𰴼��������п����ǰ���������
	{
		//key_delay(); //��ʱ����
		if(KEY1)   //�ٴ��ж��Ƿ�Ϊ�ߵ�ƽ
		{
			key1=KEY1_OK;   //ʶ��߰�������
		}		
		
		else if(!KEY3)  
		{
			key1=KEY3_OK;   
		}	
		else if(!KEY4)
		{
			key1=KEY4_OK;  
		}	
		
		
		if(key_time==3)   //���ʶ������
		{
			key_time=0;
			
			if((key3==key1) && (key2==key1))
			{
				key=key1;
				
				key_sta=0;     //������ʶ�𰴼�
			}
			
		}
	}
	else if((0==KEY1) &&(KEY3==1) &&(KEY4==1) )   //û�а�������
	{
		key_sta=1;     //����ʶ�𰴼�
		key_time=0;
	}
	return key;
}

/***********************
** ��������: keyscanf_longshort()
** ��������: ��⵽һ�ΰ������²�������
** �䡡 ��: �����ź�
** �� ��: 0:�ް��� 1:һ�ε��� 2��һ�γ���
** ȫ�ֱ���: lngcounter
** ����ģ��: 
** ˵���� lngcounter�����ж�ʱ��Ϊһ����Ķ�ʱ�����Լ�һ��
** ע�⣺
********************/
u8 keyscanf_longshort(void)
{
	if(KEY2==0)
	{
		delay_ms(10);  //��ʱ����
		if(KEY2==0)
		{
			TIM13->CR1 |=  0X01<<0;  //ʹ�ܶ�ʱ��
			while(KEY2==0) display_tim();
			TIM13->CR1 &=  ~(0x01<<0); //���ܶ�ʱ��
			if(lngcounter>2000)  //�������ʱ�䳬��2s,��Ϊ����, ����ֵ2
			{
				lngcounter = 0;
				KEY2_MODE = !KEY2_MODE;  //����������ͽ�ֹ�޸�ʱ��֮���л�
				return 2;
			}
			else  //�̰�
			{
				lngcounter = 0;
				if(KEY2_MODE) //����̰�ǰ���޸�ʱ��ģʽ
				{
					setting_mode++;
					if(setting_mode==12) setting_mode=1;
				}
			}
			return 1;
		}
		else
		return 0;  //��ʱ������û�м�⵽����
	}	
	else
	return 0;
}

void key_service(void)
{
	//key2_value = keyscanf_longshort();
	key_value = key_scanf(0);
	if(KEY2_MODE != 0)  //�������ʱ���޸�ģʽ
	{
		if(key_value !=NO_KEY)  //����а�������
		{
			RTC_Get_Time(&time_date);
			RTC_Get_Date(&time_date);
		}
		switch(setting_mode)
		{
		//����ʱ��,����ģʽ
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
			
		//��������ģʽ	
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
	else if(key_value != NO_KEY)  //������ڷ�ʱ���޸�ģʽ, �����а�������
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
