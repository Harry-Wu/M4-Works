#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "string.h"
#include "delay.h"
#include "at24c02.h"
#include "stdio.h"
#include "flash.h"
#include "lcd.h"
#include "adc.h"
#include "DMA.h"
#include "string.h"
#include "time.h"
char buf[100];

int main(void)
{
	u8 num=0,num_old=0,tranf_start=0;
	u8 key_r=NO_KEY;
	NVIC_SetPriorityGrouping(7-2);
	led_init( );    //led��ʼ��
	key_init( );
	uart_init(115200);
	at24c02_init();
	SPI_Flash_Init( );

	lcd_init();
	lcd_clear(BACK_COLOR);
	time6_init();
	time7_init();
	time12_pwm_init();
	while(1)
	{
		if(key!=NO_KEY)//r����а�������
		{
			key_r=key; //��key_r��ʱ����key��ֵ��Ȼ��������ϰ�keyֵ��ʼ��ΪNO_KEY
			key=NO_KEY;
			if(key_r==KEY1_OK)
			{
				LED2=!LED2;
			}
			else if(key_r==KEY2_OK)
			{
				if(TIM12->CCR2<900)
				{
					TIM12->CCR2 += 200;
				}
				else
				{
					TIM12->CCR2=200;
				}
			}
		}
		
		
	}
	
	
}

