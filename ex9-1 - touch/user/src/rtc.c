#include "rtc.h"
#include "delay.h"

TIME_DATE_TYPEDEF time_date;  //����һ���Զ������͵ı���


//RTC�����ʼ��ģʽ
//����ֵ:0,�ɹ�;1,ʧ��;
u8 RTC_Init_Mode(void)
{ 
	u32 retry=0X10000; 
	if(RTC->ISR&(1<<6))return 0; 
	RTC->ISR|=1<<7;	//����RTC��ʼ��ģʽ
	while(retry&&((RTC->ISR&(1<<6))==0x00))//�ȴ�����RTC��ʼ��ģʽ�ɹ�
	{
		retry--;
	}
    if(retry==0)return 1;	//ͬ��ʧ��
	else return 0; 			//ͬ���ɹ� 
}
//RTCд�������SRAM
//BKPx:�����Ĵ������,��Χ:0~19
//data:Ҫд�������,32λ����
void RTC_Write_BKR(u32 BKRx,u32 data)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4;   
	(*(u32*)temp)=data; 
}
//RTC��ȡ������SRAM
//BKPx:�����Ĵ������,��Χ:0~19
//����ֵ:��ȡ��������
u32 RTC_Read_BKR(u32 BKRx)
{
	u32 temp=0; 
	
	temp=RTC_BASE+0x50+BKRx*4;  
	return (*(u32*)temp);		//���ض�ȡ����ֵ
	
}

//ʮ����ת��ΪBCD��
//val:Ҫת����ʮ������
//����ֵ:BCD��
u8 RTC_DEC2BCD(u8 val)
{
	u8 bcdhigh = 0; 
	while(val>=10)
	{
		bcdhigh++;
		val-=10;
	} 
	return ((u8)(bcdhigh<<4)|val);
}
//BCD��ת��Ϊʮ��������
//val:Ҫת����BCD��
//����ֵ:ʮ��������
u8 RTC_BCD2DEC(u8 val)
{
	u8 temp=0;
	temp=(val>>4)*10;
	return (temp+(val&0X0F));
}
//RTCʱ������
//hour,min,sec:Сʱ,����,����
//ampm:AM/PM,0=AM/24H,1=PM.
//����ֵ:0,�ɹ�
//       1,�����ʼ��ģʽʧ�� 
u8 RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	u32 temp=0;
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode())return 1;//����RTC��ʼ��ģʽʧ��
	temp=(((u32)ampm&0X01)<<22)|((u32)RTC_DEC2BCD(hour)<<16)|((u32)RTC_DEC2BCD(min)<<8)|(RTC_DEC2BCD(sec));
	RTC->TR=temp;
	RTC->ISR&=~(1<<7);			//�˳�RTC��ʼ��ģʽ 
	return 0; 
}
/***************************************************
//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:0,�ɹ�
//       1,�����ʼ��ģʽʧ�� 
***************************************************/
u8 RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	u32 temp=0;
 	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode())return 1;//����RTC��ʼ��ģʽʧ��
	temp=(((u32)week&0X07)<<13)|((u32)RTC_DEC2BCD(year)<<16)|((u32)RTC_DEC2BCD(month)<<8)|(RTC_DEC2BCD(date)); 
	RTC->DR=temp;
	RTC->ISR&=~(1<<7);			//�˳�RTC��ʼ��ģʽ 
	return 0; 
}

/***********************************
//��ʼ��RTC
//����ֵ�� 0--��ʼ���ɹ�
		   1--LSE����ʧ��
		   2--�����ʼ��ģʽʧ��
***********************************/
u8 RTC_Init(void)
{
	u16 retry = 0x1FFF;
	RCC->APB1ENR |= 1<<28;  //ʹ�ܵ�Դ�ӿ�ʱ��
	PWR->CR |= 1<<8;  //���������ʹ��(RTC+SRAM)
	
	if(RTC->BKP0R != 0X5151)  //�Ƿ��һ�����ã� 0x5151���Լ����õ����壬�����Ƿ��һ�����õı�־
	{
		RCC->BDCR |= 1<<0;  //LSE����
		while(retry&&((RCC->BDCR&0x02)==0))  //�ȴ�LSE׼����
		{
			retry--;
			delay_ms(5);
		}
		if(retry==0) return 1;  //LSE����ʧ��
		
		RCC->BDCR |= 1<<8;  //ѡ��LSE��ΪRTC��ʱ��
		RCC->BDCR |= 1<<15;  //ʹ��RTCʱ��
		
		//�ر�RTC�Ĵ���д����
		RTC->WPR=0xCA;
		RTC->WPR=0x53;
		if(RTC_Init_Mode()) return 2;  //����RTC��ʼ��ģʽʧ��
		RTC->PRER = 0XFF;  //RTCͬ����Ƶ��ϵ��(0~7FFF), [14:0]
		RTC->PRER = 0X7F<<16;  ////RTC�첽��Ƶ��ϵ��(1~0x7F), [14:0]
		RTC->CR &= ~(1<<6);  //rtc����Ϊ24Сʱ��ʽ
		RTC->ISR &= ~(1<<7);  //�˳�rtc��ʼ��ģʽ
		RTC->WPR = 0XFF;  //д������ֵ��ʹ��rtc�Ĵ���д����
		
		RTC_Set_Time(22,50,0,0);  //����ʱ��
		RTC_Set_Date(16,9,17,2);  //��������
		//RTC_Set_AlarmA(7,0,0,10);  //��������ʱ��
		RTC->BKP0R = 0x5151;  //���һ���Ѿ����ù���
					
	}
	return 0;
}

