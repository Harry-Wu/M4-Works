#include "stm32f4xx.h" 
#include "iic.h"
#include "delay.h"
#include "at24c02.h"

void at24c02_init(void)
{
	iic_init( );
}


//дһ���ֽ����ݵ�AT24C02
//������inner_addr  �����ڲ���ַ
//_data����д�������
//���أ� 
// NO_SLAVER_1�� û�д��豸��Ӧ
// NO_SLAVER_2�� д�����ڲ���ַ����
// NO_SLAVER_3�� д�ֽ����ݴ���
u8 at24c02_write_byte(u16 inner_addr,u8 _data )
{
	iic_start( );       //������ʼ����

	iic_write_byte(AT24C02_ADDR & 0xfe); //����������ַ��дģʽ��
	if(iic_get_ack()==1)      //û�д��豸Ӧ��
	{
		iic_stop( );      //�ͷ�����
		return 		NO_SLAVER_1;
	}
	
	iic_write_byte(inner_addr);   //���������ڲ���ַ
	if(iic_get_ack()==1)      //�ڵص�ַ����
	{
		iic_stop( );      //�ͷ�����
		return 		NO_SLAVER_2;
	}
	
	iic_write_byte(_data);   //�����ֽ�����
	if(iic_get_ack()==1)      //�����ֽ����ݴ���
	{
		iic_stop( );      //�ͷ�����
		return 		NO_SLAVER_3;
	}
	
	iic_stop( );     //����ֹͣ�������ͷ�����
	delay_ms(5);    //�ȴ�AT24C02�ڲ�д�������
	return NO_ERR;

}

//��AT24C02�ж�ȡһ���ֽ�����
u8 at24c02_read_byte(u16 inner_addr,u8 *p )
{
	iic_start(); //������ʼ����
	iic_write_byte(AT24C02_ADDR &0xfe);  //����������ַ��дģʽ��
	if(iic_get_ack()==1)      //û�д��豸��Ӧ
	{
		iic_stop( );      //�ͷ�����
		return 		NO_SLAVER_4;
	}
	
	iic_write_byte(inner_addr) ;//д�����ڲ���ַ
	if(iic_get_ack()==1)      //���������ڲ���ַ����
	{
		iic_stop( );      //�ͷ�����
		return 		NO_SLAVER_5;
	}
	
	iic_start(); //�����ظ���ʼ����
	
	iic_write_byte(AT24C02_ADDR |0x01);  //����������ַ����ģʽ��
	if(iic_get_ack()==1)      //û�д��豸��Ӧ
	{
		iic_stop( );      //�ͷ�����
		return 		NO_SLAVER_6;
	}
	
	*p=iic_read_byte();   //��ȡһ���ֽ�����
	iic_put_ack(1);       //������Ӧ
	iic_stop( );  
	return NO_ERR;
}




//��AT24C02�ж�ȡn���ֽ�����
u8 at24c02_read_bytes(u16 inner_addr,u8 *p, u16 num )
{
	iic_start(); //������ʼ����
	iic_write_byte(AT24C02_ADDR &0xfe);  //����������ַ��дģʽ��
	if(iic_get_ack()==1)      //û�д��豸��Ӧ
	{
		iic_stop( );      //�ͷ�����
		return 		NO_SLAVER_4;
	}
	
	iic_write_byte(inner_addr) ;//д�����ڲ���ַ
	if(iic_get_ack()==1)      //���������ڲ���ַ����
	{
		iic_stop( );      //�ͷ�����
		return 		NO_SLAVER_5;
	}
	
	iic_start(); //�����ظ���ʼ����
	
	iic_write_byte(AT24C02_ADDR |0x01);  //����������ַ����ģʽ��
	if(iic_get_ack()==1)      //û�д��豸��Ӧ
	{
		iic_stop( );      //�ͷ�����
		return 		NO_SLAVER_6;
	}
	
	while(1)
	{
		*p++=iic_read_byte();   //��ȡһ���ֽ�����
		num--;
		if(num==0)
		{
			iic_put_ack(1);       //������Ӧ
			break;
		}
		iic_put_ack(0);       //����Ӧ
	}
	iic_stop( );  
	return NO_ERR;
}


//дn���ֽ����ݵ�AT24C02
//������inner_addr  �����ڲ���ַ
//_data����д�������
//���أ� 
// NO_SLAVER_1�� û�д��豸��Ӧ
// NO_SLAVER_2�� д�����ڲ���ַ����
// NO_SLAVER_3�� д�ֽ����ݴ���
u8 at24c02_write_bytes(u16 inner_addr,u8 *p,u16 num )
{
	u16 synum,i;
	
	
	while(1)
	{
		synum=8-inner_addr%8;   //��ҳʣ���ֽ���
		if(synum>=num)
		{
			synum=num;
		}
		
		iic_start( );       //������ʼ����
		iic_write_byte(AT24C02_ADDR & 0xfe); //����������ַ��дģʽ��
		if(iic_get_ack()==1)      //û�д��豸Ӧ��
		{
			iic_stop( );      //�ͷ�����
			return 		NO_SLAVER_1;
		}
		
		iic_write_byte(inner_addr);   //���������ڲ���ַ
		if(iic_get_ack()==1)      //�ڵص�ַ����
		{
			iic_stop( );      //�ͷ�����
			return 		NO_SLAVER_2;
		}
		
		for(i=0;i<synum;i++)
		{
			iic_write_byte(*p++);   //�����ֽ�����
			if(iic_get_ack()==1)      //�����ֽ����ݴ���
			{
				iic_stop( );      //�ͷ�����
				return 		NO_SLAVER_3;
			}
		}
		iic_stop( );     //����ֹͣ�������ͷ�����
		delay_ms(5);    //�ȴ�AT24C02�ڲ�д�������
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
