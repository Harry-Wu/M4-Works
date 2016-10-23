#include "stm32f4xx.h"
#include "iic.h"
#include "at24c02.h"
#include "delay.h"
void at24c02_init(void)
{
	iic_init();
}


//дһ���ֽ����ݵ�AT24C02
//������
//innter_addr:�����ڲ���ַ
//_data:  ��д�������
//����ֵ���쳣״̬
u8 at24c02_write_byte(u16 innter_addr,u8 _data)
{
	iic_start();   //������ʼ����
	iic_write_byte(AT24C02_ADDR&0xfe);//����������ַ��дģʽ��
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_1;    //û���豸��Ӧ
	}
	
	iic_write_byte(innter_addr);//���������ڲ���ַ
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_2;    //���������ڲ���ַ����Ӧ����Ӧ
	}
	
	iic_write_byte(_data);//дһ���ֽ�����
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_3;    //д��������Ӧ
	}
	iic_stop();
	delay_ms(5);
	return NO_ERROR;
}



//��AT24C02��һ���ֽ�����
//������
//innter_addr:�����ڲ���ַ
//p:  ָ�����ݵĴ洢�ռ�
//����ֵ���쳣״̬
u8 at24c02_read_byte(u16 innter_addr,u8 *p)
{
		iic_start();   //������ʼ����
	iic_write_byte(AT24C02_ADDR&0xfe);//����������ַ��дģʽ��
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_4;    //û���豸��Ӧ
	}
	
	iic_write_byte(innter_addr);//���������ڲ���ַ
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_5;    //���������ڲ���ַ����Ӧ����Ӧ
	}
	
	iic_start();   //�����ظ���ʼ����
	
	iic_write_byte(AT24C02_ADDR|0x01);//����������ַ����ģʽ��
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_6;    //û���豸��Ӧ
	}
	
	*p=iic_read_byte();  //��ȡ����
	iic_put_ack(1);     //������Ӧ
	iic_stop();
	return NO_ERROR;
}




//��AT24C02������ֽ�����
//������
//innter_addr:�����ڲ���ַ
//num:��ȡ���ֽ���
//p:  ָ�����ݵĴ洢�ռ�
//����ֵ���쳣״̬
u8 at24c02_read_bytes(u16 innter_addr,u16 num,u8 *p)
{
		iic_start();   //������ʼ����
	iic_write_byte(AT24C02_ADDR&0xfe);//����������ַ��дģʽ��
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_4;    //û���豸��Ӧ
	}
	
	iic_write_byte(innter_addr);//���������ڲ���ַ
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_5;    //���������ڲ���ַ����Ӧ����Ӧ
	}
	
	iic_start();   //�����ظ���ʼ����
	
	iic_write_byte(AT24C02_ADDR|0x01);//����������ַ����ģʽ��
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_6;    //û���豸��Ӧ
	}
	while(1)
	{
		*p++=iic_read_byte();  //��ȡ����
		num--;
		if(0==num)
		{
			break;
		}
		iic_put_ack(0);     //����Ӧ
	}
	iic_put_ack(1);     //������Ӧ
	iic_stop();
	return NO_ERROR;
}



//�����жϵ�����д
//ʹ������innter_addr/8=(innter_addr+num-1)/8
static u8 at24c02_no_check_write_bytes(u16 innter_addr,u16 num,u8 *p)
{
	iic_start();   //������ʼ����
	iic_write_byte(AT24C02_ADDR&0xfe);//����������ַ��дģʽ��
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_1;    //û���豸��Ӧ
	}
	
	iic_write_byte(innter_addr);//���������ڲ���ַ
	if(iic_get_ack())
	{
		iic_stop();
		return ERROR_2;    //���������ڲ���ַ����Ӧ����Ӧ
	}
	
	while(num--)
	{
		iic_write_byte(*p++);//дһ���ֽ�����
		if(iic_get_ack())
		{
			iic_stop();
			return ERROR_3;    //д��������Ӧ
		}
	}
	iic_stop();
	delay_ms(5);
	return NO_ERROR;
	
	
	
}


//д����ֽ����ݵ�AT24C02
//������
//innter_addr:�����ڲ���ַ
//num�� ��д���������
//p:  ָ������Դ
//����ֵ���쳣״̬
u8 at24c02_write_bytes(u16 innter_addr,u16 num,u8 *p)
{
	u16 less_addr;	
	while(1)
	{
	
		less_addr=8-innter_addr%8;   //��ҳ��ʣ���ַ�ռ�
		if(less_addr>=num)   //��ҳ��д��
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








