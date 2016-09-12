#include "stm32f4xx.h" 
#include "IIC.h" 

void iic_init(void)
{
	//端口时钟使能PB
	RCC->AHB1ENR |=0X01<<1;
	//pb8(SCL)(普通功能浮空开漏输出) 
  //pb9(SDA)(普通功能浮空开漏输出) 
	GPIOB->MODER &=~(0X03<<16 | 0X03<<18); 
	GPIOB->MODER |=0x01<<16 | 0x01<<18;//普通功能输出
	GPIOB->OTYPER |=0x01<<8 |0x01<<9;  //开漏输出
	GPIOB->OSPEEDR &=~(0X03<<16 | 0X03<<18); 
	GPIOB->OSPEEDR|=0x01<<16 | 0x01<<18;    //输出速度25M
	GPIOB->PUPDR &=~(0X03<<16 | 0X03<<18);   //浮空
	
	SDAOUT=1;
	SCL=1;
	
}


void iic_delay(void)
{
	u32 i=100;
	while(i--);
}



//起始条件
void iic_start(void)
{
	SCL=0;       
	SDAOUT=1;
	SCL=1;
	iic_delay();
	SDAOUT=0;
	iic_delay();
	SCL=0;
}
//停止条件
void iic_stop(void)
{
	SCL=0;
	SDAOUT=0;
	SCL=1;
	iic_delay();
	SDAOUT=1;
	iic_delay();
	iic_delay();
}
//写一个字节
//参数send_data:待发送的数据
void iic_write_byte(u8 send_data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		SCL=0;
		if(send_data&0x80)
		{
			SDAOUT=1;
		}
		else
		{
			SDAOUT=0;
		}
		iic_delay();
		iic_delay();
		send_data=send_data<<1;
		SCL=1;
		iic_delay();
	
	}
	SCL=0;
}


//读取一个字节数据
//返回值为读取到的数据
u8 iic_read_byte(void)
{
	u8 read_data,i;
	SCL=0;
	SDAOUT=1;   //让输出电路跟管脚断开，切换为读模式
	
	for(i=0;i<8;i++)
	{
		SCL=0;
		iic_delay();
		iic_delay();
		SCL=1;
		read_data =read_data<<1;
		if(SDAIN)
		{
			read_data |= 0x01;
		}
		iic_delay();
	}
		SCL=0;
	return read_data;
}


//读响应
//返回值： 1：从机无响应 0：从机有响应
u8 iic_get_ack(void)
{
	u8 ack;
	SCL=0;
	SDAOUT=1;    //让输出电路跟管脚断开，切换为读模式
	iic_delay();
	iic_delay();
	SCL=1;
	if(SDAIN)
	{
		ack=1;
	}
	else
	{
		ack=0;
	}
	iic_delay();
	SCL=0;
	
	return ack;
	
}

//写响应
//参数：ACK=1，不给响应；ACK=0，给响应
void iic_put_ack(u8 ack)
{
	SCL=0;
	if(ack)
	{
		SDAOUT=1;
	}
	else
	{
		SDAOUT=0;
	}
	iic_delay();
	iic_delay();
	SCL=1;
	iic_delay();
	SCL=0;
}
