#include "stm32f4xx.h"
#include "iic.h"
#include "delay.h"
void iic_init(void)
{
	//�˿�ʱ��ʹ��
	RCC->AHB1ENR |=0X01<<1;
	//scl(pb8) sda(pb9)��ͨ���ܿ�©���������������
	GPIOB->MODER &=~(0x03<<16 | 0x03<<18);
	GPIOB->MODER |=0x01<<16 | 0x01<<18;
	GPIOB->OTYPER |=0X03<<8;
	GPIOB->OSPEEDR &=~(0x03<<16 | 0x03<<18);
	GPIOB->OSPEEDR |=0X01<<16 |0X01<<18;  //����ٶ�25M
	GPIOB->PUPDR  &=~(0x03<<16 | 0x03<<18);
	
	IIC_SDAOUT=1;
	IIC_SCL=1;
}

//����������ʼ����
//����ռ�����ߣ��������д��豸
//SCLΪ�ߵ�ƽ�ڼ䣬SDA��һ���½���
void iic_start(void)
{
	IIC_SCL=0;
	IIC_SDAOUT=1;
	IIC_SCL=1;
	delay_us(1);
	IIC_SDAOUT=0;   //sda�½��س�������仰��
	delay_us(1);
	IIC_SCL=0;	  //�������������
}	

//��������ֹͣ����
//ֹͣͨ�š��ͷ�����
//��SCLΪ�ߵ�ƽ�ڼ䣬SDA��һ��������
void iic_stop(void)
{
	IIC_SCL=0;
	IIC_SDAOUT=0;
	IIC_SCL=1;
	delay_us(1);
	IIC_SDAOUT=1;	
	delay_us(2);
}
	

void iic_write_byte(u8 _data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		if(_data & 0x80)	IIC_SDAOUT=1;
		else  IIC_SDAOUT=0;
		delay_us(2);
		IIC_SCL=1;
		_data=_data<<1;
		delay_us(1);
	}
	IIC_SCL=0;
}

u8 iic_read_byte(void)
{
	u8 i,rec_data;
	IIC_SCL=0;
	IIC_SDAOUT=1;   //�л�Ϊ��ģʽ
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		delay_us(2);
		IIC_SCL=1;
		rec_data=rec_data<<1;
		if(IIC_SDAIN)
		{
			rec_data=rec_data |0x01;
		}
		delay_us(1);
	}
	IIC_SCL=0;
	return rec_data;
}	

//���������յ�һ���ֽ����ݺ󣬸��ӻ�һ����Ӧ
//ack=0,����Ӧ
void iic_put_ack(u8 ack)
{
	IIC_SCL=0;
	if(ack)		IIC_SDAOUT=1;
	else	IIC_SDAOUT=0;   //����Ӧ
	delay_us(2);
	IIC_SCL=1;
	delay_us(1);
	IIC_SCL=0;
}	
//����������һ���ֽ�
//���ݸ��ӻ�����Ҫ�жϴӻ��Ƿ���
//��Ӧ
//����ֵ�� ����0 �еõ���Ӧ
u8  iic_get_ack(void)
{
	u8 ack=0;
	
	IIC_SCL=0;
	IIC_SDAOUT=1;  //�л�Ϊ��ģʽ
	delay_us(2);
	IIC_SCL=1;
	if(IIC_SDAIN)
	{
		ack=1;
	}
	delay_us(1);
	IIC_SCL=0;
	return ack;
}






