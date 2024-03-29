//https://github.com/J-CHUN/STM32F103-RTC


#include "stm32f10x_conf.h"
#include "typedef.h"
#include "drvrtc.h"
#include "delay.h"
#include "drvuart.h"
#include <stdio.h>
#include <string.h>


//获取系统时间
const char *pt = __TIME__;  //20:15:05
const char *pd = __DATE__;  //Dec 30 2021

u8 hour_x;
u8 min_x;
u8 sec_x;	
//公历日月年周
u16 w_year_x;
u8  w_month_x;
u8  w_date_x;
u8  week_x;	
u8  g_time_flag = 0;	

//月份
char *g_month_buf[13] = {"NUL","Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

struct SET_ALARM alarm;

_calendar calendar;//时钟结构体


static void RTC_NVIC_Config(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}
/*******************************************************************************
* 函 数 名         : RTC_Init_LSI
* 函数功能		   : RTC初始化
* 输    入         : 无
* 输    出         : 0,初始化成功
        			 1,LSI开启失败
解决复位之后RTC_WaitForSynchro();卡死问题：此句在if外面开启时钟，RCC_LSICmd(ENABLE); 
内部晶振低速时钟40KHZ
注意：使用内部低速时钟断电后无法继续走时，即使有备用电池也不行
LSI需由主电源VDD供电，而VBAT只能使LSE起振。
*******************************************************************************/
u8 RTC_Init_LSI(void)
{
	//检查是不是第一次配置时钟
	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟
	PWR_BackupAccessCmd(ENABLE); //使能后备寄存器访问
	RCC_LSICmd(ENABLE); //设置内部低速晶振（LSI）
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050) //从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{
		BKP_DeInit(); //复位备份区域
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET&& temp < 250) //检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			temp++;
			delay_ms(10);
		}
		if(temp>=250)return 1;//初始化时钟失败,晶振有问题
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI); //设置RTC时钟(RTCCLK),选择LSI作为RTC时钟
		RCC_RTCCLKCmd(ENABLE); //使能RTC时钟
		RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro(); //等待RTC寄存器同步
		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE); //使能RTC秒中断、闹钟中断
		RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置
		//RTC_SetPrescaler(32767); //设置RTC预分频的值
		RTC_SetPrescaler(40000); //设置RTC预分频的值
		RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成

		//RTC_Set(2017,3,6,0,0,0); //设置时间
		get_time();  //获取编译时间
		RTC_Set(calendar.w_year+2000-1 ,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);  //设置时间
		
		RTC_ExitConfigMode(); //退出配置模式
		BKP_WriteBackupRegister(BKP_DR1, 0X5050); //向指定的后备寄存器中写入用户程序数据
	}
	else//系统继续计时
	{
		RTC_WaitForSynchro(); //等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE); //使能RTC秒中断、闹钟中断
		RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成
	}
	RTC_NVIC_Config();//RCT中断分组设置
	RTC_Get();//更新时间
	return 0; //ok
}

/*******************************************************************************
* 函 数 名         : RTC_Init_LSE
* 函数功能		   : RTC初始化
* 输    入         : 无
* 输    出         : 0,初始化成功
        			 1,LSE开启失败
外部低速时钟
注意：使用外部低速时钟断电后如果有备用电池可以继续走时
*******************************************************************************/

u8 RTC_Init_LSE(void)
{
	//检查是不是第一次配置时钟
	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP外设时钟
	PWR_BackupAccessCmd(ENABLE); //使能后备寄存器访问
		
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5080) //从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{
		BKP_DeInit(); //复位备份区域
		RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&& temp < 250) //检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			temp++;
			delay_ms(10);
		}
		if(temp>=250)return 1;//初始化时钟失败,晶振有问题
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //设置RTC时钟(RTCCLK),选择LSE作为RTC时钟
		RCC_RTCCLKCmd(ENABLE); //使能RTC时钟
		RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro(); //等待RTC寄存器同步
		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE); //使能RTC秒中断、闹钟中断
		RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置
		 RTC_SetPrescaler(32767); //设置RTC预分频的值
		RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成

		//RTC_Set(2017,3,6,0,0,0); //设置时间
		get_time();  //获取编译时间
		RTC_Set(calendar.w_year+2000-1 ,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);  //设置时间
		
		RTC_ExitConfigMode(); //退出配置模式
		BKP_WriteBackupRegister(BKP_DR1, 0X5080); //向指定的后备寄存器中写入用户程序数据
	}
	else//系统继续计时
	{
		RTC_WaitForSynchro(); //等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE); //使能RTC秒中断、闹钟中断
		RTC_WaitForLastTask(); //等待最近一次对RTC寄存器的写操作完成
	}
	RTC_NVIC_Config();//RCT中断分组设置
	RTC_Get();//更新时间
	return 0; //ok
}

