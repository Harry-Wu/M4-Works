#include "stm32f4xx.h" 
#include "key.h"

void key_init(void)
{
	//端口时钟使能PA0
	RCC->AHB1ENR |=0X01<<0;
	//配置管脚PA0---普通功能浮空输入
	GPIOA->MODER &=~(0X03<<0);  //PA0 普通功能输入
	GPIOA->PUPDR &=~(0X03<<0);  //PA0无上下拉
	
	
	//端口时钟使能PE
	RCC->AHB1ENR |=0X01<<4;
	//配置管脚Pe2 3 4---普通功能浮空输入
	GPIOE->MODER &=~(0X03F<<4);
	GPIOE->PUPDR &=~(0X03F<<4);
}


uint8_t  key_scanf(void)
{
	static uint8_t key_sta=1;  //key_sta=1则允许识别按键
	static uint8_t key1=NO_KEY,key2,key3,time=0;
	unsigned char key=NO_KEY;
	key3=key2;
	key2=key1;
  time++;
	if((key_sta==1)&&((KEY1!=0)||(KEY2==0)||(KEY3==0)||(KEY4==0))) //,允许识别按键，并且有一个按键按下
	{
		//Delay5ms();  //延时消抖
		if(KEY1!=0)   //再次判断是否有按键按下
		{
			key1=KEY1_OK;   //按键按下处理
			//while(p14==0);  //等待按键释放
			//key_sta=0;   //不允许识别按键
		}
		else if(KEY2==0)   //再次判断是否有按键按下
		{
			key1=KEY2_OK;   //按键按下处理
			//while(p14==0);  //等待按键释放
			//key_sta=0;   //不允许识别按键
		}
			else if(KEY3==0)   //再次判断是否有按键按下
		{
			key1=KEY3_OK;   //按键按下处理
			//while(p14==0);  //等待按键释放
			//key_sta=0;   //不允许识别按键
		}
			else if(KEY4==0)   //再次判断是否有按键按下
		{
			key1=KEY4_OK;   //按键按下处理
			//while(p14==0);  //等待按键释放
			//key_sta=0;   //不允许识别按键
		}
		if(time==3)
		{
			time=0;
			if(key1==key2 && key1==key3 && key1!=NO_KEY)
			{
				key=key1;
				key_sta=0;   //不允许识别按键
			}
		}
	}
	else if((KEY1==0)&&(KEY2!=0)&&(KEY3!=0)&&(KEY4!=0))  //所有按键都释放
	{
			key_sta=1;   //允许识别按键
			time=0;
	}
	return key;
}




