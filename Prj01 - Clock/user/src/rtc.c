#include "rtc.h"
#include "delay.h"
#include "led.h"
#include "stdio.h"

TIME_DATE_TYPEDEF time_date;  //����һ���Զ������͵ı���


//�ȴ�RSFͬ��
//����ֵ:0,�ɹ�;1,ʧ��;
u8 RTC_Wait_Synchro(void)
{
	u32 retry = 0xFFFFF;
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53;
	RTC->ISR&=~(1<<5);		//���RSFλ 
	while(retry&&((RTC->ISR&(1<<5))==0x00))//�ȴ�Ӱ�ӼĴ���ͬ��
	{
		retry--;
	}
	if(retry==0) return 1;  //ͬ��ʧ��
	RTC->WPR = 0XFF;  //ʹ��RTC�Ĵ���д����
	return 0;
}


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


//RTCд�������SRAM, ���������д��Ҫ�ȴ򿪵�Դ�ӿ�ʱ��, ������RTC_Init�����в���
//BKPx:�����Ĵ������,��Χ:0~19
//data:Ҫд�������,32λ����
void RTC_Write_BKR(u32 BKRx,u32 data)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4; //��λBKRx�ĵ�ַ  
	(*(u32*)temp)=data; 
}

//RTC��ȡ������SRAM
//BKPx:�����Ĵ������,��Χ:0~19
//����BKPx�е�����,32λ����
u32 RTC_Read_BKR(u32 BKRx)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4; //��λBKRx�ĵ�ַ  
	return (*(u32*)temp);   //���ض�ȡ����ֵ
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
	u8 temp = 0;
	temp = (val>>4)*10;  //ʮλ����ֵ
	return (temp+(val&0x0F));  //�ټ��ϵ�4λ��Ӧֵ
}

/***************************************************
//RTCʱ������
//hour,min,sec:Сʱ,����,����
//ampm:AM/PM,0=AM/24H,1=PM.
//����ֵ:0,�ɹ�
//       1,�����ʼ��ģʽʧ�� 
***************************************************/
u8 RTC_Set_Time(u8 hour, u8 min, u8 sec, u8 ampm)
{
	u32 temp = 0;
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode()) return 1;  //����RTC��ʼ��ģʽʧ��
	temp = (((u32)ampm&0x01)<<22) | ((u32)RTC_DEC2BCD(hour)<<16) | ((u32)RTC_DEC2BCD(min)<<8) | (RTC_DEC2BCD(sec));
	RTC->TR = temp;  //��ʽ�õ�ʱ��ֵд��ʱ��Ĵ���
	RTC->ISR &= ~(1<<7); //������������ģʽ, ���˳���ʼ��ģʽ
	return 0;
}



/***************************************************
//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:0,�ɹ�
//       1,�����ʼ��ģʽʧ�� 
***************************************************/

u8 RTC_Set_Date(u8 year, u8 month, u8 date, u8 week)
{
	u32 temp = 0;
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode()) return 1;  //����RTC��ʼ��ģʽʧ��
	temp = (((u32)week&0x07)<<13) | ((u32)RTC_DEC2BCD(year)<<16) | ((u32)RTC_DEC2BCD(month)<<8) | (RTC_DEC2BCD(date));
	RTC->DR = temp;  //��ʽ�õ�ʱ��ֵд��ʱ��Ĵ���
	RTC->ISR &= ~(1<<7); //������������ģʽ, ���˳���ʼ��ģʽ
	return 0;
}

