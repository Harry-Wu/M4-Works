#include "stm32f4xx.h" 
#include "iic.h"
#include "delay.h"
#include "at24c02.h"

void at24c02_init(void)
{
	iic_init( );
}


//写一个字节数据到AT24C02
//参数：inner_addr  器件内部地址
//_data：待写入的数据
//返回： 
// NO_SLAVER_1： 没有从设备响应
// NO_SLAVER_2： 写器件内部地址错误
// NO_SLAVER_3： 写字节数据错误
u8 at24c02_write_byte(u16 inner_addr,u8 _data )
{
	iic_start( );       //发送起始条件

	iic_write_byte(AT24C02_ADDR & 0xfe); //发送器件地址（写模式）
	if(iic_get_ack()==1)      //没有从设备应答
	{
		iic_stop( );      //释放总线
		return 		NO_SLAVER_1;
	}
	
	iic_write_byte(inner_addr);   //发送器件内部地址
	if(iic_get_ack()==1)      //内地地址错误
	{
		iic_stop( );      //释放总线
		return 		NO_SLAVER_2;
	}
	
	iic_write_byte(_data);   //发送字节数据
	if(iic_get_ack()==1)      //发送字节数据错误
	{
		iic_stop( );      //释放总线
		return 		NO_SLAVER_3;
	}
	
	iic_stop( );     //发送停止条件，释放总线
	delay_ms(5);    //等待AT24C02内部写操作完成
	return NO_ERR;

}

//从AT24C02中读取一个字节数据
u8 at24c02_read_byte(u16 inner_addr,u8 *p )
{
	iic_start(); //发送起始条件
	iic_write_byte(AT24C02_ADDR &0xfe);  //发送器件地址（写模式）
	if(iic_get_ack()==1)      //没有从设备响应
	{
		iic_stop( );      //释放总线
		return 		NO_SLAVER_4;
	}
	
	iic_write_byte(inner_addr) ;//写器件内部地址
	if(iic_get_ack()==1)      //发送器件内部地址错误
	{
		iic_stop( );      //释放总线
		return 		NO_SLAVER_5;
	}
	
	iic_start(); //发送重复起始条件
	
	iic_write_byte(AT24C02_ADDR |0x01);  //发送器件地址（读模式）
	if(iic_get_ack()==1)      //没有从设备响应
	{
		iic_stop( );      //释放总线
		return 		NO_SLAVER_6;
	}
	
	*p=iic_read_byte();   //读取一个字节数据
	iic_put_ack(1);       //不给响应
	iic_stop( );  
	return NO_ERR;
}




//从AT24C02中读取n个字节数据
u8 at24c02_read_bytes(u16 inner_addr,u8 *p, u16 num )
{
	iic_start(); //发送起始条件
	iic_write_byte(AT24C02_ADDR &0xfe);  //发送器件地址（写模式）
	if(iic_get_ack()==1)      //没有从设备响应
	{
		iic_stop( );      //释放总线
		return 		NO_SLAVER_4;
	}
	
	iic_write_byte(inner_addr) ;//写器件内部地址
	if(iic_get_ack()==1)      //发送器件内部地址错误
	{
		iic_stop( );      //释放总线
		return 		NO_SLAVER_5;
	}
	
	iic_start(); //发送重复起始条件
	
	iic_write_byte(AT24C02_ADDR |0x01);  //发送器件地址（读模式）
	if(iic_get_ack()==1)      //没有从设备响应
	{
		iic_stop( );      //释放总线
		return 		NO_SLAVER_6;
	}
	
	while(1)
	{
		*p++=iic_read_byte();   //读取一个字节数据
		num--;
		if(num==0)
		{
			iic_put_ack(1);       //不给响应
			break;
		}
		iic_put_ack(0);       //给响应
	}
	iic_stop( );  
	return NO_ERR;
}


//写n个字节数据到AT24C02
//参数：inner_addr  器件内部地址
//_data：待写入的数据
//返回： 
// NO_SLAVER_1： 没有从设备响应
// NO_SLAVER_2： 写器件内部地址错误
// NO_SLAVER_3： 写字节数据错误
u8 at24c02_write_bytes(u16 inner_addr,u8 *p,u16 num )
{
	u16 synum,i;
	
	
	while(1)
	{
		synum=8-inner_addr%8;   //本页剩余字节数
		if(synum>=num)
		{
			synum=num;
		}
		
		iic_start( );       //发送起始条件
		iic_write_byte(AT24C02_ADDR & 0xfe); //发送器件地址（写模式）
		if(iic_get_ack()==1)      //没有从设备应答
		{
			iic_stop( );      //释放总线
			return 		NO_SLAVER_1;
		}
		
		iic_write_byte(inner_addr);   //发送器件内部地址
		if(iic_get_ack()==1)      //内地地址错误
		{
			iic_stop( );      //释放总线
			return 		NO_SLAVER_2;
		}
		
		for(i=0;i<synum;i++)
		{
			iic_write_byte(*p++);   //发送字节数据
			if(iic_get_ack()==1)      //发送字节数据错误
			{
				iic_stop( );      //释放总线
				return 		NO_SLAVER_3;
			}
		}
		iic_stop( );     //发送停止条件，释放总线
		delay_ms(5);    //等待AT24C02内部写操作完成
		if(synum==num)
		{
			break;
		}
			num=num-synum;
			inner_addr=inner_addr+synum;
			//p=p+synum;
	}
	return NO_ERR;

}
