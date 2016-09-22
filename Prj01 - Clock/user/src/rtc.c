#include "rtc.h"
#include "delay.h"
#include "led.h"
#include "stdio.h"

TIME_DATE_TYPEDEF time_date;  //定义一个自定义类型的变量


//等待RSF同步
//返回值:0,成功;1,失败;
u8 RTC_Wait_Synchro(void)
{
	u32 retry = 0xFFFFF;
	//关闭RTC寄存器写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53;
	RTC->ISR&=~(1<<5);		//清除RSF位 
	while(retry&&((RTC->ISR&(1<<5))==0x00))//等待影子寄存器同步
	{
		retry--;
	}
	if(retry==0) return 1;  //同步失败
	RTC->WPR = 0XFF;  //使能RTC寄存器写保护
	return 0;
}


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


//RTC写入后备区域SRAM, 备份区域读写都要先打开电源接口时钟, 可以在RTC_Init函数中操作
//BKPx:后备区寄存器编号,范围:0~19
//data:要写入的数据,32位长度
void RTC_Write_BKR(u32 BKRx,u32 data)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4; //定位BKRx的地址  
	(*(u32*)temp)=data; 
}

//RTC读取后备区域SRAM
//BKPx:后备区寄存器编号,范围:0~19
//返回BKPx中的数据,32位长度
u32 RTC_Read_BKR(u32 BKRx)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4; //定位BKRx的地址  
	return (*(u32*)temp);   //返回读取到的值
}


//十进制转换为BCD码
//val:要转换的十进制数
//返回值:BCD码
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
//BCD码转换为十进制数据
//val:要转换的BCD码
//返回值:十进制数据
u8 RTC_BCD2DEC(u8 val)
{
	u8 temp = 0;
	temp = (val>>4)*10;  //十位以上值
	return (temp+(val&0x0F));  //再加上低4位对应值
}

/***************************************************
//RTC时间设置
//hour,min,sec:小时,分钟,秒钟
//ampm:AM/PM,0=AM/24H,1=PM.
//返回值:0,成功
//       1,进入初始化模式失败 
***************************************************/
u8 RTC_Set_Time(u8 hour, u8 min, u8 sec, u8 ampm)
{
	u32 temp = 0;
	//关闭RTC寄存器写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode()) return 1;  //进入RTC初始化模式失败
	temp = (((u32)ampm&0x01)<<22) | ((u32)RTC_DEC2BCD(hour)<<16) | ((u32)RTC_DEC2BCD(min)<<8) | (RTC_DEC2BCD(sec));
	RTC->TR = temp;  //格式好的时间值写入时间寄存器
	RTC->ISR &= ~(1<<7); //返回自由运行模式, 即退出初始化模式
	return 0;
}



/***************************************************
//RTC日期设置
//year,month,date:年(0~99),月(1~12),日(0~31)
//week:星期(1~7,0,非法!)
//返回值:0,成功
//       1,进入初始化模式失败 
***************************************************/

u8 RTC_Set_Date(u8 year, u8 month, u8 date, u8 week)
{
	u32 temp = 0;
	//关闭RTC寄存器写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode()) return 1;  //进入RTC初始化模式失败
	temp = (((u32)week&0x07)<<13) | ((u32)RTC_DEC2BCD(year)<<16) | ((u32)RTC_DEC2BCD(month)<<8) | (RTC_DEC2BCD(date));
	RTC->DR = temp;  //格式好的时间值写入时间寄存器
	RTC->ISR &= ~(1<<7); //返回自由运行模式, 即退出初始化模式
	return 0;
}

