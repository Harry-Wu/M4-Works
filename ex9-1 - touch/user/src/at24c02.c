#include "stm32f4xx.h" 
#include "iic.h"
#include "at24c02.h"
#include "delay.h"


void at24c02_init(void)
{
	iic_init( );
}



//д����_data��AT24C02�ڲ�innter��ַ
//����ֵ����������

u8 at24c02_write_byte(u16 innter,u8 _data)
{
	iic_start( );  //������ʼ����
	iic_write_byte(AT24C02_ADDR&0xfe);  //����������ַ��дģʽ
	if(iic_get_ack( ))    //���û�ж�����Ӧ��û�д��豸��Ӧ
	{
		iic_stop( );
		return  ACK1;
	}
		iic_write_byte(innter);  //���������ڲ���ַ
	if(iic_get_ack( ))    //���û�ж�����Ӧ
	{
		iic_stop( );
		return  ACK2;
	}
	
	iic_write_byte(_data);  //���������ڲ���ַ
	if(iic_get_ack( ))    //���û�ж�����Ӧ
	{
		iic_stop( );
		return  ACK3;
	}
	iic_stop( );
	delay_ms(5);
	return ACK_OK;
}


//��AT24C02�ڲ���ַinnter��ȡһ���ֽ���
//�洢��ָ�����p��ָ��ĵ�ַ�ռ�
//����ֵ����������
u8 at24c02_read_byte(u16 innter,u8 *p)
{

	iic_start( );  //������ʼ����
	iic_write_byte(AT24C02_ADDR&0xfe);  //����������ַ��дģʽ
	if(iic_get_ack( ))    //���û�ж�����Ӧ��û�д��豸��Ӧ
	{
		iic_stop( );
		return  ACK1;
	}
		iic_write_byte(innter);  //���������ڲ���ַ
	if(iic_get_ack( ))    //���û�ж�����Ӧ
	{
		iic_stop( );
		return  ACK2;
	}
	
	iic_start( );  //�����ظ���ʼ����
	iic_write_byte(AT24C02_ADDR|0x01);  //����������ַ����ģʽ
	if(iic_get_ack( ))    //���û�ж�����Ӧ��û�д��豸��Ӧ
	{
		iic_stop( );
		return  ACK4;
	}
	*p=iic_read_byte( );  //������
	iic_put_ack(1);     //������Ӧ
  iic_stop( );
	return ACK_OK;
	
}


//��AT24C02�ڲ���ַinnter������ȡnum���ֽ���
//�洢��ָ�����p��ָ��ĵ�ַ�ռ�
//����ֵ����������
u8 at24c02_read_bytes(u16 innter,u8 num,u8 *p)
{

	iic_start( );  //������ʼ����
	iic_write_byte(AT24C02_ADDR&0xfe);  //����������ַ��дģʽ
	if(iic_get_ack( ))    //���û�ж�����Ӧ��û�д��豸��Ӧ
	{
		iic_stop( );
		return  ACK1;
	}
		iic_write_byte(innter);  //���������ڲ���ַ
	if(iic_get_ack( ))    //���û�ж�����Ӧ
	{
		iic_stop( );
		return  ACK2;
	}
	
	iic_start( );  //�����ظ���ʼ����
	iic_write_byte(AT24C02_ADDR|0x01);  //����������ַ����ģʽ
	if(iic_get_ack( ))    //���û�ж�����Ӧ��û�д��豸��Ӧ
	{
		iic_stop( );
		return  ACK4;
	}
	for(;num>0;num--)
	{
		*p++=iic_read_byte( );  //������
		if(num==1)
		{
			iic_put_ack(1); 
			break;			
		}
		iic_put_ack(0);     //����Ӧ
	}
  iic_stop( );
	return ACK_OK;
	
}



//��AT24C02�ڲ���ַinnter����дnum���ֽ���
//�洢��ָ�����p��ָ��ĵ�ַ�ռ�
//����ֵ����������
u8 at24c02_write_bytes(u16 innter,u8 num,u8 *p)
{
	u8 i;
	u16 less_addr;
	
	while(1)
	{
		less_addr=8-innter%8;   //��ʼ��ַ������ҳ��ʣ���ٸ��ֽڿռ�
		
		if(less_addr>=num)
		{
			less_addr=num;
		}
			///////////////////////////////
			iic_start( );  //������ʼ����
			iic_write_byte(AT24C02_ADDR&0xfe);  //����������ַ��дģʽ
			if(iic_get_ack( ))    //���û�ж�����Ӧ��û�д��豸��Ӧ
			{
				iic_stop( );
				return  ACK1;
			}
				iic_write_byte(innter);  //���������ڲ���ַ
			if(iic_get_ack( ))    //���û�ж�����Ӧ
			{
				iic_stop( );
				return  ACK2;
			}
			for(i=0;i<less_addr;i++)
			{
				iic_write_byte(*p++);  //��������
				if(iic_get_ack( ))    //���û�ж�����Ӧ
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




