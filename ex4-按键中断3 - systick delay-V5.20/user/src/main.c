#include "stm32f4xx.h"
#include "sys.h"
#include "key.h" 
#include "led.h"
//#include "io_bit.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"
#include "exti.h"


//uint8_t buf[100];
int main(void)
{	
	Stm32_Clock_Init(336,8,2,7);//����ʱ��,168Mhz
	
	//uint8_t i=0,j=100;
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
		delay_ms(500);
		EXTI->SWIER |= 0x01<<9; //����ж��¼��Ĵ�����1
	}
	
}

