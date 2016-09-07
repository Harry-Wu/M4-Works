#include "stm32f4xx.h" 
#include "iic.h"
#include "MyTypeDef.h"

void iic_init(void)
{
	//�˿�ʱ��ʹ��pb
	RCC->AHB1ENR |=0X01<<1;
	//�˿�ģʽ����
	//SCL(pb8) SDA(pb9)-----��ͨ���ܿ�©���
	GPIOB->MODER &=~(0X03<<16 | 0X03<<18);
	GPIOB->MODER |=0X01<<16 |0X01<<18; //��ͨ�������
	GPIOB->OTYPER |=0X01<<8 | 0X01<<9;  //��©���
	GPIOB->OSPEEDR &=~(0X03<<16 | 0X03<<18);//����ٶ�2M
	GPIOB->PUPDR &=~(0X03<<16 | 0X03<<18); //�Ȳ�����Ҳ������
		
	//�˿ڳ�ʼ״̬-----�����ߴ��ڿ���״̬
	IIC_SDA_OUT=1;
	IIC_SCL=1;
}

static void iic_delay(void)
{
	u32 i=50;
	while(i--);
}

//��ʼ����
void iic_start(void)
{
	IIC_SCL=0;	
	IIC_SDA_OUT=1;	
	IIC_SCL=1;
	iic_delay();  // >0.6us  tSU.STA
	IIC_SDA_OUT=0;
	iic_delay(); // >0.6us  tHD.STA
	IIC_SCL=0;
}

//ֹͣ����
void iic_stop(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT=0;
	IIC_SCL=1;
	iic_delay();		//>100ns tSU.STO
	IIC_SDA_OUT=1; 
	iic_delay();
	iic_delay();	  //tBUF
}


//����һ���ֽ�����
void iic_write_byte(u8 _data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		                 //tHD.DAT=0
		if(_data & 0x80)
		{
			IIC_SDA_OUT=1;
		}
		else
		{
			IIC_SDA_OUT=0;
		}
		iic_delay();
		iic_delay();    //tLOW
		IIC_SCL=1;
		_data=_data<<1;
		iic_delay();    //tHIGH
	}
		IIC_SCL=0;
}

//����һ���ֽ�����
u8 iic_read_byte(void)
{
	u8 i,rec_data;
	//�л�Ϊ��ģʽ
	IIC_SCL=0;
	IIC_SDA_OUT=1;	
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		iic_delay();iic_delay();    //tLOW
		IIC_SCL=1;
		rec_data=rec_data<<1;
		if(IIC_SDA_IN)
		{
			rec_data =rec_data | 0x01;
		}
		iic_delay();    //tHIGH
	}
	IIC_SCL=0;
	return rec_data;
}


//������Ӧ
//ack :   ack=0������Ӧ��
void iic_put_ack(u8 ack)
{
	IIC_SCL=0;
	if(ack)
	{
		IIC_SDA_OUT=1;    //������Ӧ
	}
	else
	{
		IIC_SDA_OUT=0;    //����Ӧ
	}
	iic_delay();iic_delay();    //tLOW
	IIC_SCL=1;
	iic_delay();    //tHIGH
	IIC_SCL=0;
}
	
//������Ӧ
u8 iic_get_ack(void)
{
	u8 ack=0;
		//�л�Ϊ��ģʽ
	IIC_SCL=0;
	IIC_SDA_OUT=1;	
	iic_delay();iic_delay();    //tLOW
	IIC_SCL=1;
	if(IIC_SDA_IN)   //û�ж�����Ӧ
	{
		ack=1;
	}
	iic_delay();    //tHIGH
	IIC_SCL=0;
	return ack;
}



