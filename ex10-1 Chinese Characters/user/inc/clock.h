#ifndef _CLOCK_H_
#define _CLOCK_H_
#include "rtc.h"

#define PI 3.14159
#define CLOCK_X   120  //闹钟的中心点
#define CLOCK_Y   160
#define CLOCK_R   83

extern Week_Alarm_TYPEDEF week_alam;  //变量保存设置的alarm A数据

void display_init(void);
void display_time(void);

extern void showsec(float i);

#endif