/***************************************
//��ȡʱ������ڵĹ��̺����ù�������, ��Ҫ�ǰѼĴ����е�ֵȡ�����ڽṹ�������.
//��ȡʱ��ʱ, Ҫ��֤Ԥװ�ؼĴ������ֵ�Ѿ�ͬ����Ӱ�ӼĴ�����
****************************************/
//��ȡRTCʱ��
//*hour,*min,*sec:Сʱ,����,���� 
//*ampm:AM/PM,0=AM/24H,1=PM.
void RTC_Get_Time(TIME_DATE_TYPEDEF *p)
{
	u32 temp=0;
 	while(RTC_Wait_Synchro());	//�ȴ�ͬ��  	 
	temp=RTC->TR;
	p->hour=RTC_BCD2DEC((temp>>16)&0X3F);
	p->min=RTC_BCD2DEC((temp>>8)&0X7F);
	p->sec=RTC_BCD2DEC(temp&0X7F);
	p->ampm=temp>>22; 
}
//��ȡRTC����
//*year,*mon,*date:��,��,��
//*week:����
void RTC_Get_Date(TIME_DATE_TYPEDEF *p)
{
	u32 temp=0;
 	while(RTC_Wait_Synchro());	//�ȴ�ͬ��  	 
	temp=RTC->DR;
	p->year=RTC_BCD2DEC((temp>>16)&0XFF);
	p->month=RTC_BCD2DEC((temp>>8)&0X1F);
	p->date=RTC_BCD2DEC(temp&0X3F);
	p->week=(temp>>13)&0X07; 
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
		RTC->PRER |= 0X7F<<16;  ////RTC�첽��Ƶ��ϵ��(1~0x7F), [14:0]
		RTC->CR &= ~(1<<6);  //rtc����Ϊ24Сʱ��ʽ
		RTC->ISR &= ~(1<<7);  //�˳�rtc��ʼ��ģʽ
		RTC->WPR = 0XFF;  //д������ֵ��ʹ��rtc�Ĵ���д����
		
		RTC_Set_Time(22,32,0,0);  //����ʱ��
		RTC_Set_Date(16,9,22,4);  //��������
		//RTC_Set_AlarmA(7,0,0,10);  //��������ʱ��
		RTC_Write_BKR(0, 0X5151);
		//RTC->BKP0R = 0x5151;  //���һ���Ѿ����ù���
					
	}
	return 0;
}


/***********************************
//��������Aʱ��
//(����������,24Сʱ��)
//week:���ڼ�(1~7)
//hour,min,sec:Сʱ,����,����
***********************************/
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA; 
	RTC->WPR=0x53;
	RTC->CR &= ~(RTC_CR_ALRAE);  //�ر�����A
	while((RTC->ISR&RTC_ISR_ALRAWF)==0);  //�ȴ�����A�޸�����
	RTC->ALRMAR = 0;  //���ԭ������
	RTC->ALRMAR |= RTC_ALRMAR_WDSEL;  //����������
	RTC->ALRMAR &= ~RTC_ALRMAR_PM;  //24Сʱ��
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(week)<<24;//��������
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(hour)<<16;//Сʱ����
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(min)<<8;	//��������
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(sec);		//��������
	RTC->ALRMASSR=0;						//��ʹ��SUB SEC
	
	RTC->CR |= RTC_CR_ALRAIE;  //��������A�ж�
	RTC->CR |= RTC_CR_ALRAE;  //��������A
	RTC->WPR=0XFF;		//��ֹ�޸�RTC�Ĵ���
	
	RTC->ISR&=~(1<<8);	//���RTC����A�ı�־
	EXTI->PR=1<<17;  	//���LINE17�ϵ��жϱ�־λ  
	EXTI->IMR|=1<<17;	//����line17�ϵ��ж� 
	EXTI->RTSR|=1<<17;	//line17���¼��������ش��� 

	NVIC_SetPriority(RTC_Alarm_IRQn,NVIC_EncodePriority(7-2,2,2));  //�������ȼ�
	NVIC_EnableIRQ(RTC_Alarm_IRQn); //�ⲿ�ж�ʹ�ܣ�ϵͳ�ж�û�����ʹ�ܣ�	
}
	
/***********************************
//�����Ի��Ѷ�ʱ������
//wksel:000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;
//      10x,ck_spre,1Hz;11x,1Hz,��cntֵ����2^16(��cnt+2^16)
//ע��:RTC����RTC��ʱ��Ƶ��,��RTCCLK!
//cnt:�Զ���װ��ֵ.����0,�����ж�.
***********************************/
void RTC_Set_WakeUp(u8 wksel, u16 cnt)
{
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA; 
	RTC->WPR=0x53;
	RTC->CR &= ~(RTC_CR_WUTE);  //�ر�WAKE UP
	while((RTC->ISR&RTC_ISR_WUTWF)==0);  //�ȴ�WAKE UP�޸�����
	RTC->CR &= ~(7<<0);  //���ԭ������
	RTC->CR |= wksel&0x07;  //�����µ�ֵ
	RTC->WUTR=cnt;				//����WAKE UP�Զ���װ�ؼĴ���ֵ
	
	
	RTC->CR|=1<<14;				//����WAKE UP ��ʱ���ж�
	RTC->CR|=1<<10;				//����WAKE UP ��ʱ��
	RTC->WPR=0XFF;				//��ֹ�޸�RTC�Ĵ��� 
	
	RTC->ISR &= ~RTC_ISR_WUTF;  //���RTC WAKE UP�ı�־

	EXTI->PR=1<<22;  			//���LINE22�ϵ��жϱ�־λ  
	EXTI->IMR|=1<<22;			//����line22�ϵ��ж� 
	EXTI->RTSR|=1<<22;			//line22���¼��������ش��� 

	NVIC_SetPriority(RTC_WKUP_IRQn,NVIC_EncodePriority(7-2,2,3));  //�������ȼ�
	NVIC_EnableIRQ(RTC_WKUP_IRQn); //�ⲿ�ж�ʹ�ܣ�ϵͳ�ж�û�����ʹ�ܣ�	
}


