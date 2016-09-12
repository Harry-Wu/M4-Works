#include "stm32f4xx.h" 
#include "KEY.h" 
//�����˿ڳ�ʼ��
void key_init(void)
{
	//ʱ��ʹ��Pa Pe
	RCC->AHB1ENR |= 0X01<<4 | 0X01<<0 ;
	//�˿ڳ�ʼ��  pe2/3/4 --��ͨ���ܸ�������
	GPIOE->MODER &=~(0X03<<4 |0X03<<6 |0X03<<8);  //pe2/3/4Ϊ����ģʽ
	GPIOE->PUPDR &=~(0X03<<4 |0X03<<6 |0X03<<8);  //��������
	
	//�˿ڳ�ʼ��  pA0 --��ͨ���ܸ�������
	GPIOA->MODER &=~(0X03<<0);  //pA0Ϊ����ģʽ
	GPIOA->PUPDR &=~(0X03<<0);  //��������
	
}


void key_delay(void)
{
	u32 i=0x5000;
	while(i--);
}


//����ʶ��
//����ֵ�� ʶ�𵽵İ����ı�־
u8 key_scanf(void)
{
	static u8 key_sta=1;   //key_sta=1����ʶ�𰴼���key_sta=0������ʶ�𰴼�
	static u8 key1,key2,key3,num=0;
	u8 key=NO_KEY;
	
	key3=key2;
	key2=key1;
	num++;
	if((key_sta==1) &&((KEY2==0) ||(KEY1==0)  ||(KEY0==0) ||(WKUP_KEY==1) ))  //����ʶ�𰴼��������а�������
	{
	//	key_delay();  //��ʱ����
		if(KEY2==0)  //�ٴ��ж��Ƿ��а�������
		{
			//key_sta=0;     //������ʶ�𰴼�
			key1=KEY2_OK;
		}
		else if(KEY1==0)  //�ٴ��ж��Ƿ��а�������
		{
			//key_sta=0;     //������ʶ�𰴼�
			key1=KEY1_OK;
		}
		else if(KEY0==0)  //�ٴ��ж��Ƿ��а�������
		{
			//key_sta=0;     //������ʶ�𰴼�
			key1=KEY0_OK;
		}
			else if(WKUP_KEY==1)  //�ٴ��ж��Ƿ��а�������
		{
			//key_sta=0;     //������ʶ�𰴼�
			key1=WKUP_KEY_OK;
		}
		//while((KEY2==0) ||(KEY1==0) ||(KEY0==0) ||(WKUP_KEY==1));//�ȴ������ͷ�
	if(num==3)
	{
		num=0;
		if((key1==key3)&&(key1==key2))
		{
			key=key1;
			key_sta=0;     //������ʶ�𰴼�(״̬��)
		}
	}
	
	}
	else if((KEY2==1) &&(KEY1==1) &&(KEY0==1) &&(WKUP_KEY==0)) //���еİ������ͷ���
	{
		key_sta=1;     //�����´�ʶ�𰴼�
		num=0;
	}
	
	return key;
	
}
	




