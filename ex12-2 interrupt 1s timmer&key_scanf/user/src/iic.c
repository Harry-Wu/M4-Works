#include "stm32f4xx.h"
#include "iic.h"
#include "delay.h"
void iic_init(void)
{
	//端口时钟使能
	RCC->AHB1ENR |=0X01<<1;
	//scl(pb8) sda(pb9)普通功能开漏输出，不用上下拉
	GPIOB->MODER &=~(0x03<<16 | 0x03<<18);
	GPIOB->MODER |=0x01<<16 | 0x01<<18;
	GPIOB->OTYPER |=0X03<<8;
	GPIOB->OSPEEDR &=~(0x03<<16 | 0x03<<18);
	GPIOB->OSPEEDR |=0X01<<16 |0X01<<18;  //输出速度25M
	GPIOB->PUPDR  &=~(0x03<<16 | 0x03<<18);
	
	IIC_SDAOUT=1;
	IIC_SCL=1;
}

//主机发送起始条件
//主机占用总线，唤醒所有从设备
//SCL为高电平期间，SDA有一个下降沿
void iic_start(void)
{
	IIC_SCL=0;
	IIC_SDAOUT=1;
	IIC_SCL=1;
	delay_us(1);
	IIC_SDAOUT=0;   //sda下降沿出现在这句话后
	delay_us(1);
	IIC_SCL=0;	  //方便接下来操作
}	

//主机发送停止条件
//停止通信、释放总线
//在SCL为高电平期间，SDA有一个上升沿
void iic_stop(void)
{
	IIC_SCL=0;
	IIC_SDAOUT=0;
	IIC_SCL=1;
	delay_us(1);
	IIC_SDAOUT=1;	
	delay_us(2);
}
	

void iic_write_byte(u8 _data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		if(_data & 0x80)	IIC_SDAOUT=1;
		else  IIC_SDAOUT=0;
		delay_us(2);
		IIC_SCL=1;
		_data=_data<<1;
		delay_us(1);
	}
	IIC_SCL=0;
}

u8 iic_read_byte(void)
{
	u8 i,rec_data;
	IIC_SCL=0;
	IIC_SDAOUT=1;   //切换为读模式
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		delay_us(2);
		IIC_SCL=1;
		rec_data=rec_data<<1;
		if(IIC_SDAIN)
		{
			rec_data=rec_data |0x01;
		}
		delay_us(1);
	}
	IIC_SCL=0;
	return rec_data;
}	

//当主机接收到一个字节数据后，给从机一个响应
//ack=0,给响应
void iic_put_ack(u8 ack)
{
	IIC_SCL=0;
	if(ack)		IIC_SDAOUT=1;
	else	IIC_SDAOUT=0;   //给响应
	delay_us(2);
	IIC_SCL=1;
	delay_us(1);
	IIC_SCL=0;
}	
//当主机发送一个字节
//数据给从机后需要判断从机是否有
//响应
//返回值： 返回0 有得到响应
u8  iic_get_ack(void)
{
	u8 ack=0;
	
	IIC_SCL=0;
	IIC_SDAOUT=1;  //切换为读模式
	delay_us(2);
	IIC_SCL=1;
	if(IIC_SDAIN)
	{
		ack=1;
	}
	delay_us(1);
	IIC_SCL=0;
	return ack;
}






