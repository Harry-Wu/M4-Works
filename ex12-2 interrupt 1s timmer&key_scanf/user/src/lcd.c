#include "stm32f4xx.h"
#include "lcd.h"


u16 POINT_COLOR=BLACK;
u16 BACK_COLOR=WHITE;


/********************************************************************
* Function：   lcd_port_init
* Description: 初始化lcd引脚
* Input：  无
* Output： 无
* Return : 无
* Author:  XYD
* Others:  这个函数必须要调用才可以正常操LCD
* date of completion:  2015-08-24
* date of last modify: 2015-08-24
*********************************************************************/
void lcd_port_init(void)
{
	//1、开引脚时钟 PD PE PG PB PF
	RCC->AHB1ENR |= (1<<3);//开PD
	RCC->AHB1ENR |= (1<<4);//开PE
	RCC->AHB1ENR |= (1<<6);//开PG
	RCC->AHB1ENR |= (1<<1);//开PB
	RCC->AHB1ENR |= (1<<5);//开PF
	//2、配置引脚
	// 数据线：-----推挽输出  时钟50M
	// DB0---PD14
	// DB1---PD15
	// DB2---PD0
	// DB3---PD1
	GPIOD->MODER |=(0X1<<(2*14))|(0X1<<(2*15));  //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*14))|(0X2<<(2*15));   //配置成输出速率50M 		

	GPIOD->MODER |=(0X1<<(2*0))|(0X1<<(2*1));  //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*0))|(0X2<<(2*1));   //配置成输出速率50M
	
	// DB4---PE7
	// DB5---PE8
	// DB6---PE9
	// DB7---PE10
	// DB8---PE11
	// DB9---PE12
	// DB10---PE13
	// DB11---PE14
	// DB12---PE15
	GPIOE->MODER |=0x55554000;  //配置成通用输出  
	GPIOE->OTYPER |= 0;  //配置成输出推挽
	GPIOE->OSPEEDR |=0x55554000;  //配置成输出速率50M

	// DB13---PD8
	// DB14---PD9
	// DB15---PD10
	GPIOD->MODER |=(0X1<<(2*8))|(0X1<<(2*9))|(0X1<<(2*10));  //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*1))|(0X2<<(2*1))|(0X2<<(2*1));   //配置成输出速率50M
	// 控制线：-----推挽输出  时钟50M 空闲为高电平
	// WR：PD5
	// RD：PD4
	GPIOD->MODER |=(0X1<<(2*4))|(0X1<<(2*5));  //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*4))|(0X2<<(2*5));
	GPIOD->ODR |= (1<<4)|(1<<5);
	// CE：PG12
	GPIOG->MODER |=(0X1<<(2*12));  //配置成通用输出  
	GPIOG->OSPEEDR |=(0X2<<(2*12));
	GPIOG->ODR |= (1<<12);
	// RS：PF12
	GPIOF->MODER |=(0X1<<(2*12));  //配置成通用输出  
	GPIOF->OSPEEDR |=(0X2<<(2*12));
	GPIOF->ODR |= (1<<12);
	//LCD的背光 BL--PB15 1亮  三极管
	GPIOB->MODER |=(0X1<<(2*15));  //配置成通用输出  
	GPIOB->OSPEEDR |=(0X2<<(2*15));
	GPIOB->ODR &= ~(1<<15);//关闭背光，在LCD初始化完后再打开

}

//发送命令字
void LCD_ILI9341_CMD(u8 common)
{
	u8 i;
	LCD_WR=1;
	LCD_RS=0;    //命令
	LCD_RD=1;  
	
	LCD_CS=0;
	LCD_WR=0;
	if(common&0x01) LCD_D0=1; else LCD_D0=0;
	if(common&0x02) LCD_D1=1; else LCD_D1=0;
	if(common&0x04) LCD_D2=1; else LCD_D2=0;
	if(common&0x08) LCD_D3=1; else LCD_D3=0;
	if(common&0x10) LCD_D4=1; else LCD_D4=0;
	if(common&0x20) LCD_D5=1; else LCD_D5=0;
	if(common&0x40) LCD_D6=1; else LCD_D6=0;
	if(common&0x80) LCD_D7=1; else LCD_D7=0;
	i=10;
	LCD_WR=1;
	LCD_CS=1;
	
}

