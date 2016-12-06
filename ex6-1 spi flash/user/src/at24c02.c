#include "stm32f4xx.h" 
#include "iic.h"
#include "at24c02.h"
#include "delay.h"


void at24c04_init(void)
{
	iic_init();
}


//��at24c02��д��һ���ֽ�����

//������inner_addr�������ڲ���ַ
//_data:��д�������
//����ֵ������״̬ 
u8 at24c02_write_byte(u16 inner_addr,u8 _data)
{
	iic_start( );
	iic_write_byte(AT24C02_ADDR&0xfe);  //����������ַ
	if(iic_get_ack())     //����Ӧ���ж���û�дӻ���Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR1;
	}
	
	iic_write_byte(inner_addr);  //���������ڲ���ַ
	if(iic_get_ack())     //����Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR2;
	}

	iic_write_byte(_data);  //��������
	if(iic_get_ack())     //����Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR3;
	}
	
	iic_stop( ); 
	delay_ms(5);
	return AT24C02_NOERR;
	
}


//д������ֽ����ݵ��洢���в�����ҳ�л�����
//ע����ô˺������뱣֤inner_addr/8=(inner_addr+num��/8
//Ҳ���Ǳ�������������ͬһҳ��
//inner_addr�������ڲ���ַ
//p:ָ������Դ
//num:д���ٸ��ֽ����ݵ��洢��
//����ֵ�������쳣״̬
u8 at24c02_page_write(u16 inner_addr,u8 *p,u8 num)
{
	iic_start( );
	iic_write_byte(AT24C02_ADDR&0xfe);  //����������ַ
	if(iic_get_ack())     //����Ӧ���ж���û�дӻ���Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR1;
	}
	
	iic_write_byte(inner_addr);  //���������ڲ���ַ
	if(iic_get_ack())     //����Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR2;
	}

	for(;num>0;num--)
	{
		iic_write_byte(*p++);  //��������
		if(iic_get_ack())     //����Ӧ
		{
			iic_stop( );   //�ͷ�����
			return AT24C02_ERR3;
		}
	}
	
	iic_stop( ); 
	delay_ms(5);
	return AT24C02_NOERR;
	
}


//��AT24C02�ж�ȡһ���ֽ�����
//������inner_addr�������ڲ���ַ
//p:ָ������Ŀ��洢λ��
//����ֵ������״̬ 
u8 at24c02_read_byte(u16 inner_addr,u8 *p)
{
	iic_start( );
	iic_write_byte(AT24C02_ADDR&0xfe);  //����������ַ
	if(iic_get_ack())     //����Ӧ���ж���û�дӻ���Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR4;
	}
	
	iic_write_byte(inner_addr);  //���������ڲ���ַ
	if(iic_get_ack())     //����Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR5;
	}

		iic_start( );
	iic_write_byte(AT24C02_ADDR | 0x01);  //����������ַ
	if(iic_get_ack())     //����Ӧ���ж���û�дӻ���Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR6;
	}
	
	*p=iic_read_byte( );   //��ȡ����
	iic_put_ack(1);       //������Ӧ
 	iic_stop( );   
	return AT24C02_NOERR;
}



//������inner_addr�������ڲ���ַ
//p:ָ����������ݴ洢������
//num:�����ٸ��ֽ�����
//����ֵ������״̬ 
u8 at24c02_read_bytes(u16 inner_addr,u8 *p,u16 num)
{
	iic_start( );
	iic_write_byte(AT24C02_ADDR&0xfe);  //����������ַ
	if(iic_get_ack())     //����Ӧ���ж���û�дӻ���Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR4;
	}
	
	iic_write_byte(inner_addr);  //���������ڲ���ַ
	if(iic_get_ack())     //����Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR5;
	}

		iic_start( );
	iic_write_byte(AT24C02_ADDR | 0x01);  //����������ַ
	if(iic_get_ack())     //����Ӧ���ж���û�дӻ���Ӧ
	{
		iic_stop( );   //�ͷ�����
		return AT24C02_ERR6;
	}
	
	for(;num>0;num--)
	{
	  *p++=iic_read_byte( );   //��ȡ����
		if(num==1)
		{
			break;
		}
		iic_put_ack(0);       //����Ӧ
	}
	iic_put_ack(1);       //������Ӧ
 	iic_stop( );   
	return AT24C02_NOERR;
}



//д�������ֽ����ݵ�AT24C02
//������inner_addr�������ڲ���ַ
//p:ָ���д�������
//num:д���������
//����ֵ������״̬ 
u8 at24c02_write_bytes(u16 inner_addr,u8 *p,u16 num)
{
	u8 sta;
	u8  lest_num;
	while(1)
	{
		lest_num=8-inner_addr%8;  //��ҳ��ʣ��ռ�
		if(lest_num>=num)       //��ҳ��д��
		{
			lest_num=num;
		}
		sta=at24c02_page_write(inner_addr,p,lest_num);  //ע������д�������Ϊlest_num
		if(sta!=AT24C02_NOERR)  //д�������
		{
			return sta;
		}
		
		if(lest_num==num)  //�Ѿ�д��
		{
			break;
		}
		num=num-lest_num;         //�õ�ʣ���������
		inner_addr=inner_addr+lest_num;  //ѭ��д���µ��ڲ���ַ
		p=p+lest_num;               //�õ��µ�����Դ��ַ
	}   
	return AT24C02_NOERR;
}


