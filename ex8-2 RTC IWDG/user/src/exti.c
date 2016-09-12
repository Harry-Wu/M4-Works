#include "stm32f4xx.h" 
#include "LED.h" 

void exti0_init(void)
{
	//u8 prior;
	//对于IO口配置为输入
	
	//系统配置控制器时钟使能 
	RCC->APB2ENR |=0X01<<14;
	
	SYSCFG->EXTICR[0] &=~(0X0F<<0);
	
	
	
	EXTI->IMR |=0X01<<0;   //使能exti0中断
	EXTI->EMR &=~(0X01<<0);  //屏蔽exti0事件请求
	EXTI->RTSR |=(0X01<<0); //允许 exti0上升沿触发
	EXTI->FTSR &=~(0X01<<0);     // 禁止exti0下降沿触发
	
	//prior=NVIC_EncodePriority(7-2,1,1);  //计算优先级
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(7-2,1,1));  //设置优先级
	NVIC_EnableIRQ(EXTI0_IRQn); //外部中断使能（系统中断没有这个使能）
	
}

void exti2_init(void)
{
	//u8 prior;
	//对于IO口配置为输入
	
	//系统配置控制器时钟使能 
	RCC->APB2ENR |=0X01<<14;
	
	SYSCFG->EXTICR[0] &=~(0X0F<<8);
	SYSCFG->EXTICR[0] |=0X04<<8;    //exti2映射到PE2
	
	
	EXTI->IMR |=0X01<<2;   //使能exti2中断
	EXTI->EMR &=~(0X01<<2);  //屏蔽exti2事件请求
	EXTI->RTSR &=~(0X01<<2); //禁止 exti2上升沿触发
	EXTI->FTSR |=0X01<<2;     // 允许exti2下降沿触发
	
	//prior=NVIC_EncodePriority(7-2,1,1);  //计算优先级
	NVIC_SetPriority(EXTI2_IRQn,NVIC_EncodePriority(7-2,1,1));  //设置优先级
	NVIC_EnableIRQ(EXTI2_IRQn); //外部中断使能（系统中断没有这个使能）
	
}


void EXTI0_IRQHandler(void)
{
	EXTI->PR |=0X01<<0;  //清挂起标志
	LED2=!LED2;
}



void EXTI2_IRQHandler(void)
{
	EXTI->PR |=0X01<<2;  //清挂起标志
	LED1=!LED1;
}

