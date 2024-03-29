//-----------------------------------------------------------------------------------------------------------
//	模块名称 : 主函数(For STM32F103RC)
//	文件名称 : main.c
//	版    本 : V1.0.0
//	说    明 : 主函数。
//			  
//	修改记录 :
//		版本号    日期         作者       说明
//		V1.0    2019-12-14    霁风AI   正式发布
//
//	Copyright (C), 2018-2030, 微信公众号 TECHTIMES
//
//------------------------------------------------------------------------------------------------------------

#include "typedef.h"
#include "declares.h"
#include "variable.h"
#include "drvexflash.h"
#include "drvnrf2401.h"
#include "drvadc.h"
#include "drvuart.h"
#include "drvsdcard.h"
#include "hardware_spi.h"
#include "delay.h"
#include "drvled.h"
#include "app_adc.h"
#include "app_flash.h"
#include "app_htu21d.h"
#include "app_at24cxx.h"
#include "drvoled.h"
#include "drvtimer.h"
#include "drvexti.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic_grammer.h"
#include "c_language_regular.h"
#include "hcsr04.h"
#include "drvrtc.h"



extern uint8_t g_WriteData[0x100];

uint32_t ulFlashIdVal = 0;

void Hardware_AllInit(void)
{
	
	Bsp_LedInit();
	

	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// 设置中断优先级分组2
	delay_init();	    	 		//延时函数初始化	  
			  			//初始化与LED连接的硬件接口
//	Usart_Init(USART1, 9600);		//串口1初始化,用于调试信息输出
	uart_init(1, 9600);
//	printf (" \r\n mcu enter main \r\n");
	
//	stack_test();
//	heap_test();
//	pack_test();
//	macro_define_test();
//	cpu_mem_endian();
	
	
#ifdef ADC_FUNC_TEST
	Adc_Init();
	App_AdcTest();
	
#endif	//ADC_FUNC_TEST
	
#ifdef TIMER_FUNC_TEST
	timer_init(3);
#endif

#ifdef EXTI_FUNC_TEST
	exti_init(0);
#endif	//EXTI_FUNC_TEST

#if SPI_COMM_MODE
	#ifdef HARD_SPI_COMM
	spi_master_init(1);
	#ifdef OLED_FUNC_TEST
	oled_init();
	oled_dis_logo();
//	oled_dis_picture(0, 0, 128, 7, bmp_buff);
//	oled_dis_num(60, 4, 1314, 4, 16);
//	oled_dis_one_char(5, 2, 'A');
//	oled_dis_one_char(12, 2, 'B');
//	oled_dis_one_char(129, 2, 'B');
//	oled_dis_chinese(33,1,0);
//	oled_dis_chinese(53,1,1);
//	oled_dis_chinese(73,1,2);
//	oled_dis_chinese(93,1,3);
//	oled_dis_chinese(33,5,4);
//	oled_dis_chinese(53,5,5);
//	oled_dis_chinese(73,5,6);
//	oled_dis_chinese(50, 3, 8);
//	oled_dis_chinese(80, 3, 9);
	#endif
	Flash_TestDataInit();
	ulFlashIdVal = spi_flash_read_id();
	printf("Hard --1 spi flash val is 0x%0X \r\n", ulFlashIdVal);
	ulFlashIdVal = spi_flash_read_id();
	printf("Hard --2 Spi flash val is 0x%0X \r\n", ulFlashIdVal);
	#endif	//HARD_SPI_COMM
#else
	#ifdef SOFT_SPI_COMM
	Spi_GpioInit();
	Flash_TestDataInit();
	
	ulFlashIdVal = Flash_ReadJEDECID();
	printf("Spi flash val is 0x%0X \r\n", ulFlashIdVal);
	
//	W25_WriteReadBytes();
//	W25_CircleWriteRead(0x01, 0x00, g_WriteData, g_DataTmpBuffer, 256);	//flash循环读写测试
	#endif	//SOFT_SPI_COMM
#endif	//SPI_COMM_MODE	

#ifdef I2C_HTU_TEST
	Htu_Init();
	ee_at24cxx_init();
#endif
}




