#include "stm32f4xx.h"

void delay_ms(int t)
{
	int i = 0;
	//u32 j = 0x992;
	for(i=t;i>0; i--)
	{
		u32 j = 0x992;  //—” ±1ms
		while (j--);
	}
}
