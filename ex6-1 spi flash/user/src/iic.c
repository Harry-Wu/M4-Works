#include "stm32f4xx.h" 
#include "iic.h"

static void iic_delay(void)
{
	u32 i=100;
	while(i--);
}



void iic_init(void)
{
	//�˿�ʱ��ʹ��PB
	RCC->AHB1ENR |=0X01<<1;
	//�˿�����PB8(SCL)  PB9(SDA)----��ͨ���ܿ�©�����������
	GPIOB->MODER &=~((0X3<<16)| (0X3<<18));
	GPIOB->MODER |=((0X1<<16)| (0X1<<18)) ;  //��ͨ�������
	GPIOB->OTYPER |=((0X01<<8)| (0X01<<9));   //��©���
	GPIOB->OSPEEDR &=~((0X3<<16)| (0X3<<18));  //����ٶ�2M
	GPIOB->PUPDR &=~((0X3<<16)| (0X3<<18));    //����/����
	
	//�����ߴ��ڿ���״̬
	IIC_SCL=1;
	IIC_SDA_OUT=1;
}


//��ʼ����
void iic_start(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT=1;
	IIC_SCL=1;
	iic_delay( );
	IIC_SDA_OUT=0;
	iic_delay( );
	IIC_SCL=0;
}
//ֹͣ����
void iic_stop(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT=0;
	IIC_SCL=1;
	iic_delay( );
	IIC_SDA_OUT=1;
	iic_delay( );
	iic_delay( );
}

//дһ���ֽ�
void iic_write_byte(u8 send_data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		if(send_data & 0x80)
		{
			IIC_SDA_OUT=1;
		}
		else 
		{
			IIC_SDA_OUT=0;
		}
		send_data=send_data<<1;
		iic_delay( );
		iic_delay( );
		IIC_SCL=1;
		iic_delay( );
	}
	IIC_SCL=0;
}


//��ȡһ���ֽ�
//����ֵ����ȡ����һ���ֽ�����
u8 iic_read_byte(void)
{
	u8 i,rec_data;
	//�л�Ϊ��ģʽ
	IIC_SCL=0;
	IIC_SDA_OUT=1;
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		iic_delay( );
		iic_delay( );
		IIC_SCL=1;
		rec_data =rec_data<<1;
		if(IIC_SDA_IN)
		{
			rec_data |=1;
		}
		iic_delay( );
	}
		IIC_SCL=0;
	return rec_data;
}
//��Ӧ��
//����������һ���ֽ����ݺ󣬴ӻ����Ӧ��
//�������ʱ���Ҫ��ȡӦ���жϴӻ��Ƿ�����
//���յ�������
//����ֵ�� 1��û��Ӧ��0����Ӧ��
u8 iic_get_ack(void)
{
	u8 ack;
	//�л�Ϊ��ģʽ
	IIC_SCL=0;
	IIC_SDA_OUT=1;
	iic_delay( );
	iic_delay( );
	IIC_SCL=1;
	if(IIC_SDA_IN)
	{
		ack=1;
	}
	else
	{
		ack=0;
	}
	iic_delay( );
	IIC_SCL=0;
	return ack;
}
	
//������Ӧ
//���������յ��ӻ����͹�����һ���ֽ����ݺ�
//����򲻸��ӻ�һ����Ӧ
//������ ack=0 ����Ӧ��ack=1������Ӧ
void iic_put_ack(u8 ack)
{
	IIC_SCL=0;
	if(ack)
	{
		IIC_SDA_OUT=1;   //������Ӧ
	}
	else
	{
		IIC_SDA_OUT=0;  //����Ӧ
	}
	iic_delay( );
	iic_delay( );
	IIC_SCL=1;
	iic_delay( );
	IIC_SCL=0;
}