//写命令参数
void LCD_ILI9341_Parameter(u8 common)
{
	u8 i;
	LCD_WR=1;
	LCD_RS=1;    //数据
	LCD_RD=1;  
	
	LCD_CS=0;
	LCD_WR=0;
	if(common&0x01) LCD_D0=1; else LCD_D0=0;
	if(common&0x02) LCD_D1=1; else LCD_D1=0;
	if(common&0x04) LCD_D2=1; else LCD_D2=0;
	if(common&0x08) LCD_D3=1; else LCD_D3=0;
	if(common&0x10) LCD_D4=1; else LCD_D4=0;
	if(common&0x20) LCD_D5=1; else LCD_D5=0;
	if(common&0x40) LCD_D6=1; else LCD_D6=0;
	if(common&0x80) LCD_D7=1; else LCD_D7=0;
	i=10;
	LCD_WR=1;
	LCD_CS=1;
	
}



//写显示颜色
void lcd_send_data(u16 common)
{
	u8 i;
	LCD_WR=1;
	LCD_RS=1;    //数据
	LCD_RD=1;  
	
	LCD_CS=0;
	LCD_WR=0;
	if(common&0x01) LCD_D0=1; else LCD_D0=0;
	if(common&0x02) LCD_D1=1; else LCD_D1=0;
	if(common&0x04) LCD_D2=1; else LCD_D2=0;
	if(common&0x08) LCD_D3=1; else LCD_D3=0;
	if(common&0x10) LCD_D4=1; else LCD_D4=0;
	if(common&0x20) LCD_D5=1; else LCD_D5=0;
	if(common&0x40) LCD_D6=1; else LCD_D6=0;
	if(common&0x80) LCD_D7=1; else LCD_D7=0;
	
	if(common&0x0100) LCD_D8=1; 	else LCD_D8=0;
	if(common&0x0200) LCD_D9=1; 	else LCD_D9=0;
	if(common&0x0400) LCD_D10=1; else LCD_D10=0;
	if(common&0x0800) LCD_D11=1; else LCD_D11=0;
	if(common&0x1000) LCD_D12=1; else LCD_D12=0;
	if(common&0x2000) LCD_D13=1; else LCD_D13=0;
	if(common&0x4000) LCD_D14=1; else LCD_D14=0;
	if(common&0x8000) LCD_D15=1; else LCD_D15=0;
	i=10;
	LCD_WR=1;
	LCD_CS=1;
	
}






void lcd_init(void)
{
	lcd_port_init( );
	
	
		//ili9341初始化
		//************* Reset LCD Driver ****************//
	//复位信号接到我们单片机的复位引脚
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
	//LCD驱动器退出睡眠状态
	LCD_ILI9341_CMD(0x29); // Display on
	
    lcd_clear(0xffff);//把LCD清成白屏
    
	LCD_BL=1;//开启背光
	
}



void lcd_clear(u16 color)
{
	u32 i;
	LCD_ILI9341_CMD(0X2A);  //定位X坐标
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0);
	LCD_ILI9341_Parameter (239&0xff);
	
	LCD_ILI9341_CMD(0X2B);  //定位Y坐标
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (319>>8);
	LCD_ILI9341_Parameter (319&0xff);
	
	LCD_ILI9341_CMD(0X2C);
	for(i=0;i<320*240;i++)
	{
		lcd_send_data(color);
	}
}




