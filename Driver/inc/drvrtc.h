#ifndef __DRVRTC_H__
#define __DRVRTC_H__

//时间结构体
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;	
	
	//公历日月年周
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;		 
}_calendar;					 
extern _calendar calendar;	//日历结构体

struct SET_ALARM
{
	s8 hour;
	s8 minute;
	u8 week;
};

extern u8 hour_x;
extern u8 min_x;
extern u8 sec_x;	
//公历日月年周
extern u16 w_year_x;
extern u8  w_month_x;
extern u8  w_date_x;
extern u8  week_x;	
extern u8  g_time_flag;
    
u8 RTC_Init_LSI(void);//初始化RTC,返回0,失败;1,成功;
u8 RTC_Init_LSE(void);
u8 Is_Leap_Year(u16 year);//平年,闰年判断
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //更新时间   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间	
void  get_time(void);

#endif

