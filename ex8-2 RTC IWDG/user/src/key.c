#include "stm32f4xx.h" 
#include "KEY.h" 
//按键端口初始化
void key_init(void)
{
	//时钟使能Pa Pe
	RCC->AHB1ENR |= 0X01<<4 | 0X01<<0 ;
	//端口初始化  pe2/3/4 --普通功能浮空输入
	GPIOE->MODER &=~(0X03<<4 |0X03<<6 |0X03<<8);  //pe2/3/4为输入模式
	GPIOE->PUPDR &=~(0X03<<4 |0X03<<6 |0X03<<8);  //浮空输入
	
	//端口初始化  pA0 --普通功能浮空输入
	GPIOA->MODER &=~(0X03<<0);  //pA0为输入模式
	GPIOA->PUPDR &=~(0X03<<0);  //浮空输入
	
}


void key_delay(void)
{
	u32 i=0x5000;
	while(i--);
}


//按键识别
//返回值： 识别到的按键的标志
u8 key_scanf(void)
{
	static u8 key_sta=1;   //key_sta=1允许识别按键，key_sta=0不允许识别按键
	static u8 key1,key2,key3,num=0;
	u8 key=NO_KEY;
	
	key3=key2;
	key2=key1;
	num++;
	if((key_sta==1) &&((KEY2==0) ||(KEY1==0)  ||(KEY0==0) ||(WKUP_KEY==1) ))  //允许识别按键，并且有按键按下
	{
	//	key_delay();  //延时消抖
		if(KEY2==0)  //再次判断是否有按键按下
		{
			//key_sta=0;     //不允许识别按键
			key1=KEY2_OK;
		}
		else if(KEY1==0)  //再次判断是否有按键按下
		{
			//key_sta=0;     //不允许识别按键
			key1=KEY1_OK;
		}
		else if(KEY0==0)  //再次判断是否有按键按下
		{
			//key_sta=0;     //不允许识别按键
			key1=KEY0_OK;
		}
			else if(WKUP_KEY==1)  //再次判断是否有按键按下
		{
			//key_sta=0;     //不允许识别按键
			key1=WKUP_KEY_OK;
		}
		//while((KEY2==0) ||(KEY1==0) ||(KEY0==0) ||(WKUP_KEY==1));//等待按键释放
	if(num==3)
	{
		num=0;
		if((key1==key3)&&(key1==key2))
		{
			key=key1;
			key_sta=0;     //不允许识别按键(状态机)
		}
	}
	
	}
	else if((KEY2==1) &&(KEY1==1) &&(KEY0==1) &&(WKUP_KEY==0)) //所有的按键都释放了
	{
		key_sta=1;     //允许下次识别按键
		num=0;
	}
	
	return key;
	
}
	