//RTC时钟中断
//每秒触发一次  
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
	{							
		RTC_Get();//更新时间
		g_time_flag= 1;
		hour_x=calendar.hour;
		min_x=calendar.min;
		sec_x=calendar.sec;	
		w_year_x=calendar.w_year;;
		w_month_x=calendar.w_month;
		w_date_x=calendar.w_date;	
//		printf("RTC Time:20%d-%d-%d %d:%d:%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);//输出闹铃时间	
				
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	  	
		RTC_Get();				//更新时间   
		printf("Alarm Time:%d-%d-%d %d:%d:%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);//输出闹铃时间	
		
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//清闹钟中断
	RTC_WaitForLastTask();	  	    						 	   	 
}
//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   

//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/*******************************************************************************
* 函 数 名         : RTC_Set
* 函数功能		   : RTC设置日期时间函数（以1970年1月1日为基准，把输入的时钟转换为秒钟）
						1970~2099年为合法年份
* 输    入         : syear：年  smon：月  sday：日
					hour：时   min：分	 sec：秒			
* 输    出         : 0,成功
        			 1,失败
*******************************************************************************/
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
    seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	RTC_SetCounter(seccount);	//设置RTC计数器的值

	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	return 0;	    
}

//初始化闹钟		  
//以1970年1月1日为基准
//1970~2099年为合法年份
//syear,smon,sday,hour,min,sec：闹钟的年月日时分秒   
//返回值:0,成功;其他:错误代码.
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
    seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去 			    
	//设置时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	//上面三步是必须的!
	RTC_SetAlarm(seccount);
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	
	return 0;	    
}

//得到当前的时间
//返回值:0,成功;其他:错误代码.
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
    timecount=RTC_GetCounter();	 
 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		calendar.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//得到月份
		calendar.w_date=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	calendar.hour=temp/3600;     	//小时
	calendar.min=(temp%3600)/60; 	//分钟	
	calendar.sec=(temp%3600)%60; 	//秒钟
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//获取星期   
	return 0;
}	

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日 
//返回值：星期号																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}			  

void get_time(void)
{
	u8 i;
	u8 tmp_str[4] = "   ";
	//Dec  3 2021
	if( pd[4] == ' ' )
	{
		calendar.w_date = pd[5]-'0';//得到日
	}
	else   //Dec 30 2021
	{
		calendar.w_date= (pd[4]-'0')*10 + (pd[5]-'0');//得到日
	}
	
	calendar.w_year = (pd[9]-'0')*10 + (pd[10]-'0');//得到年	
	//printf("年:%d\r\n",calendar.w_year );
	tmp_str[0] = pd[0];
	tmp_str[1] = pd[1];
	tmp_str[2] = pd[2];
	for(i = 1; i <= 12; i++)
	{
		if(strcmp((const char *)tmp_str, (char *)g_month_buf[i]) == 0)
		{
			break;	//找到月份了
		}
//		printf("strcmp((const char *)pd:%c%c%c \r\n", pd[0],pd[1],pd[2]);
//		printf("strcmp((const char *)pd:%s \r\n", tmp_str);
	}
	calendar.w_month = i;//得到月 

	calendar.hour = (pt[0]-'0')*10 + (pt[1]-'0');//得到小时
	calendar.min = (pt[3]-'0')*10 + (pt[4]-'0');//得到分钟
	calendar.sec = (pt[6]-'0')*10 + (pt[7]-'0');//得到秒
}