void drow_point(u16 x,u16 y)
{

	LCD_ILI9341_CMD(0X2A);  //定位X坐标
	LCD_ILI9341_Parameter (x>>8);
	LCD_ILI9341_Parameter (x&0xff);
	
	LCD_ILI9341_CMD(0X2B);  //定位Y坐标
	LCD_ILI9341_Parameter (y>>8);
	LCD_ILI9341_Parameter (y&0xff);

	
	LCD_ILI9341_CMD(0X2C);
	lcd_send_data(POINT_COLOR);
	
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
		//画点 
		drow_point(uRow,uCol);
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
		drow_point(x0+a,y0-b);             //5
 		drow_point(x0+b,y0-a);             //0           
		drow_point(x0+b,y0+a);             //4               
		drow_point(x0+a,y0+b);             //6 
		drow_point(x0-a,y0+b);             //1       
 		drow_point(x0-b,y0+a);             
		drow_point(x0-a,y0-b);             //2             
  	drow_point(x0-b,y0-a);             //7     	         
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

//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}


//mode=0，非叠加   mode=1;
void lcd_show_ascii(u16 x, u16 y, u8 _data,u8 mode)
{
	u8 i,j,a; 
	u16 color=POINT_COLOR;
	u16 x0=x;   //保存X的起始位置 
	_data=_data-' ';   //得到待显示字符在字库中处于第几行
	for(i=0;i<16;i++)
	{
		a=ascii[_data][i];
		for(j=0;j<8;j++)
		{
			if(a&0x80)
			{
				drow_point(x,y); 
			}
			else
			{
				if(mode==0)
				{
					POINT_COLOR=BACK_COLOR;
					drow_point(x,y); 
					POINT_COLOR=color;
				}
			}
			x++;
			a=a<<1;
		}
		x=x0;
		y++;
		
	}

}

void lcd_show_ascii_string(u16 x,u16 y,u8 *p,u8 mode)
{
	while(*p!='\0')
	{
		lcd_show_ascii(x,y, *p++,mode);
		x=x+8;
		if(x>(239-8))
		{
			x=0;
			y=y+16;
		}
	}
	
}


//mode=0，非叠加   mode=1;
void lcd_show_hz(u16 x, u16 y, u8 num,u8 mode)
{
	u8 i,j,a; 
	u16 color=POINT_COLOR;
	u16 x0=x;   //保存X的起始位置 

	for(i=0;i<16;i++)
	{
		a=HZ[num][i];
		for(j=0;j<8;j++)
		{
			if(a&0x80)
			{
				drow_point(x,y); 
			}
			else
			{
				if(mode==0)
				{
					POINT_COLOR=BACK_COLOR;
					drow_point(x,y); 
					POINT_COLOR=color;
				}
			}
			x++;
			a=a<<1;
		}
		x=x0;
		y++;
		
	}
	
	x0=x0+8;
	x=x0;
	y=y-16;	
	for(i=16;i<32;i++)
	{
		a=HZ[num][i];
		for(j=0;j<8;j++)
		{
			if(a&0x80)
			{
				drow_point(x,y); 
			}
			else
			{
				if(mode==0)
				{
					POINT_COLOR=BACK_COLOR;
					drow_point(x,y); 
					POINT_COLOR=color;
				}
			}
			x++;
			a=a<<1;
		}
		x=x0;
		y++;	
	}
}




void fill_Rectangle(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	
	u32 i;
	LCD_ILI9341_CMD(0X2A);  //定位X坐标
	LCD_ILI9341_Parameter (x1>>8);
	LCD_ILI9341_Parameter (x1&0xff);
	LCD_ILI9341_Parameter (x2>>8);
	LCD_ILI9341_Parameter (x2&0xff);
	
	LCD_ILI9341_CMD(0X2B);  //定位Y坐标
	LCD_ILI9341_Parameter (y1>>8);
	LCD_ILI9341_Parameter (y1&0xff);
	LCD_ILI9341_Parameter (y2>>8);
	LCD_ILI9341_Parameter (y2&0xff);
	
	LCD_ILI9341_CMD(0X2C);
	for(i=0;i<(x2-x1+1)*(y2-y1+1);i++)
	{
		lcd_send_data(color);
	}
	
	
	LCD_ILI9341_CMD(0X2A);  //定位X坐标
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0);
	LCD_ILI9341_Parameter (239&0xff);
	
	LCD_ILI9341_CMD(0X2B);  //定位Y坐标
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (319>>8);
	LCD_ILI9341_Parameter (319&0xff);
	
	
}



#if 0
void lcd_show_pic(u16 x,u16 y,u16 *p)
{
	
	u32 i;
	LCD_ILI9341_CMD(0X2A);  //定位X坐标
	LCD_ILI9341_Parameter (x>>8);
	LCD_ILI9341_Parameter (x&0xff);
	LCD_ILI9341_Parameter ((x+ *(p+1)-1)>>8);
	LCD_ILI9341_Parameter ((x+ *(p+1)-1)&0xff);
	
	LCD_ILI9341_CMD(0X2B);  //定位Y坐标
	LCD_ILI9341_Parameter (y>>8);
	LCD_ILI9341_Parameter (y&0xff);
	LCD_ILI9341_Parameter ((y+ *(p+2)-1)>>8);
	LCD_ILI9341_Parameter ((y+ *(p+2)-1)&0xff);
	
	LCD_ILI9341_CMD(0X2C);
	for(i=0;i<(*(p+1))*(*(p+2));i++)
	{
		lcd_send_data(*(p+4+i));
	}
	
	
	LCD_ILI9341_CMD(0X2A);  //定位X坐标
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0);
	LCD_ILI9341_Parameter (239&0xff);
	
	LCD_ILI9341_CMD(0X2B);  //定位Y坐标
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (319>>8);
	LCD_ILI9341_Parameter (319&0xff);
	

}

#endif


