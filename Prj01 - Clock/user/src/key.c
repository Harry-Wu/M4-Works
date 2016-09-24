#include "stm32f4xx.h" 
#include "key.h"

u8 KEY2_MODE=0;  //初始状态下, 不允许修改时间

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
				if(key == KEY2_OK)
				{
					KEY2_MODE++;
					if(KEY2_MODE==12) KEY2_MODE=0;
				}
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


