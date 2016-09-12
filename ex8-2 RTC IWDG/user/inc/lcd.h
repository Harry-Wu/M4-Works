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



//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)



extern const u8 asc_font[][16];
extern const u8 hz_font[][32];

extern  u16 point_color;
extern  u16 back_color;

void LCD_ILI9341_CMD(u8 common);
void LCD_ILI9341_Parameter(u16 _data);
void lcd_clear(u16 color);
void lcd_init(void);
void write_piont(u16 x,u16 y,u16 color);  //����
void Draw_Circle(u16 x0,u16 y0,u8 r);     //��Բ
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);  //����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2); //������

void show_ascii(u16 x,u16 y,u8 zf,u8 mode);//��ʾһ��ASCII�ַ�
void show_ascii_string(u16 x,u16 y,u8 *p,u8 mode);//��ʾһ��ASCII�ַ���
void show_hz(u16 x,u16 y,u8 zf,u8 mode);
void lcd_show_pic(void);
#endif