////�����Ի��Ѷ�ʱ������
////wksel:000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;
////      10x,ck_spre,1Hz;11x,1Hz,��cntֵ����2^16(��cnt+2^16)
////ע��:RTC����RTC��ʱ��Ƶ��,��RTCCLK!
////cnt:�Զ���װ��ֵ.����0,�����ж�.
//void RTC_Set_WakeUp(u8 wksel,u16 cnt)
//{ 
//	//�ر�RTC�Ĵ���д����
//	RTC->WPR = 0xCA; 
//	RTC->WPR = 0x53;
//	RTC->CR &= ~(1<<10);			//�ر�WAKE UP
//	while((RTC->ISR&0X04)==0);	//�ȴ�WAKE UP�޸�����
//	RTC->CR &= ~(7<<0);			//���ԭ���Ļ���ʱ��Ƶ������
//	RTC->CR |= wksel&0X07;		//�����µ�ֵ
//	RTC->WUTR = cnt;           //����WAKE UP�Զ���װ�ؼĴ���ֵ
//	RTC->ISR&=~(1<<10);			//���RTC WAKE UP�ı�־
//	RTC->CR|=1<<14;				//����WAKE UP ��ʱ���ж�
//	EXTI->PR=1<<22;  			//���LINE22�ϵ��жϱ�־λ  
//	EXTI->IMR|=1<<22;			//����line22�ϵ��ж�
//	EXTI->RTSR|=1<<22;			//line22���¼��������ش���
//	NVIC_SetPriority(RTC_WKUP_IRQn,NVIC_EncodePriority(7-2,2,3));  //�������ȼ�
//	NVIC_EnableIRQ(RTC_WKUP_IRQn);  //�ⲿ�ж�ʹ�ܣ�ϵͳ�ж�û�����ʹ��)
//	RTC->CR|=1<<10;				//����WAKE UP ��ʱ��
//	RTC->WPR=0XFF;				//��ֹ�޸�RTC�Ĵ��� 
//}

//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{
	if(RTC->ISR&RTC_ISR_ALRAF)  //��ѯAlam A�жϱ�־λ
	{
		RTC->ISR &= ~RTC_ISR_ALRAF;  //����жϱ�־
		printf("Alam A!\r\n");
		BEEP = 1;
	}
	EXTI->PR |= EXTI_PR_PR17;  //����ж���17���жϱ�־
}

//RTC WAKE UP�жϷ�����
void RTC_WKUP_IRQHandler(void)
{ 
	EXTI->PR|=1<<22;	//����ж���22���жϱ�־   
	if(RTC->ISR&(1<<10))//WK_UP�жϱ�־λ?
	{ 
		RTC->ISR&=~(1<<10);	//����жϱ�־
		RTC_Get_Time(&time_date);
		RTC_Get_Date(&time_date);
	}
	LED6 = !LED6;
									
}
	

//�������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//year,month,day������������ 
//����ֵ�����ں�(1~7,������1~����)
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	 
u8 Calcu_Week(u16 year, u8 month, u8 day)
{
	u16 temp2;
	u8 yearH, yearL;
	yearH=year/100; yearL=year%100;
	//���Ϊ21����, �������100
	if(yearH>19) yearL += 100;
	//����������ֻ����1900��֮���
	temp2 = yearL+yearL/4;
	temp2 = temp2%7;
	temp2 = temp2 + day + table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	temp2%=7;
	if(temp2==0)temp2=7;
	return temp2;
}
