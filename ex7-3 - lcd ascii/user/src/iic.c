#include "stm32f4xx.h" 
#include "iic.h"
#include "MyTypeDef.h"

void iic_init(void)
{
	//端口时钟使能pb
	RCC->AHB1ENR |=0X01<<1;
	//端口模式设置
	//SCL(pb8) SDA(pb9)-----普通功能开漏输出
	GPIOB->MODER &=~(0X03<<16 | 0X03<<18);
	GPIOB->MODER |=0X01<<16 |0X01<<18; //普通功能输出
	GPIOB->OTYPER |=0X01<<8 | 0X01<<9;  //开漏输出
	GPIOB->OSPEEDR &=~(0X03<<16 | 0X03<<18);//输出速度2M
	GPIOB->PUPDR &=~(0X03<<16 | 0X03<<18); //既不上拉也不下拉
		
	//端口初始状态-----让总线处于空闲状态
	IIC_SDA_OUT=1;
	IIC_SCL=1;
}

static void iic_delay(void)
{
	u32 i=50;
	while(i--);
}

//起始条件
void iic_start(void)
{
	IIC_SCL=0;	
	IIC_SDA_OUT=1;	
	IIC_SCL=1;
	iic_delay();  // >0.6us  tSU.STA
	IIC_SDA_OUT=0;
	iic_delay(); // >0.6us  tHD.STA
	IIC_SCL=0;
}

//停止条件
void iic_stop(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT=0;
	IIC_SCL=1;
	iic_delay();		//>100ns tSU.STO
	IIC_SDA_OUT=1; 
	iic_delay();
	iic_delay();	  //tBUF
}


//发送一个字节数据
void iic_write_byte(u8 _data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		                 //tHD.DAT=0
		if(_data & 0x80)
		{
			IIC_SDA_OUT=1;
		}
		else
		{
			IIC_SDA_OUT=0;
		}
		iic_delay();
		iic_delay();    //tLOW
		IIC_SCL=1;
		_data=_data<<1;
		iic_delay();    //tHIGH
	}
		IIC_SCL=0;
}

//接收一个字节数据
u8 iic_read_byte(void)
{
	u8 i,rec_data;
	//切换为读模式
	IIC_SCL=0;
	IIC_SDA_OUT=1;	
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		iic_delay();iic_delay();    //tLOW
		IIC_SCL=1;
		rec_data=rec_data<<1;
		if(IIC_SDA_IN)
		{
			rec_data =rec_data | 0x01;
		}
		iic_delay();    //tHIGH
	}
	IIC_SCL=0;
	return rec_data;
}


//发送响应
//ack :   ack=0（给响应）
void iic_put_ack(u8 ack)
{
	IIC_SCL=0;
	if(ack)
	{
		IIC_SDA_OUT=1;    //不给响应
	}
	else
	{
		IIC_SDA_OUT=0;    //给响应
	}
	iic_delay();iic_delay();    //tLOW
	IIC_SCL=1;
	iic_delay();    //tHIGH
	IIC_SCL=0;
}
	
//接收响应
u8 iic_get_ack(void)
{
	u8 ack=0;
		//切换为读模式
	IIC_SCL=0;
	IIC_SDA_OUT=1;	
	iic_delay();iic_delay();    //tLOW
	IIC_SCL=1;
	if(IIC_SDA_IN)   //没有读到响应
	{
		ack=1;
	}
	iic_delay();    //tHIGH
	IIC_SCL=0;
	return ack;
}



