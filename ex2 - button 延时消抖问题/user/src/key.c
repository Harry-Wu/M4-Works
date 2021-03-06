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
	static u8 key_time = 0;
	static u8 key1,key2,key3;
	u8 key = NO_KEY;
	
	key3 = key2;
	key2 = key1;
	key_time++;
	
	if((key_sta==1)&& KEY1)  //如果识别为高电平,并且允许识别按键
	{
		//delay_ms(20);  //延时消抖
		if(KEY1)  //再次判断是否为高电平
		{
			key1 = KEY1_OK; //识别为高,但还不能判定是否真的有按键按下,暂时把值赋给key1
			//key_sta = 0; //不允许识别按键
			//while (KEY1); //判断按键是否释放
		}
		if(key_time==3)
		{
			key_time = 0;
			if((key3==key1) && (key2==key1))
			{
				key = key1;   //最终确定有按键按下了,把最终的值赋给key
				key_sta = 0;  //不允许识别按键
			}
		}
	}
	else if(!KEY1) //低电平,没有按键按下
		{
			key_sta = 1; //允许识别按键
			key_time = 0; //识别一次按键按下后,初始化key_time的值
		}
	return key;
}
