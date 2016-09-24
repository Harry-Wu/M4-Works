#include "stm32f4xx.h" 
#include "key.h"

u8 KEY2_MODE=0;  //��ʼ״̬��, �������޸�ʱ��

void key_init(void)
{
	//�˿�ʱ��ʹ��PA
	RCC->AHB1ENR |=0X01<<0;
	//�˿�����PA0����Ϊ��ͨ���ܸ�������
	GPIOA->MODER &=~(0X03<<0); //��ͨ��������ģʽ
	GPIOA->PUPDR &=~(0X03<<0); //��������
	
	
		//�˿�ʱ��ʹ��PE
	RCC->AHB1ENR |=0X01<<4;
	//�˿�����PE 2 3 4����Ϊ��ͨ���ܸ�������
	GPIOE->MODER &=~(0X03<<4 | 0X03<<6 |0X03<<8); //��ͨ��������ģʽ
	GPIOE->PUPDR &=~(0X03<<4 | 0X03<<6 |0X03<<8);  //��������
	
	
}

void key_delay(void)
{
	u32 i=0x30000;
	while(i--);
}


//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0xFF��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY4����
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2>KEY3>KEY4!!
u8 key_scanf(u8 mode)
{
	static u8  key_sta=1;    //key_sta=1,����ʶ�𰴼�
	static u8 key_time=0;

	static u8 key1,key2,key3;
	u8 key=NO_KEY;
	
	if(mode)
	{
		key_sta=1;  //�����ɿ���־, ֧������
	}
	
	key3=key2;
	key2=key1;	
	key_time++;
	
	if((key_sta==1)&& ((KEY1==1) || (KEY2==0) || (KEY3==0) || (KEY4==0)))  //����ʶ�𰴼��������п����ǰ���������
	{
		//key_delay(); //��ʱ����
		if(KEY1)   //�ٴ��ж��Ƿ�Ϊ�ߵ�ƽ
		{
			key1=KEY1_OK;   //ʶ��߰�������
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
		
		
		if(key_time==3)   //���ʶ������
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
				key_sta=0;     //������ʶ�𰴼�
			}
			
		}
	}
	else if((0==KEY1) &&(KEY2==1) &&(KEY3==1) &&(KEY4==1) )   //û�а�������
	{
		key_sta=1;     //����ʶ�𰴼�
		key_time=0;
	}
	return key;
}


