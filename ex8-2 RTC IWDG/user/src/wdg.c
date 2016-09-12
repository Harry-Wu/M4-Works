#include "stm32f4xx.h"

void iwdg_init(u8 pr, u16 rlr)
{
	IWDG->KR = 0x5555;  //�޸����������Ĵ���
	//while(IWDG->SR & 0X03);
	while((IWDG->SR & 0x01)==1);
	IWDG->PR = pr;
	while((IWDG->SR & 0x02)==2);
	IWDG->RLR = rlr;
	IWDG->KR = 0XAAAA;  //��װ�ؼ�����
	//while(IWDG->SR & 0X03);
	IWDG->KR = 0XCCCC;  //�������Ź�
	
	
}