int mainX(void)
{	
	Hardware_AllInit();
	
#ifdef DEBUG_ENABLE
	printk("Using Usart1 Output Debuging Infomation\r\n");
#endif
	
	while(1)
	{
		Sys_DelayUs(1000);
		Sys_DelayUs(1000);
		Sys_DelayUs(1000);
		Sys_DelayUs(1000);
//		Bsp_LedTest(500);
		
	#ifdef ADC_FUNC_TEST
		Adc_Init();
		App_AdcTest();
		
	#endif	//ADC_FUNC_TEST

#if SPI_COMM_MODE		
	#ifdef 	HARD_SPI_COMM
		ulFlashIdVal = Flash_ReadJEDECID();
		printf(" hard spi flash val is 0x%0X \r\n", ulFlashIdVal);
		W25_WriteReadBytes();

	#endif	//HARD_SPI_COMM	
#else	
	#ifdef SOFT_SPI_COMM	
		ulFlashIdVal = Flash_ReadJEDECID();
		printf(" soft spi flash val is 0x%0X \r\n", ulFlashIdVal);
		W25_WriteReadBytes();
	#endif	//SOFT_SPI_COM
	
#endif	//SPI_COMM_MODE

#ifdef I2C_HTU_TEST
	Htu_Display();
	ee_at24cxx_test();
#endif

#ifdef UART_FUNC_TEST
//	uart_test();
#endif

#ifdef TIMER_FUNC_TEST
	timer_test();
#endif

	printf("Main is ruinning\r\n");
//	Bsp_LedTest(300);
//	sprintf_test();	
//	strcpy_test();
//	memcmp_test();
//  strcmp_test();

		delay_ms(500);
		
//	if (USART_RX_STA)
//	{
//		printf("mcu will soft reset\r\n");
//		NVIC_SystemReset();
//		printf("mcu reset ok\r\n");
//	}
		
	}
}

//超声波测试函数
int main_hcr(void)
{
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(1, 115200 * 8);	 //串口初始化为961200
	Bsp_LedInit();		  	 //初始化与LED连接的硬件接口 
	
	timer4_cap_init(0Xffff, 72 - 1);
	
	while (1)
	{
		hcsr04_read_distance();
		delay_ms(500);
		LED0 = 1;
		delay_ms(500);
		LED0 = 0;
		
		printf("g_cap_distance = %d \r\n", g_cap_distance);	//用串口1打印输出
	}
}

//RTC测试函数
int main_rtc(void)
{
	u8 i = 0;
	
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(1, 115200 * 8);	 //串口初始化为961200
	Bsp_LedInit();		  	 //初始化与LED连接的硬件接口 

	RTC_Init_LSI(); //内部低速时钟
	while (1)
	{
		get_time();
		i++;
		if(i%100==0)
		{
			LED0 = !LED0;
		}
		delay_ms(10);
		if (g_time_flag)
		{
			g_time_flag = 0;
			printf("RTC Time:20%d-%d-%d %d:%d:%d\r\n",w_year_x, w_month_x, w_date_x, hour_x, min_x, sec_x);//输出时间	
		}
	}
}

//超声波测试函数
int main_timer(void)
{
	u8 i = 0;
	
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(1, 115200 * 8);	 //串口初始化为961200
	Bsp_LedInit();		  	 //初始化与LED连接的硬件接口 

	RTC_Init_LSI(); //内部低速时钟
	timer_init(3); //内部低速时钟
	while (1)
	{
		get_time();
		i++;
		if(i%100==0)
		{
			LED0 = !LED0;
		}
		delay_ms(10);
		if (g_timer3_tick == 10)	//5秒输出一次
		{
			g_timer3_tick = 0;
			LED1 = !LED1;
			printf("RTC Time:20%d-%d-%d %d:%d:%d\r\n",w_year_x, w_month_x, w_date_x, hour_x, min_x, sec_x);//输出时间	
		}
	}
}

int main(void)
{
//	main_rtc();
	main_timer();
}


//***************************** 微信公众号 TECHTIMES (END OF FILE) *********************************/

