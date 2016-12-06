#include "stm32f4xx.h" 
#include "iic.h"

static void iic_delay(void)
{
	u32 i=100;
	while(i--);
}



void iic_init(void)
{
	//端口时钟使能PB
	RCC->AHB1ENR |=0X01<<1;
	//端口设置PB8(SCL)  PB9(SDA)----普通功能开漏输出无上下拉
	GPIOB->MODER &=~((0X3<<16)| (0X3<<18));
	GPIOB->MODER |=((0X1<<16)| (0X1<<18)) ;  //普通功能输出
	GPIOB->OTYPER |=((0X01<<8)| (0X01<<9));   //开漏输出
	GPIOB->OSPEEDR &=~((0X3<<16)| (0X3<<18));  //输出速度2M
	GPIOB->PUPDR &=~((0X3<<16)| (0X3<<18));    //无上/下拉
	
	//让总线处于空闲状态
	IIC_SCL=1;
	IIC_SDA_OUT=1;
}


//起始条件
void iic_start(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT=1;
	IIC_SCL=1;
	iic_delay( );
	IIC_SDA_OUT=0;
	iic_delay( );
	IIC_SCL=0;
}
//停止条件
void iic_stop(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT=0;
	IIC_SCL=1;
	iic_delay( );
	IIC_SDA_OUT=1;
	iic_delay( );
	iic_delay( );
}

//写一个字节
void iic_write_byte(u8 send_data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		if(send_data & 0x80)
		{
			IIC_SDA_OUT=1;
		}
		else 
		{
			IIC_SDA_OUT=0;
		}
		send_data=send_data<<1;
		iic_delay( );
		iic_delay( );
		IIC_SCL=1;
		iic_delay( );
	}
	IIC_SCL=0;
}


//读取一个字节
//返回值：读取到的一个字节数据
u8 iic_read_byte(void)
{
	u8 i,rec_data;
	//切换为读模式
	IIC_SCL=0;
	IIC_SDA_OUT=1;
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		iic_delay( );
		iic_delay( );
		IIC_SCL=1;
		rec_data =rec_data<<1;
		if(IIC_SDA_IN)
		{
			rec_data |=1;
		}
		iic_delay( );
	}
		IIC_SCL=0;
	return rec_data;
}
//读应答
//当主机发送一个字节数据后，从机会给应答
//主机这个时候就要读取应答，判断从机是否正常
//接收到了数据
//返回值： 1：没有应答；0：有应答
u8 iic_get_ack(void)
{
	u8 ack;
	//切换为读模式
	IIC_SCL=0;
	IIC_SDA_OUT=1;
	iic_delay( );
	iic_delay( );
	IIC_SCL=1;
	if(IIC_SDA_IN)
	{
		ack=1;
	}
	else
	{
		ack=0;
	}
	iic_delay( );
	IIC_SCL=0;
	return ack;
}
	
//发送响应
//当主机接收到从机发送过来的一个字节数据后
//会给或不给从机一个响应
//参数： ack=0 给响应；ack=1不给响应
void iic_put_ack(u8 ack)
{
	IIC_SCL=0;
	if(ack)
	{
		IIC_SDA_OUT=1;   //不给响应
	}
	else
	{
		IIC_SDA_OUT=0;  //给响应
	}
	iic_delay( );
	iic_delay( );
	IIC_SCL=1;
	iic_delay( );
	IIC_SCL=0;
}




