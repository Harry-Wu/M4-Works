#include "stm32f4xx.h" 
#include "key.h" 
#include "led.h"
#include "io_bit.h"
#include "delay.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "at24c02.h"
#include "flash.h"

u8 *p="hello world\r\n";

u8 buf[100];
int main(void)
{
	//u8 i=0,j=100;
	//u8 _data,key;
	//硬件初始化
	NVIC_SetPriorityGrouping(7-2);//设置分组
	led_init( );
	key_init();
	uart1_init(115200);
	at24c04_init();
	SPI_Flash_Init();
	SPI_Flash_Write(p,0x100,strlen(p)+1);
	SPI_Flash_Read(buf,0x100,strlen(p)+1); 
	
	while(1)
	{
		printf("0x%x\r\n",SPI_FLASH_TYPE);
		printf("%s",buf);
		delay_ms(500);
		
	}

	
}


