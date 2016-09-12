#include "stm32f4xx.h" 
#include "IO_BIT.h" 
#include "LCD.h" 
#include "delay.h"

u16 point_color=BLACK;
u16 back_color=WHITE;

void lcd_init(void)
{
	//端口时钟使能 PD E F G
	RCC->AHB1ENR |=0X01<<1 |0X01<<3 |0X01<<4 |0X01<<5 |0X01<<6;

	//模式----通用输出模式（01）
	GPIOD->MODER &=~(0x03<<0 |0x03<<2 | 0x03<<8 | 0x03<<10 | 0x03<<16 | 0x03<<18 | 0x03<<20 | 0x03<<28 | (u32)0x03<<30);
	GPIOD->MODER |=(0x01<<0 |0x01<<2 | 0x01<<8 | 0x01<<10 | 0x01<<16 | 0x01<<18 | 0x01<<20 | 0x01<<28 | (u32)0x01<<30);
	//输出类型----推挽
	GPIOD->OTYPER &= ~(0X01<<0 | 0X01<<1 | 0X01<<4 | 0X01<<5 | 0X01<<8 | 0X01<<9 | 0X01<<10 | 0X01<<14 | 0X01<<15);	
	//输出速度----100M
	GPIOD->OSPEEDR |=(0x03<<0 |0x03<<2 | 0x03<<8 | 0x03<<10 | 0x03<<16 | 0x03<<18 | 0x03<<20 | 0x03<<28 | (u32)0x03<<30);
	
	
	//模式----通用输出模式（01）
	GPIOE->MODER &=~(0x03<<14 |0x03<<16 | 0x03<<18 | 0x03<<20 | 0x03<<22 | 0x03<<24 | 0x03<<26 | 0x03<<28 | (u32)0x03<<30);
	GPIOE->MODER |=(0x01<<14 |0x01<<16 | 0x01<<18 | 0x01<<20 | 0x01<<22 | 0x01<<24 | 0x01<<26 | 0x01<<28 | (u32)0x01<<30);
	//输出类型----推挽
	GPIOE->OTYPER &= ~(0X01<<7| 0X01<<8 | 0X01<<9 | 0X01<<10 | 0X01<<11 | 0X01<<12 | 0X01<<13 | 0X01<<14 | 0X01<<15);	
	//输出速度----100M
	GPIOE->OSPEEDR |=(0x03<<14 |0x03<<16 | 0x03<<18 | 0x03<<20 | 0x03<<22 | 0x03<<24 | 0x03<<26 | 0x03<<28 | (u32)0x03<<30);
	
	GPIOF->MODER &=~(0X03<<24);
	GPIOF->MODER |=0X01<<24;
	GPIOF->OTYPER &=~(0X01<<12);
	GPIOF->OSPEEDR |=0x03<<24;
	
	GPIOG->MODER &=~(0X03<<24);
	GPIOG->MODER |=0X01<<24;
	GPIOG->OTYPER &=~(0X01<<12);
	GPIOG->OSPEEDR |=0x03<<24;
	
	//设置背光控制
	GPIOB->MODER &=~((unsigned int)0X03<<30);
	GPIOB->MODER |=(unsigned int)0X01<<30;
	GPIOB->OTYPER &=~(0X01<<15);
	GPIOB->OSPEEDR |=(unsigned int)0x03<<30;
	LCD_BL=0;     //关背光
	
	LCD_CS=1;
	LCD_RD=1;
	LCD_WR=1;
	
		delay_ms(120); // Delay 120 ms
	//************* Start Initial Sequence **********//
	LCD_ILI9341_CMD(0xCF);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x83);
	LCD_ILI9341_Parameter (0X30);
	LCD_ILI9341_CMD(0xED);
	LCD_ILI9341_Parameter (0x64);
	LCD_ILI9341_Parameter (0x03);
	LCD_ILI9341_Parameter (0X12);
	LCD_ILI9341_Parameter (0X81);
	LCD_ILI9341_CMD(0xE8);
	LCD_ILI9341_Parameter (0x85);
	LCD_ILI9341_Parameter (0x01);
	LCD_ILI9341_Parameter (0x79);
	LCD_ILI9341_CMD(0xCB);
	LCD_ILI9341_Parameter (0x39);
	LCD_ILI9341_Parameter (0x2C);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x34);
	LCD_ILI9341_Parameter (0x02);
	LCD_ILI9341_CMD(0xF7);
	LCD_ILI9341_Parameter (0x20);

	LCD_ILI9341_CMD(0xEA);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_CMD(0xC0); //Power control
	LCD_ILI9341_Parameter (0x1D); //VRH[5:0]
	LCD_ILI9341_CMD(0xC1); //Power control
	LCD_ILI9341_Parameter (0x11); //SAP[2:0];BT[3:0]
	LCD_ILI9341_CMD(0xC5); //VCM control
	LCD_ILI9341_Parameter (0x33);
	LCD_ILI9341_Parameter (0x34);
	LCD_ILI9341_CMD(0xC7); //VCM control2
	LCD_ILI9341_Parameter (0Xbe);
	LCD_ILI9341_CMD(0x36); // Memory Access Control
	LCD_ILI9341_Parameter (0x08);
	LCD_ILI9341_CMD(0xB1);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x1B);
	LCD_ILI9341_CMD(0xB6); // Display Function Control
	LCD_ILI9341_Parameter (0x0A);
	LCD_ILI9341_Parameter (0xA2);
	LCD_ILI9341_CMD(0xF2); // 3Gamma Function Disable
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_CMD(0x26); //Gamma curve selected
	LCD_ILI9341_Parameter (0x01);
	LCD_ILI9341_CMD(0xE0); //Set Gamma
	LCD_ILI9341_Parameter (0x0F);
	LCD_ILI9341_Parameter (0x23);
	LCD_ILI9341_Parameter (0x1F);

	LCD_ILI9341_Parameter (0x09);
	LCD_ILI9341_Parameter (0x0f);
	LCD_ILI9341_Parameter (0x08);
	LCD_ILI9341_Parameter (0x4B);
	LCD_ILI9341_Parameter (0Xf2);
	LCD_ILI9341_Parameter (0x38);
	LCD_ILI9341_Parameter (0x09);
	LCD_ILI9341_Parameter (0x13);
	LCD_ILI9341_Parameter (0x03);
	LCD_ILI9341_Parameter (0x12);
	LCD_ILI9341_Parameter (0x07);
	LCD_ILI9341_Parameter (0x04);
	LCD_ILI9341_CMD(0XE1); //Set Gamma
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x1d);
	LCD_ILI9341_Parameter (0x20);
	LCD_ILI9341_Parameter (0x02);
	LCD_ILI9341_Parameter (0x11);
	LCD_ILI9341_Parameter (0x07);
	LCD_ILI9341_Parameter (0x34);
	LCD_ILI9341_Parameter (0x81);
	LCD_ILI9341_Parameter (0x46);
	LCD_ILI9341_Parameter (0x06);
	LCD_ILI9341_Parameter (0x0e);
	LCD_ILI9341_Parameter (0x0c);
	LCD_ILI9341_Parameter (0x32);
	LCD_ILI9341_Parameter (0x38);
	LCD_ILI9341_Parameter (0x0F);
	
    /* 由我们添加 */
	LCD_ILI9341_CMD(0X3A); //设定16BPP
	LCD_ILI9341_Parameter(0X55);
	
	LCD_ILI9341_CMD(0x11); // Sleep out
	delay_ms(120);
	LCD_ILI9341_CMD(0x29); // Display on
	
  lcd_clear(0xffff);//把LCD清成白屏
	LCD_BL=1;     //开背光
	
}

