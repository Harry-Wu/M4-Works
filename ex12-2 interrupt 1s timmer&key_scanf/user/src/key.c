#include "stm32f4xx.h"
#include "key.h"

void key_init(void)
{
	//�˿�ʱ��ʹ��PA PE
	RCC->AHB1ENR |= 0X01<<0 | 0X01<<4;
	//�ܽ�����
	GPIOA->MODER &=~(0X03<<0);  //pa0��ͨ��������
	GPIOA->PUPDR &=~(0X03<<0);  //��������	
	
	GPIOE->MODER &=~(0X03<<4 | 0X03<<6 | 0X03<<8);  //pE 2 3 4��ͨ��������
	GPIOE->PUPDR &=~(0X03<<4 | 0X03<<6 | 0X03<<8);   //��������	
	
	
}

void key_delay(void)
{
	u32 i=0x100;
	while(i--);
}


u8 key_scanf(void)
{	
	u8 key=NO_KEY;
	static u8 key_sta=1;   //=1�����Ǳ�����������
	static u8 key1,key2,key3,time=0;
	key3=key2;
	key2=key1;
	time++;
	if((key_sta==1)&& ((KEY1==1) || (KEY2==0)  || (KEY3==0) || (KEY4==0)))//�ж��Ƿ�����ʶ�𰴼� ���� �Ƿ��а�������
	{
		//key_delay();//��ʱ����
		//�ٴ��ж��Ƿ�Ϊ��Ч��ƽ
		if(KEY1)  //������ȷ�Ǳ�������
		{
			key1=KEY1_OK;  //ʶ�𵽰�������
		}
		else if(0==KEY2)
		{
			key1=KEY2_OK;  //ʶ�𵽰�������
		}
		else if(0==KEY3)
		{
			key1=KEY3_OK;  //ʶ�𵽰�������
		}
		else if(0==KEY4)
		{
			key1=KEY4_OK;  //ʶ�𵽰�������
		}
		if(time==3)
		{
			time=0;
			if((key1==key2) && (key1==key3))
			{
				key=key1;
				key_sta=0;  //������ʶ�𰴼�
			}
		}
	}
	else if((0==KEY1)&&(1==KEY2)&&(1==KEY3)&&(1==KEY4))//û�а���������
	{
		key_sta=1;  //����ʶ�𰴼�
		time=0;
	}
	return key;
	
}


