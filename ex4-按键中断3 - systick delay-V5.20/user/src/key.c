#include "stm32f4xx.h" 
#include "key.h"

void key_init(void)
{
	//�˿�ʱ��ʹ��PA0
	RCC->AHB1ENR |=0X01<<0;
	//���ùܽ�PA0---��ͨ���ܸ�������
	GPIOA->MODER &=~(0X03<<0);  //PA0 ��ͨ��������
	GPIOA->PUPDR &=~(0X03<<0);  //PA0��������
	
	
	//�˿�ʱ��ʹ��PE
	RCC->AHB1ENR |=0X01<<4;
	//���ùܽ�Pe2 3 4---��ͨ���ܸ�������
	GPIOE->MODER &=~(0X03F<<4);
	GPIOE->PUPDR &=~(0X03F<<4);
}


uint8_t  key_scanf(void)
{
	static uint8_t key_sta=1;  //key_sta=1������ʶ�𰴼�
	static uint8_t key1=NO_KEY,key2,key3,time=0;
	unsigned char key=NO_KEY;
	key3=key2;
	key2=key1;
  time++;
	if((key_sta==1)&&((KEY1!=0)||(KEY2==0)||(KEY3==0)||(KEY4==0))) //,����ʶ�𰴼���������һ����������
	{
		//Delay5ms();  //��ʱ����
		if(KEY1!=0)   //�ٴ��ж��Ƿ��а�������
		{
			key1=KEY1_OK;   //�������´���
			//while(p14==0);  //�ȴ������ͷ�
			//key_sta=0;   //������ʶ�𰴼�
		}
		else if(KEY2==0)   //�ٴ��ж��Ƿ��а�������
		{
			key1=KEY2_OK;   //�������´���
			//while(p14==0);  //�ȴ������ͷ�
			//key_sta=0;   //������ʶ�𰴼�
		}
			else if(KEY3==0)   //�ٴ��ж��Ƿ��а�������
		{
			key1=KEY3_OK;   //�������´���
			//while(p14==0);  //�ȴ������ͷ�
			//key_sta=0;   //������ʶ�𰴼�
		}
			else if(KEY4==0)   //�ٴ��ж��Ƿ��а�������
		{
			key1=KEY4_OK;   //�������´���
			//while(p14==0);  //�ȴ������ͷ�
			//key_sta=0;   //������ʶ�𰴼�
		}
		if(time==3)
		{
			time=0;
			if(key1==key2 && key1==key3 && key1!=NO_KEY)
			{
				key=key1;
				key_sta=0;   //������ʶ�𰴼�
			}
		}
	}
	else if((KEY1==0)&&(KEY2!=0)&&(KEY3!=0)&&(KEY4!=0))  //���а������ͷ�
	{
			key_sta=1;   //����ʶ�𰴼�
			time=0;
	}
	return key;
}