/***************************************
//获取时间和日期的过程和设置过程相逆, 主要是把寄存器中的值取出放在结构体变量中.
//获取时间时, 要保证预装载寄存器里的值已经同步到影子寄存器里
****************************************/
//获取RTC时间
//*hour,*min,*sec:小时,分钟,秒钟 
//*ampm:AM/PM,0=AM/24H,1=PM.
void RTC_Get_Time(TIME_DATE_TYPEDEF *p)
{
	u32 temp=0;
 	while(RTC_Wait_Synchro());	//等待同步  	 
	temp=RTC->TR;
	p->hour=RTC_BCD2DEC((temp>>16)&0X3F);
	p->min=RTC_BCD2DEC((temp>>8)&0X7F);
	p->sec=RTC_BCD2DEC(temp&0X7F);
	p->ampm=temp>>22; 
}
//获取RTC日期
//*year,*mon,*date:年,月,日
//*week:星期
void RTC_Get_Date(TIME_DATE_TYPEDEF *p)
{
	u32 temp=0;
 	while(RTC_Wait_Synchro());	//等待同步  	 
	temp=RTC->DR;
	p->year=RTC_BCD2DEC((temp>>16)&0XFF);
	p->month=RTC_BCD2DEC((temp>>8)&0X1F);
	p->date=RTC_BCD2DEC(temp&0X3F);
	p->week=(temp>>13)&0X07; 
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
		RTC->PRER |= 0X7F<<16;  ////RTC异步分频器系数(1~0x7F), [14:0]
		RTC->CR &= ~(1<<6);  //rtc设置为24小时格式
		RTC->ISR &= ~(1<<7);  //退出rtc初始化模式
		RTC->WPR = 0XFF;  //写入任意值可使能rtc寄存器写保护
		
		RTC_Set_Time(22,32,0,0);  //设置时间
		RTC_Set_Date(16,9,22,4);  //设置日期
		//RTC_Set_AlarmA(7,0,0,10);  //设置闹钟时间
		RTC_Write_BKR(0, 0X5151);
		//RTC->BKP0R = 0x5151;  //标记一下已经配置过了
					
	}
	return 0;
}


/***********************************
//设置闹钟A时间
//(按星期闹铃,24小时制)
//week:星期几(1~7)
//hour,min,sec:小时,分钟,秒钟
***********************************/
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
{
	//关闭RTC寄存器写保护
	RTC->WPR=0xCA; 
	RTC->WPR=0x53;
	RTC->CR &= ~(RTC_CR_ALRAE);  //关闭闹钟A
	while((RTC->ISR&RTC_ISR_ALRAWF)==0);  //等待闹钟A修改允许
	RTC->ALRMAR = 0;  //清空原来设置
	RTC->ALRMAR |= RTC_ALRMAR_WDSEL;  //按星期闹铃
	RTC->ALRMAR &= ~RTC_ALRMAR_PM;  //24小时制
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(week)<<24;//星期设置
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(hour)<<16;//小时设置
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(min)<<8;	//分钟设置
	RTC->ALRMAR|=(u32)RTC_DEC2BCD(sec);		//秒钟设置
	RTC->ALRMASSR=0;						//不使用SUB SEC
	
	RTC->CR |= RTC_CR_ALRAIE;  //开启闹钟A中断
	RTC->CR |= RTC_CR_ALRAE;  //开启闹钟A
	RTC->WPR=0XFF;		//禁止修改RTC寄存器
	
	RTC->ISR&=~(1<<8);	//清除RTC闹钟A的标志
	EXTI->PR=1<<17;  	//清除LINE17上的中断标志位  
	EXTI->IMR|=1<<17;	//开启line17上的中断 
	EXTI->RTSR|=1<<17;	//line17上事件上升降沿触发 

	NVIC_SetPriority(RTC_Alarm_IRQn,NVIC_EncodePriority(7-2,2,2));  //设置优先级
	NVIC_EnableIRQ(RTC_Alarm_IRQn); //外部中断使能（系统中断没有这个使能）	
}
	