//写命令
void LCD_ILI9341_CMD(u8 common)
{
	LCD_RS=0;
	LCD_CS=0;
	LCD_WR=0;
	if(common&(0x01<<0))
		LCD_D0=1;   else LCD_D0=0;
	if(common&(0x01<<1))
		LCD_D1=1;   else LCD_D1=0;
	if(common&(0x01<<2))
		LCD_D2=1;   else LCD_D2=0;
	if(common&(0x01<<3))
		LCD_D3=1;   else LCD_D3=0;
	if(common&(0x01<<4))
		LCD_D4=1;   else LCD_D4=0;
	if(common&(0x01<<5))
		LCD_D5=1;   else LCD_D5=0;
	if(common&(0x01<<6))
		LCD_D6=1;   else LCD_D6=0;
	if(common&(0x01<<7))
		LCD_D7=1;   else LCD_D7=0;
	LCD_WR=1;
	LCD_CS=1;
}



//写显示数据/参数
void LCD_ILI9341_Parameter(u16 _data)
{
	LCD_RS=1;
	LCD_CS=0;
	LCD_WR=0;
	if(_data&(0x01<<0))
		LCD_D0=1;   else LCD_D0=0;
	if(_data&(0x01<<1))
		LCD_D1=1;   else LCD_D1=0;
	if(_data&(0x01<<2))
		LCD_D2=1;   else LCD_D2=0;
	if(_data&(0x01<<3))
		LCD_D3=1;   else LCD_D3=0;
	if(_data&(0x01<<4))
		LCD_D4=1;   else LCD_D4=0;
	if(_data&(0x01<<5))
		LCD_D5=1;   else LCD_D5=0;
	if(_data&(0x01<<6))
		LCD_D6=1;   else LCD_D6=0;
	if(_data&(0x01<<7))
		LCD_D7=1;   else LCD_D7=0;
	
	if(_data&(0x01<<8))
		LCD_D8=1;   else LCD_D8=0;
	if(_data&(0x01<<9))
		LCD_D9=1;   else LCD_D9=0;
	if(_data&(0x01<<10))
		LCD_D10=1;   else LCD_D10=0;
	if(_data&(0x01<<11))
		LCD_D11=1;   else LCD_D11=0;
	if(_data&(0x01<<12))
		LCD_D12=1;   else LCD_D12=0;
	if(_data&(0x01<<13))
		LCD_D13=1;   else LCD_D13=0;
	if(_data&(0x01<<14))
		LCD_D14=1;   else LCD_D14=0;
	if(_data&(0x01<<15))
		LCD_D15=1;   else LCD_D15=0;
	LCD_WR=1;
	LCD_CS=1;
}


