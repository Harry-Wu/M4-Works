#include "stm32f4xx.h" 
#include "iic.h"
#include "at24c02.h"
#include "delay.h"


void at24c04_init(void)
{
	iic_init();
}


//往at24c02中写入一个字节数据

//参数：inner_addr：器件内部地址
//_data:待写入的数据
//返回值：错误状态 
u8 at24c02_write_byte(u16 inner_addr,u8 _data)
{
	iic_start( );
	iic_write_byte(AT24C02_ADDR&0xfe);  //发送器件地址
	if(iic_get_ack())     //读响应，判断有没有从机响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR1;
	}
	
	iic_write_byte(inner_addr);  //发送器件内部地址
	if(iic_get_ack())     //读响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR2;
	}

	iic_write_byte(_data);  //发送数据
	if(iic_get_ack())     //读响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR3;
	}
	
	iic_stop( ); 
	delay_ms(5);
	return AT24C02_NOERR;
	
}


//写任意个字节数据到存储器中不考虑页切换问题
//注意调用此函数必须保证inner_addr/8=(inner_addr+num）/8
//也就是本操作必须是在同一页中
//inner_addr：器件内部地址
//p:指向数据源
//num:写多少个字节数据到存储器
//返回值：操作异常状态
u8 at24c02_page_write(u16 inner_addr,u8 *p,u8 num)
{
	iic_start( );
	iic_write_byte(AT24C02_ADDR&0xfe);  //发送器件地址
	if(iic_get_ack())     //读响应，判断有没有从机响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR1;
	}
	
	iic_write_byte(inner_addr);  //发送器件内部地址
	if(iic_get_ack())     //读响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR2;
	}

	for(;num>0;num--)
	{
		iic_write_byte(*p++);  //发送数据
		if(iic_get_ack())     //读响应
		{
			iic_stop( );   //释放总线
			return AT24C02_ERR3;
		}
	}
	
	iic_stop( ); 
	delay_ms(5);
	return AT24C02_NOERR;
	
}


//从AT24C02中读取一个字节数据
//参数：inner_addr：器件内部地址
//p:指向数据目标存储位置
//返回值：错误状态 
u8 at24c02_read_byte(u16 inner_addr,u8 *p)
{
	iic_start( );
	iic_write_byte(AT24C02_ADDR&0xfe);  //发送器件地址
	if(iic_get_ack())     //读响应，判断有没有从机响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR4;
	}
	
	iic_write_byte(inner_addr);  //发送器件内部地址
	if(iic_get_ack())     //读响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR5;
	}

		iic_start( );
	iic_write_byte(AT24C02_ADDR | 0x01);  //发送器件地址
	if(iic_get_ack())     //读响应，判断有没有从机响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR6;
	}
	
	*p=iic_read_byte( );   //读取数据
	iic_put_ack(1);       //不给响应
 	iic_stop( );   
	return AT24C02_NOERR;
}



//参数：inner_addr：器件内部地址
//p:指向读到的数据存储到哪里
//num:读多少个字节数据
//返回值：错误状态 
u8 at24c02_read_bytes(u16 inner_addr,u8 *p,u16 num)
{
	iic_start( );
	iic_write_byte(AT24C02_ADDR&0xfe);  //发送器件地址
	if(iic_get_ack())     //读响应，判断有没有从机响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR4;
	}
	
	iic_write_byte(inner_addr);  //发送器件内部地址
	if(iic_get_ack())     //读响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR5;
	}

		iic_start( );
	iic_write_byte(AT24C02_ADDR | 0x01);  //发送器件地址
	if(iic_get_ack())     //读响应，判断有没有从机响应
	{
		iic_stop( );   //释放总线
		return AT24C02_ERR6;
	}
	
	for(;num>0;num--)
	{
	  *p++=iic_read_byte( );   //读取数据
		if(num==1)
		{
			break;
		}
		iic_put_ack(0);       //给响应
	}
	iic_put_ack(1);       //不给响应
 	iic_stop( );   
	return AT24C02_NOERR;
}



//写任意多个字节数据到AT24C02
//参数：inner_addr：器件内部地址
//p:指向待写入的数据
//num:写入的数据量
//返回值：错误状态 
u8 at24c02_write_bytes(u16 inner_addr,u8 *p,u16 num)
{
	u8 sta;
	u8  lest_num;
	while(1)
	{
		lest_num=8-inner_addr%8;  //本页中剩余空间
		if(lest_num>=num)       //本页能写完
		{
			lest_num=num;
		}
		sta=at24c02_page_write(inner_addr,p,lest_num);  //注意这里写入的数量为lest_num
		if(sta!=AT24C02_NOERR)  //写入出错处理
		{
			return sta;
		}
		
		if(lest_num==num)  //已经写完
		{
			break;
		}
		num=num-lest_num;         //得到剩余的数据量
		inner_addr=inner_addr+lest_num;  //循环写入新的内部地址
		p=p+lest_num;               //得到新的数据源地址
	}   
	return AT24C02_NOERR;
}


