#ifndef _LCD_H_
#define _LCD_H_
#include "stm32f4xx.h" 


#define LCD_CS   	PGout(12)
#define LCD_RS   	PFout(12)
#define LCD_WR   	PDout(5)
#define LCD_RD		PDout(4)

#define LCD_D0   PDout(14)  
#define LCD_D1   PDout(15) 
#define LCD_D2    PDout(0) 
#define LCD_D3   PDout(1)  
#define LCD_D4    PEout(7) 
#define LCD_D5    PEout(8) 
#define LCD_D6    PEout(9)  
#define LCD_D7     PEout(10) 

#define LCD_D8    PEout(11)  
#define LCD_D9    PEout(12)  
#define LCD_D10    PEout(13)  
#define LCD_D11    PEout(14)  
#define LCD_D12     PEout(15) 
#define LCD_D13     PDout(8) 
#define LCD_D14      PDout(9) 
#define LCD_D15      PDout(10) 

#define LCD_BL      PBout(15)



//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			0XFFE0
#define GBLUE				 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)



extern const u8 asc_font[][16];
extern const u8 hz_font[][32];

extern  u16 point_color;
extern  u16 back_color;

void LCD_ILI9341_CMD(u8 common);
void LCD_ILI9341_Parameter(u16 _data);
void lcd_clear(u16 color);
void lcd_init(void);
void write_piont(u16 x,u16 y,u16 color);  //画点
void Draw_Circle(u16 x0,u16 y0,u8 r);     //画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);  //画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2); //画矩形

void show_ascii(u16 x,u16 y,u8 zf,u8 mode);//显示一个ASCII字符
void show_ascii_string(u16 x,u16 y,u8 *p,u8 mode);//显示一个ASCII字符串
void show_hz(u16 x,u16 y,u8 zf,u8 mode);
void lcd_show_pic(void);
#endif
