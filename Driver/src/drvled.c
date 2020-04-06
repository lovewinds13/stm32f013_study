//-----------------------------------------------------------------------------------------------------------
//
//	模块名称 : BSP模块(For STM32F103RC)
//	文件名称 : bsp.c
//	版    本 : V1.0
//	说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
//			   bsp = Borad surport packet 板级支持包
//	修改记录 :
//		版本号  日期         作者       说明
//		V1.0    2019-11-24  霁风AI   正式发布
//
//	Copyright (C), 2018-2030, 微信公众号 TECHTIMES
//
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
//	正点原子F103RC开发板LED口线分配：
//		LED0     : PA8          (低电平点亮，高电平熄灭)
//		LED1     : PD2          (低电平点亮，高电平熄灭)
//------------------------------------------------------------------------------------------------------------

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "drvled.h"
#include "delay.h"

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void Bsp_LedInit(void)
{
	GPIO_InitTypeDef  Gpio_StInit;

	RCC_APB2PeriphClockCmd(LED_GPIO_CLK_ALLENABLE, ENABLE);	 //使能PA,PD端口时钟

	Gpio_StInit.GPIO_Pin 	= LED0_CON_IO;				 //LED0-->PA.8 端口配置
	Gpio_StInit.GPIO_Mode 	= GPIO_Mode_Out_PP; 		 //推挽输出
	Gpio_StInit.GPIO_Speed 	= GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(LED0_GPIO_PORT, &Gpio_StInit);					 	

	Gpio_StInit.GPIO_Pin 	= LED1_CON_IO;	    		 //LED1-->PD.2 端口配置, 推挽输出
	Gpio_StInit.GPIO_Mode 	= GPIO_Mode_Out_PP; 		 //推挽输出
	Gpio_StInit.GPIO_Speed 	= GPIO_Speed_50MHz;		//推挽输出 ，IO口速度为50MHz
	GPIO_Init(LED1_GPIO_PORT, &Gpio_StInit);	  				
	
	GPIO_SetBits(LED0_GPIO_PORT, LED0_CON_IO);
	GPIO_SetBits(LED1_GPIO_PORT, LED1_CON_IO); 						 
}

//-------------------------------------------------------------------------------------------------------
//	函 数 名: bsp_LedOn
//	功能说明: 点亮指定的LED指示灯,灌电流点亮
//	形    参:  _no : 指示灯序号，范围 0 - 1
//	返 回 值: 无
//-------------------------------------------------------------------------------------------------------
void Bsp_LedOn(uint8_t _no)
{	
	if (_no == 0)
	{
		 GPIO_ResetBits(LED0_GPIO_PORT, LED0_CON_IO);
	}
	else if (_no == 1)
	{
		 GPIO_ResetBits(LED1_GPIO_PORT, LED1_CON_IO);
	}
}

//-------------------------------------------------------------------------------------------------------
//	函 数 名: Bsp_LedOff
//	功能说明: 关闭指定的LED指示灯
//	形    参:  _no : 指示灯序号，范围 0 - 1
//	返 回 值: 无
//-------------------------------------------------------------------------------------------------------
void Bsp_LedOff(uint8_t _no)
{
	if (_no == 0)
	{
		 GPIO_SetBits(LED0_GPIO_PORT, LED0_CON_IO);
	}
	else if (_no == 1)
	{
		 GPIO_SetBits(LED1_GPIO_PORT, LED1_CON_IO);
	}
}

//-------------------------------------------------------------------------------------------------------
//	函 数 名: bsp_LedToggle
//	功能说明: 翻转指定的LED指示灯。
//	形    参:  _no : 指示灯序号，范围 0 - 1
//	返 回 值: 无
//-------------------------------------------------------------------------------------------------------
void Bsp_LedToggle(uint8_t _no)
{
	if (_no == 0)
	{
		LED0_GPIO_PORT->ODR ^= LED0_CON_IO;
	}
	else if (_no == 1)
	{
		LED1_GPIO_PORT->ODR ^= LED1_CON_IO;
	}
}

//-------------------------------------------------------------------------------------------------------
//	函 数 名: Bsp_LedTest
//	功能说明: LED灯驱动测试。
//	形    参:  _uiTime : 延时时间
//	返 回 值: 无
//-------------------------------------------------------------------------------------------------------
void Bsp_LedTest(uint16_t _uiTime)
{
	Bsp_LedToggle(0);
	delay_ms(_uiTime);
	Bsp_LedToggle(1);
	delay_ms(_uiTime);
}
 
//***************************** 微信公众号 TECHTIMES (END OF FILE) *********************************/
