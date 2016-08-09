#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"

void key_init(void)
{
	//�˿�ʱ��ʹ��
	RCC->AHB1ENR |= 0x01<<0;
	//�˿�����, PA0����Ϊ��ͨ����,��������
	GPIOA->MODER &=~(0x03<<0);//��ͨ����
	GPIOA->PUPDR &=~(0x03<<0);//��������
}

u8 key_scanf(void)
{
	static u8 key_sta = 1;  //key_sta=1,����ʶ�𰴼�. ����һֱ���ŵ�ʱ��,������ʶ��
	u8 key = NO_KEY;
	if((key_sta==1)&& KEY1)  //���ʶ��Ϊ�ߵ�ƽ,��������ʶ�𰴼�
	{
		delay_ms(20);  //��ʱ����
		if(KEY1)  //�ٴ��ж��Ƿ�Ϊ�ߵ�ƽ
		{
			key = KEY1_OK; //ʶ��Ϊ��,��������
			key_sta = 0; //������ʶ�𰴼�
			//while (KEY1); //�жϰ����Ƿ��ͷ�
		}		
	}
	else if(!KEY1) //�͵�ƽ,û�а�������
		{
			key_sta = 1; //����ʶ�𰴼�
		}
	return key;
}
