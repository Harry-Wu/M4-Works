#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"
#include "io_bit.h"
#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"


//#define LS1 PFout(8)



int main(void)
{
	u8 buf[100];
	
	//Ӳ����ʼ��
	NVIC_SetPriorityGrouping(7-2);//���÷��� 
	led_init();
	key_init();
	uart1_init(115200);
	
	while(1)
	{
//		//�ȴ����յ�����
//		while(!(USART1->SR & (0x01<<5)));
//		//��������
//		rec_data = USART1->DR;
//		//�ȴ��ϴη���
//		while(!(USART1->SR & (0x01<<7)));
//		//���յ������ݻط�
//		USART1->DR = rec_data;
		
		if(rec_sta==1)
		{
			if(0==strcmp((const char *)rec_buf, "open"))
			{
				D5 = 0;
				
			}
			else if(0==strcmp((const char *)rec_buf, "close"))
			{
				D5 = 1;
			}
			rec_sta = 0;
		}
								
	}	
}


