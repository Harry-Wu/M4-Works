/*****************************************************************************
@File name:  
@Description: ��ǰһ����ϰ�����ϣ������keyscanfȥ��ⰴ��(�жϷ�ʽ��ⰴ�����׵��¶���ж�)
@Author: Harry Wu
@Version: V1.0
@Date: 
@History: 
*****************************************************************************/

#include "stm32f4xx.h" 
#include "key.h" 
#include "led.h"
#include "io_bit.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"
#include "exti.h"
#include "MyTypeDef.h"


//u8 buf[100];
int main(void)
{
	//u8 i=0,j=100;
	//Ӳ����ʼ��
	NVIC_SetPriorityGrouping(7-2);//���÷���
	led_init();
	key_init();
	uart1_init(9600);
	exti_init();
	exti2_init();
	exti9_soft_init();
	while(1)
	{
		delay_ms(100);
		EXTI->SWIER |= 0x01<<9; //����ж��¼��Ĵ�����1
	}
	
}


