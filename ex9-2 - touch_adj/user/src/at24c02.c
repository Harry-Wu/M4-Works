#include "stm32f4xx.h" 
#include "iic.h"
#include "at24c02.h"
#include "delay.h"


void at24c02_init(void)
{
	iic_init( );
}



//写数据_data到AT24C02内部innter地址
//返回值：错误类型

u8 at24c02_write_byte(u16 innter,u8 _data)
{
	iic_start( );  //发送起始条件
	iic_write_byte(AT24C02_ADDR&0xfe);  //发送器件地址，写模式
	if(iic_get_ack( ))    //如果没有读到响应，没有从设备响应
	{
		iic_stop( );
		return  ACK1;
	}
		iic_write_byte(innter);  //发送器件内部地址
	if(iic_get_ack( ))    //如果没有读到响应
	{
		iic_stop( );
		return  ACK2;
	}
	
	iic_write_byte(_data);  //发送器件内部地址
	if(iic_get_ack( ))    //如果没有读到响应
	{
		iic_stop( );
		return  ACK3;
	}
	iic_stop( );
	delay_ms(5);
	return ACK_OK;
}


//从AT24C02内部地址innter读取一个字节数
//存储到指针变量p所指向的地址空间
//返回值：错误类型
u8 at24c02_read_byte(u16 innter,u8 *p)
{

	iic_start( );  //发送起始条件
	iic_write_byte(AT24C02_ADDR&0xfe);  //发送器件地址，写模式
	if(iic_get_ack( ))    //如果没有读到响应，没有从设备响应
	{
		iic_stop( );
		return  ACK1;
	}
		iic_write_byte(innter);  //发送器件内部地址
	if(iic_get_ack( ))    //如果没有读到响应
	{
		iic_stop( );
		return  ACK2;
	}
	
	iic_start( );  //发送重复起始条件
	iic_write_byte(AT24C02_ADDR|0x01);  //发送器件地址，读模式
	if(iic_get_ack( ))    //如果没有读到响应，没有从设备响应
	{
		iic_stop( );
		return  ACK4;
	}
	*p=iic_read_byte( );  //读数据
	iic_put_ack(1);     //不给响应
  iic_stop( );
	return ACK_OK;
	
}


//从AT24C02内部地址innter连续读取num个字节数
//存储到指针变量p所指向的地址空间
//返回值：错误类型
u8 at24c02_read_bytes(u16 innter,u8 num,u8 *p)
{

	iic_start( );  //发送起始条件
	iic_write_byte(AT24C02_ADDR&0xfe);  //发送器件地址，写模式
	if(iic_get_ack( ))    //如果没有读到响应，没有从设备响应
	{
		iic_stop( );
		return  ACK1;
	}
		iic_write_byte(innter);  //发送器件内部地址
	if(iic_get_ack( ))    //如果没有读到响应
	{
		iic_stop( );
		return  ACK2;
	}
	
	iic_start( );  //发送重复起始条件
	iic_write_byte(AT24C02_ADDR|0x01);  //发送器件地址，读模式
	if(iic_get_ack( ))    //如果没有读到响应，没有从设备响应
	{
		iic_stop( );
		return  ACK4;
	}
	for(;num>0;num--)
	{
		*p++=iic_read_byte( );  //读数据
		if(num==1)
		{
			iic_put_ack(1); 
			break;			
		}
		iic_put_ack(0);     //给响应
	}
  iic_stop( );
	return ACK_OK;
	
}



//往AT24C02内部地址innter连续写num个字节数
//存储到指针变量p所指向的地址空间
//返回值：错误类型
u8 at24c02_write_bytes(u16 innter,u8 num,u8 *p)
{
	u8 i;
	u16 less_addr;
	
	while(1)
	{
		less_addr=8-innter%8;   //起始地址所处的页还剩多少个字节空间
		
		if(less_addr>=num)
		{
			less_addr=num;
		}
			///////////////////////////////
			iic_start( );  //发送起始条件
			iic_write_byte(AT24C02_ADDR&0xfe);  //发送器件地址，写模式
			if(iic_get_ack( ))    //如果没有读到响应，没有从设备响应
			{
				iic_stop( );
				return  ACK1;
			}
				iic_write_byte(innter);  //发送器件内部地址
			if(iic_get_ack( ))    //如果没有读到响应
			{
				iic_stop( );
				return  ACK2;
			}
			for(i=0;i<less_addr;i++)
			{
				iic_write_byte(*p++);  //发送数据
				if(iic_get_ack( ))    //如果没有读到响应
				{
					iic_stop( );
					return  ACK3;
				}
			}
			iic_stop( );
			delay_ms(5);
			if(less_addr==num)
			{
				break;
			}
			////////////////////////////////////
			innter = innter+less_addr;
			num = num-less_addr;
		}
	
		return ACK_OK;
		
}




