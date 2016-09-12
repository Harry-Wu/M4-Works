#include "stm32f4xx.h" 
#include "IIC.h" 

void iic_init(void)
{
	//�˿�ʱ��ʹ��PB
	RCC->AHB1ENR |=0X01<<1;
	//pb8(SCL)(��ͨ���ܸ��տ�©���) 
  //pb9(SDA)(��ͨ���ܸ��տ�©���) 
	GPIOB->MODER &=~(0X03<<16 | 0X03<<18); 
	GPIOB->MODER |=0x01<<16 | 0x01<<18;//��ͨ�������
	GPIOB->OTYPER |=0x01<<8 |0x01<<9;  //��©���
	GPIOB->OSPEEDR &=~(0X03<<16 | 0X03<<18); 
	GPIOB->OSPEEDR|=0x01<<16 | 0x01<<18;    //����ٶ�25M
	GPIOB->PUPDR &=~(0X03<<16 | 0X03<<18);   //����
	
	SDAOUT=1;
	SCL=1;
	
}


void iic_delay(void)
{
	u32 i=100;
	while(i--);
}



//��ʼ����
void iic_start(void)
{
	SCL=0;       
	SDAOUT=1;
	SCL=1;
	iic_delay();
	SDAOUT=0;
	iic_delay();
	SCL=0;
}
//ֹͣ����
void iic_stop(void)
{
	SCL=0;
	SDAOUT=0;
	SCL=1;
	iic_delay();
	SDAOUT=1;
	iic_delay();
	iic_delay();
}
//дһ���ֽ�
//����send_data:�����͵�����
void iic_write_byte(u8 send_data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		SCL=0;
		if(send_data&0x80)
		{
			SDAOUT=1;
		}
		else
		{
			SDAOUT=0;
		}
		iic_delay();
		iic_delay();
		send_data=send_data<<1;
		SCL=1;
		iic_delay();
	
	}
	SCL=0;
}


//��ȡһ���ֽ�����
//����ֵΪ��ȡ��������
u8 iic_read_byte(void)
{
	u8 read_data,i;
	SCL=0;
	SDAOUT=1;   //�������·���ܽŶϿ����л�Ϊ��ģʽ
	
	for(i=0;i<8;i++)
	{
		SCL=0;
		iic_delay();
		iic_delay();
		SCL=1;
		read_data =read_data<<1;
		if(SDAIN)
		{
			read_data |= 0x01;
		}
		iic_delay();
	}
		SCL=0;
	return read_data;
}


//����Ӧ
//����ֵ�� 1���ӻ�����Ӧ 0���ӻ�����Ӧ
u8 iic_get_ack(void)
{
	u8 ack;
	SCL=0;
	SDAOUT=1;    //�������·���ܽŶϿ����л�Ϊ��ģʽ
	iic_delay();
	iic_delay();
	SCL=1;
	if(SDAIN)
	{
		ack=1;
	}
	else
	{
		ack=0;
	}
	iic_delay();
	SCL=0;
	
	return ack;
	
}

//д��Ӧ
//������ACK=1��������Ӧ��ACK=0������Ӧ
void iic_put_ack(u8 ack)
{
	SCL=0;
	if(ack)
	{
		SDAOUT=1;
	}
	else
	{
		SDAOUT=0;
	}
	iic_delay();
	iic_delay();
	SCL=1;
	iic_delay();
	SCL=0;
}
