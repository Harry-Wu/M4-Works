#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"
#include "io_bit.h"
#include "led.h"
#include "uart.h"

//#define LS1 PFout(8)



int main(void)
{
	u8 rec_data;
	
	//Ӳ����ʼ��
	led_init();
	key_init();
	uart1_init(115200);
	
	while(1)
	{
		//�ȴ����յ�����
		while(!(USART1->SR & (0x01<<5)));
		//��������
		rec_data = USART1->DR;
		//�ȴ��ϴη���
		while(!(USART1->SR & (0x01<<7)));
		//���յ������ݻط�
		USART1->DR = rec_data;
								
	}	
}