/***********************************
//周期性唤醒定时器设置
//wksel:000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;
//      10x,ck_spre,1Hz;11x,1Hz,且cnt值增加2^16(即cnt+2^16)
//注意:RTC就是RTC的时钟频率,即RTCCLK!
//cnt:自动重装载值.减到0,产生中断.
***********************************/
void RTC_Set_WakeUp(u8 wksel, u16 cnt)
{
	//关闭RTC寄存器写保护
	RTC->WPR=0xCA; 
	RTC->WPR=0x53;
	RTC->CR &= ~(RTC_CR_WUTE);  //关闭WAKE UP
	while((RTC->ISR&RTC_ISR_WUTWF)==0);  //等待WAKE UP修改允许
	RTC->CR &= ~(7<<0);  //清空原来设置
	RTC->CR |= wksel&0x07;  //设置新的值
	RTC->WUTR=cnt;				//设置WAKE UP自动重装载寄存器值
	
	
	RTC->CR|=1<<14;				//开启WAKE UP 定时器中断
	RTC->CR|=1<<10;				//开启WAKE UP 定时器
	RTC->WPR=0XFF;				//禁止修改RTC寄存器 
	
	RTC->ISR &= ~RTC_ISR_WUTF;  //清除RTC WAKE UP的标志

	EXTI->PR=1<<22;  			//清除LINE22上的中断标志位  
	EXTI->IMR|=1<<22;			//开启line22上的中断 
	EXTI->RTSR|=1<<22;			//line22上事件上升降沿触发 

	NVIC_SetPriority(RTC_WKUP_IRQn,NVIC_EncodePriority(7-2,2,3));  //设置优先级
	NVIC_EnableIRQ(RTC_WKUP_IRQn); //外部中断使能（系统中断没有这个使能）	
}


////周期性唤醒定时器设置
////wksel:000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;
////      10x,ck_spre,1Hz;11x,1Hz,且cnt值增加2^16(即cnt+2^16)
////注意:RTC就是RTC的时钟频率,即RTCCLK!
////cnt:自动重装载值.减到0,产生中断.
//void RTC_Set_WakeUp(u8 wksel,u16 cnt)
//{ 
//	//关闭RTC寄存器写保护
//	RTC->WPR = 0xCA; 
//	RTC->WPR = 0x53;
//	RTC->CR &= ~(1<<10);			//关闭WAKE UP
//	while((RTC->ISR&0X04)==0);	//等待WAKE UP修改允许
//	RTC->CR &= ~(7<<0);			//清除原来的唤醒时钟频率设置
//	RTC->CR |= wksel&0X07;		//设置新的值
//	RTC->WUTR = cnt;           //设置WAKE UP自动重装载寄存器值
//	RTC->ISR&=~(1<<10);			//清除RTC WAKE UP的标志
//	RTC->CR|=1<<14;				//开启WAKE UP 定时器中断
//	EXTI->PR=1<<22;  			//清除LINE22上的中断标志位  
//	EXTI->IMR|=1<<22;			//开启line22上的中断
//	EXTI->RTSR|=1<<22;			//line22上事件上升降沿触发
//	NVIC_SetPriority(RTC_WKUP_IRQn,NVIC_EncodePriority(7-2,2,3));  //设置优先级
//	NVIC_EnableIRQ(RTC_WKUP_IRQn);  //外部中断使能（系统中断没有这个使能)
//	RTC->CR|=1<<10;				//开启WAKE UP 定时器
//	RTC->WPR=0XFF;				//禁止修改RTC寄存器 
//}

//RTC闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{
	if(RTC->ISR&RTC_ISR_ALRAF)  //查询Alam A中断标志位
	{
		RTC->ISR &= ~RTC_ISR_ALRAF;  //清除中断标志
		printf("Alam A!\r\n");
		BEEP = 1;
	}
	EXTI->PR |= EXTI_PR_PR17;  //清除中断线17的中断标志
}

//RTC WAKE UP中断服务函数
void RTC_WKUP_IRQHandler(void)
{ 
	EXTI->PR|=1<<22;	//清除中断线22的中断标志   
	if(RTC->ISR&(1<<10))//WK_UP中断标志位?
	{ 
		RTC->ISR&=~(1<<10);	//清除中断标志
		RTC_Get_Time(&time_date);
		RTC_Get_Date(&time_date);
	}
	LED6 = !LED6;
									
}
	

//计算现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//year,month,day：公历年月日 
//返回值：星期号(1~7,代表周1~周日)
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	 
u8 Calcu_Week(u16 year, u8 month, u8 day)
{
	u16 temp2;
	u8 yearH, yearL;
	yearH=year/100; yearL=year%100;
	//如果为21世纪, 年份数加100
	if(yearH>19) yearL += 100;
	//所过闰年数只计算1900年之后的
	temp2 = yearL+yearL/4;
	temp2 = temp2%7;
	temp2 = temp2 + day + table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	temp2%=7;
	if(temp2==0)temp2=7;
	return temp2;
}
