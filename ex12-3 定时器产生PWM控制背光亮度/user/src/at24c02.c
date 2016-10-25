#include "stm32f4xx.h"
#include "iic.h"
#include "at24c02.h"
#include "delay.h"
void at24c02_init(void)
{
	iic_init();
}


//写一个字节数据到AT24C02
//参数：
//innter_addr:器件内部地址
//_data:  待写入的数据
//返回值：异常状态
u8 at24c02_write_byte(u16 innter_addr,u8 _data)
{
	iic_start();   //发送起始条件
	iic_write_byte(AT24C02_ADDR&0xfe);//发送器件地址（写模式）
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_1;    //没有设备响应
	}
	
	iic_write_byte(innter_addr);//发送器件内部地址
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_2;    //发送器件内部地址无响应无响应
	}
	
	iic_write_byte(_data);//写一个字节数据
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_3;    //写数据无响应
	}
	iic_stop();
	delay_ms(5);
	return NO_ERROR;
}



//从AT24C02读一个字节数据
//参数：
//innter_addr:器件内部地址
//p:  指向数据的存储空间
//返回值：异常状态
u8 at24c02_read_byte(u16 innter_addr,u8 *p)
{
		iic_start();   //发送起始条件
	iic_write_byte(AT24C02_ADDR&0xfe);//发送器件地址（写模式）
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_4;    //没有设备响应
	}
	
	iic_write_byte(innter_addr);//发送器件内部地址
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_5;    //发送器件内部地址无响应无响应
	}
	
	iic_start();   //发送重复起始条件
	
	iic_write_byte(AT24C02_ADDR|0x01);//发送器件地址（读模式）
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_6;    //没有设备响应
	}
	
	*p=iic_read_byte();  //读取数据
	iic_put_ack(1);     //不给响应
	iic_stop();
	return NO_ERROR;
}




//从AT24C02读多个字节数据
//参数：
//innter_addr:器件内部地址
//num:读取的字节数
//p:  指向数据的存储空间
//返回值：异常状态
u8 at24c02_read_bytes(u16 innter_addr,u16 num,u8 *p)
{
		iic_start();   //发送起始条件
	iic_write_byte(AT24C02_ADDR&0xfe);//发送器件地址（写模式）
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_4;    //没有设备响应
	}
	
	iic_write_byte(innter_addr);//发送器件内部地址
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_5;    //发送器件内部地址无响应无响应
	}
	
	iic_start();   //发送重复起始条件
	
	iic_write_byte(AT24C02_ADDR|0x01);//发送器件地址（读模式）
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_6;    //没有设备响应
	}
	while(1)
	{
		*p++=iic_read_byte();  //读取数据
		num--;
		if(0==num)
		{
			break;
		}
		iic_put_ack(0);     //给响应
	}
	iic_put_ack(1);     //不给响应
	iic_stop();
	return NO_ERROR;
}



//不加判断的连续写
//使用条件innter_addr/8=(innter_addr+num-1)/8
static u8 at24c02_no_check_write_bytes(u16 innter_addr,u16 num,u8 *p)
{
	iic_start();   //发送起始条件
	iic_write_byte(AT24C02_ADDR&0xfe);//发送器件地址（写模式）
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_1;    //没有设备响应
	}
	
	iic_write_byte(innter_addr);//发送器件内部地址
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_2;    //发送器件内部地址无响应无响应
	}
	
	while(num--)
	{
		iic_write_byte(*p++);//写一个字节数据
		if(iic_get_ack())
		{
			iic_stop();
			return ERROR_3;    //写数据无响应
		}
	}
	iic_stop();
	delay_ms(5);
	return NO_ERROR;
	
	
	
}


//写多个字节数据到AT24C02
//参数：
//innter_addr:器件内部地址
//num： 待写入的数据量
//p:  指向数据源
//返回值：异常状态
u8 at24c02_write_bytes(u16 innter_addr,u16 num,u8 *p)
{
	u16 less_addr;	
	while(1)
	{
	
		less_addr=8-innter_addr%8;   //本页中剩余地址空间
		if(less_addr>=num)   //本页能写完
		{
			less_addr=num;    
		}
		at24c02_no_check_write_bytes(innter_addr,less_addr,p);
		if(less_addr==num)
		{
			break;
		}
		innter_addr=innter_addr+less_addr;
		num=num-less_addr;
		p=p+less_addr;
	}
	
}