void write_piont(u16 x,u16 y,u16 color)
{
	LCD_ILI9341_CMD(0X2A);
	LCD_ILI9341_Parameter(x>>8);
	LCD_ILI9341_Parameter(x&0xff);
	
	LCD_ILI9341_CMD(0X2b);
	LCD_ILI9341_Parameter(y>>8);
	LCD_ILI9341_Parameter(y&0xff);
	
	LCD_ILI9341_CMD(0X2c);
	LCD_ILI9341_Parameter(color);	
}


void lcd_clear(u16 color)
{
	u32 i=320*240;
	
	LCD_ILI9341_CMD(0X2A);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(239>>8);
	LCD_ILI9341_Parameter(239&0xff);
	
	LCD_ILI9341_CMD(0X2b);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(319>>8);
	LCD_ILI9341_Parameter(319&0xff);
	
	LCD_ILI9341_CMD(0X2c);
	while(i--)
	{
		LCD_ILI9341_Parameter(color);	
	}
	
}

//////////////////////////////////////////////////////////////////////	 
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		write_piont(x0+a,y0-b,point_color);
		write_piont(x0+b,y0-a,point_color);
		write_piont(x0+b,y0+a,point_color);
		write_piont(x0+a,y0+b,point_color);
		write_piont(x0-a,y0+b,point_color);
		write_piont(x0-b,y0+a,point_color);
		write_piont(x0-a,y0-b,point_color);
		write_piont(x0-b,y0-a,point_color);
		
   	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
				
	}
}



//////////////////////////////////////////////////////////////////////	 
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		//LCD_DrawPoint(uRow,uCol);//画点
				write_piont(uRow,uCol,point_color);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		}
	
	}  
}   
//////////////////////////////////////////////////////////////////////	 
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}


//显示一个ascii字符
//参数：x,y 显示字符的起始坐标，
//参数zf:待显示字符
//参数mode:显示模式 mode=0 非叠加  mode=1 叠加
void show_ascii(u16 x,u16 y,u8 zf,u8 mode)
{
	u16 x0=x,i,j,color;
	zf=zf-' ';   //待显示字符字模在数组中的行号
	
		for(i=0;i<16;i++)
		{
			color=asc_font[zf][i];
			for(j=0;j<8;j++)
			{
				if(color&0x80)
				{
					write_piont(x,y,point_color);
				}
				else
				{
					if(mode==0)
					{
						write_piont(x,y,back_color);
					}
				}
				x++;
				color=color<<1;
			}
			x=x0;
			y++;
			
		}
		
}


void show_ascii_string(u16 x,u16 y,u8 *p,u8 mode)
{
	while(*p!='\0')
	{
		show_ascii(x,y,*p++, mode);
		x+=8;
		if(x>239-8)
		{
			break;
		}
		
	}
}



//显示一个汉字
//参数：x,y 显示字符的起始坐标，
//参数zf:待显示汉字在数组中是第几个汉字
//参数mode:显示模式 mode=0 非叠加  mode=1 叠加
void show_hz(u16 x,u16 y,u8 zf,u8 mode)
{
	u16 x0=x,i,j,color;

		for(i=0;i<32;i++)
		{
			x=x0;
			if(i>=16)
			{
				x+=8;
			}
			if(i==16)
			{
				y=y-16;
			}
			
			color=hz_font[zf][i];
			for(j=0;j<8;j++)
			{
				if(color&0x80)
				{
					write_piont(x,y,point_color);
				}
				else
				{
					if(mode==0)
					{
						write_piont(x,y,back_color);
					}
				}
				x++;
				color=color<<1;
			}
			
			
			y++;
			
		}
		
}

#if 0
#include "pic.h"
void lcd_show_pic(void)
{
	u32 i=0;
	u16 color;
	
	LCD_ILI9341_CMD(0X2A);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(239>>8);
	LCD_ILI9341_Parameter(239&0xff);
	
	LCD_ILI9341_CMD(0X2b);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(0);
	LCD_ILI9341_Parameter(319>>8);
	LCD_ILI9341_Parameter(319&0xff);
	
	LCD_ILI9341_CMD(0X2c);
	for(i=0;i<320*240*2;i+=2)
	{
		color =gImage_pic[i]| gImage_pic[i+1]<<8 ;
		LCD_ILI9341_Parameter(color);
	}
	
}

#endif

