/*****************************************************************************
@File name:  
@Description: 显示RTC时间,按键设置时间或闹钟 
@Author: Harry Wu
@Version: V1.5
@Date: 2016-10-03
@History: 
		V1.0: 基于ex9-2, 添加闹钟功能和weak up中断功能
		V1.1: 增加亮度数值显示功能.
		V1.2: 加入定时器中断功能, 定时1s闪烁D6. 下一步, 把按键长按短按功能加进去, 使用定时器计数
		V1.3: 加入长按短按识别功能,为了保证显示刷新,在按键按下时, 插入显示函数
		V1.4: 在时间修改模式下,相应区段的字符背景色变为绿色. 左上角触摸按键加入状态字符.
		V1.5: 修复设置时数值超出范围的问题; 每次上电不再初始化闹钟设置,同时间设置, 只设置一次; 优化扫描模式按键函数,假如1ms延时,不再一次短按相应多次. 

		
*****************************************************************************/

#include "stm32f4xx.h" 
//#include "sys.h"
#include "key.h"
#include "io_bit.h"
#include "led.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "at24c02.h"
#include "delay.h"
#include "flash.h"
#include "lcd.h"
#include "rtc.h"
#include "touch.h"
#include "led.h"
#include "adc.h"
#include "timer.h"
#include "clock.h"
#include "dma.h"


char *p="hello world1234567890\r\n";
u8 buf[100];
extern u8 write_start_flag;
extern u8 time_out;
int main(void)
{
	_TOUCH_CSYS_TYPEDEF touch_addr;

	
	//Stm32_Clock_Init(336, 8, 2, 7);  //系统时钟186MHz
	
	//u8 key_value;
	//u8 tbuf[40];

	NVIC_SetPriorityGrouping(7-2);//设置分组
	at24c02_init( );
	//硬件初始化
	led_init( );
	
	key_init();
	uart1_init(115200);
	SPI_Flash_Init();
	lcd_init();
	
	
	
	touch_init();
	adc1_init();
	
	dma_init((u32)&USART1->DR);
	if(KEY1==1)
	{
		LCD_ShowString(10, 20, "Erasing flash...", 0);
		printf("Erasing flash...\r\n");
		SPI_Flash_Erase_Chip();
		LCD_ShowString(10, 40, "Erased, please send font", 0);
		printf("Erased, please send font\r\n");
		while(write_start_flag==0);  //等待上位机发数据
		LCD_ShowString(10, 60, "font writing, please wait...", 0);
		printf("font writing, please wait...\r\n");
		while(1)
		{
			
			delay_ms(1);
			time_out++;
			if(time_out>100)
			{
				break;  //没接收到数据了,跳出while(1)循环
			}
		}
		LCD_ShowString(10, 80, "font writing done!", 0);
		printf("font writing done!\r\n");
	}
	
	//RTC对数据写入flash有干扰, "一定要放到写字库后面"
	RTC_Init();
	RTC_Set_WakeUp(4,0);
	timer14_init(5000-1, 8400-1); //Tout=8400/84000000*5000=0.5s
	timer13_config(10-1, 8400-1);  //1ms

	LCD_ShowString(10,120, "欢迎回来, 一切OK!", 0); 
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	display_init();
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	
	while(1)
	{
		display_time();
	
		touch_scanf(&touch_addr, 0);
		if(touch_addr.x>10 &&touch_addr.x<50 &&touch_addr.y>10 &&touch_addr.y<50 )
		{
			LED5=!LED5;
			if(LED5)  //如果灯是灭的
			{
				LCD_DrawSolidRectangle(10, 10,50,50,RED);
				BACK_COLOR = RED;
				LCD_ShowString(22, 22, "关",0);
				BACK_COLOR = BLACK;
			}
			else
			{
				LCD_DrawSolidRectangle(10, 10,50,50,GREEN);
				BACK_COLOR = GREEN;
				LCD_ShowString(22, 22, "开",0);
				BACK_COLOR = BLACK;
			}
		}

		if(touch_addr.x>60 &&touch_addr.x<100 &&touch_addr.y>10 &&touch_addr.y<50 )
		{
			dma2_stream7_tranf((u32)p,strlen(p));
			LED5=!LED5;
			if(LED5)  //如果灯是灭的
			{
				LCD_DrawSolidRectangle(60, 10,100,50,RED);
				BACK_COLOR = RED;
				LCD_ShowString(22+50, 22, "发",0);
				BACK_COLOR = BLACK;
			}
			else
			{
				LCD_DrawSolidRectangle(60, 10,100,50,GREEN);
				BACK_COLOR = GREEN;
				LCD_ShowString(12+50, 22, "再发",0);
				BACK_COLOR = BLACK;
			}
		}

		key_service();
//		key_value = keyscanf_longshort();
//		switch(key_value)
//		{
//			case 0:
//				LCD_ShowString(20,60, "NO KEY PRESSED",0);
//				break;
//			case 1:
//				LCD_ShowString(20,80, "SHORT PRESS",0);
//				break;
//			case 2:
//				LCD_ShowString(20,100, "LONG PRESS",0);
//				break;
//		}		

//		CNV_touch2lcd(&touch_add);
//		if(touch_add.x!=0xffff)
//		{
//			LCD_DrawPoint(touch_add.x,touch_add.y);
//			LCD_DrawPoint(touch_add.x+1,touch_add.y);
//			LCD_DrawPoint(touch_add.x,touch_add.y+1);
//			LCD_DrawPoint(touch_add.x+1,touch_add.y+1);
//		}
		

		
//		get_touch_ad_filled(&touch_addr);
//		printf("x = %4d y = %4d\r\n", touch_addr.x, touch_addr.y);
		//delay_ms(100);

//		printf("0x%x\r\n",SPI_FLASH_TYPE);
//		delay_ms(500);
//		printf("HELLO\r\n");
//		delay_ms(500);
	}
}

