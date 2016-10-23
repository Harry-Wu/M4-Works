#include "stm32f4xx.h"
#include "key.h"

void key_init(void)
{
	//端口时钟使能PA PE
	RCC->AHB1ENR |= 0X01<<0 | 0X01<<4;
	//管脚配置
	GPIOA->MODER &=~(0X03<<0);  //pa0普通功能输入
	GPIOA->PUPDR &=~(0X03<<0);  //浮空输入	
	
	GPIOE->MODER &=~(0X03<<4 | 0X03<<6 | 0X03<<8);  //pE 2 3 4普通功能输入
	GPIOE->PUPDR &=~(0X03<<4 | 0X03<<6 | 0X03<<8);   //浮空输入	
	
	
}

void key_delay(void)
{
	u32 i=0x100;
	while(i--);
}


u8 key_scanf(void)
{	
	u8 key=NO_KEY;
	static u8 key_sta=1;   //=1允许是被按键。。。
	static u8 key1,key2,key3,time=0;
	key3=key2;
	key2=key1;
	time++;
	if((key_sta==1)&& ((KEY1==1) || (KEY2==0)  || (KEY3==0) || (KEY4==0)))//判断是否允许识别按键 并且 是否有按键按下
	{
		//key_delay();//延时消抖
		//再次判断是否为有效电平
		if(KEY1)  //按键的确是被按下了
		{
			key1=KEY1_OK;  //识别到按键处理
		}
		else if(0==KEY2)
		{
			key1=KEY2_OK;  //识别到按键处理
		}
		else if(0==KEY3)
		{
			key1=KEY3_OK;  //识别到按键处理
		}
		else if(0==KEY4)
		{
			key1=KEY4_OK;  //识别到按键处理
		}
		if(time==3)
		{
			time=0;
			if((key1==key2) && (key1==key3))
			{
				key=key1;
				key_sta=0;  //不允许识别按键
			}
		}
	}
	else if((0==KEY1)&&(1==KEY2)&&(1==KEY3)&&(1==KEY4))//没有按键被按下
	{
		key_sta=1;  //允许识别按键
		time=0;
	}
	return key;
	
}


