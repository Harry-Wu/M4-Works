#include "stm32f4xx.h" 
#include "key.h"

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

void key_delay(void)
{
	u32 i=0x30000;
	while(i--);
}



u8 key_scanf(void)
{
	static u8  key_sta=1;    //key_sta=1,允许识别按键
	static u8 key_time=0;
	static u8 key1,key2,key3;
	u8 key=NO_KEY;
	
	key3=key2;
	key2=key1;	
	key_time++;
	
	if((key_sta==1)&& ((KEY1==1) || (KEY2==0) || (KEY3==0) || (KEY4==0)))  //允许识别按键，并且有可能是按键按下了
	{
		//key_delay(); //延时消抖
		if(KEY1)   //再次判断是否为高电平
		{
			key1=KEY1_OK;   //识别高按键处理
		}		
		else if(!KEY2)   
		{
			key1=KEY2_OK;  
		}		
		else if(!KEY3)  
		{
			key1=KEY3_OK;   
		}	else if(!KEY4)
		{
			key1=KEY4_OK;  
		}	
		
		
		if(key_time==3)
		{
			key_time=0;
			
			if((key3==key1) && (key2==key1))
			{
				key=key1;
				key_sta=0;     //不允许识别按键
			}
			
		}
	}
	else if((0==KEY1) &&(KEY2==1) &&(KEY3==1) &&(KEY4==1) )   //没有按键按下
	{
		key_sta=1;     //允许识别按键
		key_time=0;
	}
	return key;
}


