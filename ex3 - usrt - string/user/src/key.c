/* KEY1--PA0, KEY2--PE2, KEY3--PE3, KEY4--PE4 */

#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"

void key_init(void)
{	
	//�˿�ʱ��ʹ��PA
	RCC->AHB1ENR |= 0x01<<0;
	//�˿�����, PA0����Ϊ��ͨ����,��������
	GPIOA->MODER &=~(0x03<<0);//��ͨ����
	GPIOA->PUPDR &=~(0x03<<0);//��������
	
	//�˿�ʱ��ʹ��PE
	RCC->AHB1ENR |= 0x01<<4;
	//�˿�����, PE2,3,4����Ϊ��ͨ����,��������
	GPIOE->MODER &=~(0x03<<4 | 0x03<<6 | 0x03<<8);//��ͨ����
	GPIOE->PUPDR &=~(0x03<<4 | 0x03<<6 | 0x03<<8);//��������
}

u8 key_scanf(void)
{
	static u8 key_sta = 1;  //key_sta=1,����ʶ�𰴼�. ����һֱ���ŵ�ʱ��,������ʶ��
	static u8 key_time = 0;
	static u8 key1,key2,key3;
	u8 key = NO_KEY;
	
	key3 = key2;
	key2 = key1;
	key_time++;
	
	if((key_sta==1)&& ((KEY1==1) || (KEY2==0) || (KEY3==0) || (KEY4==0)))  //���ʶ��Ϊ�ߵ�ƽ,��������ʶ�𰴼�
	{
		//delay_ms(20);  //��ʱ����
		if(KEY1)  //�ٴ��ж��Ƿ�Ϊ�ߵ�ƽ
		{
			key1 = KEY1_OK; //ʶ��Ϊ��,���������ж��Ƿ�����а�������,��ʱ��ֵ����key1
			//key_sta = 0; //������ʶ�𰴼�
			//while (KEY1); //�жϰ����Ƿ��ͷ�
		}
		else if(!KEY2)
		{
			key1 = KEY2_OK;
		}
		else if(!KEY3)
		{
			key1 = KEY3_OK;
		}
		else if(!KEY4)
		{
			key1 = KEY4_OK;
		}
		
		if(key_time==3)
		{
			key_time = 0;
			if((key3==key1) && (key2==key1))
			{
				key = key1;   //����ȷ���а���������,�����յ�ֵ����key
				key_sta = 0;  //������ʶ�𰴼�
			}
		}
	}
	else if((KEY1==0) && (KEY2==1) && (KEY3==1) && (KEY4==1)) //�͵�ƽ,û���κΰ�������
		{
			key_sta = 1; //����ʶ�𰴼�
			key_time = 0; //ʶ��һ�ΰ������º�,��ʼ��key_time��ֵ
		}
	return key;
}
