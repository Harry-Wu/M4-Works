#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"

void key_init(void)
{
	//端口时钟使能
	RCC->AHB1ENR |= 0x01<<0;
	//端口配置, PA0配置为普通功能,浮空输入
	GPIOA->MODER &=~(0x03<<0);//普通输入
	GPIOA->PUPDR &=~(0x03<<0);//无上下拉
}

u8 key_scanf(void)
{
	static u8 key_sta = 1;  //key_sta=1,允许识别按键. 按键一直按着的时候,不允许识别
	u8 key = NO_KEY;
	if((key_sta==1)&& KEY1)  //如果识别为高电平,并且允许识别按键
	{
		delay_ms(20);  //延时消抖
		if(KEY1)  //再次判断是否为高电平
		{
			key = KEY1_OK; //识别为高,按键处理
			key_sta = 0; //不允许识别按键
			//while (KEY1); //判断按键是否释放
		}		
	}
	else if(!KEY1) //低电平,没有按键按下
		{
			key_sta = 1; //允许识别按键
		}
	return key;
}
