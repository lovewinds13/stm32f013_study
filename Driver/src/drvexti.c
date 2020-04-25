#ifndef __PRJ_STM32F10X_DRVEXTI_C__
#define __PRJ_STM32F10X_DRVEXTI_C__

#include "Typedef.h"
#include "drvexti.h"
#include "drvled.h"
#include "delay.h"
#include "stm32f10x_conf.h"

volatile uint8_t g_exti_cnt = 0;

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: exti_gpio_config
//	功能说明: 外部中断GPIO参数配置
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-25
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
static void exti_gpio_config(void)
{
	GPIO_InitTypeDef gpio_init_config;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟
	
	gpio_init_config.GPIO_Pin  		= GPIO_Pin_0;//PA0
	gpio_init_config.GPIO_Mode 		= GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &gpio_init_config);//初始化GPIOA.0
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: exti_config
//	功能说明: 外部中断参数配置
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-25
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
static void exti_config(void)
{
	EXTI_InitTypeDef exti_init_config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//外部中断，需要使能AFIO时钟
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//PA0设置为外部中断
	
	exti_init_config.EXTI_Line		= EXTI_Line0;
  	exti_init_config.EXTI_Mode 		= EXTI_Mode_Interrupt;	
  	exti_init_config.EXTI_Trigger 	= EXTI_Trigger_Rising;//上升沿触发
  	exti_init_config.EXTI_LineCmd 	= ENABLE;
  	
	EXTI_Init(&exti_init_config);	 	//根据EXTI_InitTypeDef中指定的参数初始化外设EXTI寄存器
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: exit_nvic_config
//	功能说明: 中断向量参数配置
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-25
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
static void exti_nvic_config(void)
{
	NVIC_InitTypeDef nvic_init_config;
	
	nvic_init_config.NVIC_IRQChannel 					= EXTI0_IRQn;//使能按键所在的外部中断通道
	nvic_init_config.NVIC_IRQChannelPreemptionPriority 	= 0x02;//抢占优先级2
	nvic_init_config.NVIC_IRQChannelSubPriority 		= 0x02;//子优先级1
	nvic_init_config.NVIC_IRQChannelCmd 				= ENABLE;
	
	NVIC_Init(&nvic_init_config);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: exti_init
//	功能说明: 外部中断初始化
//	形    参: 	exti_no：中断号
//	返 回 值: 无
//	日    期: 2020-04-25
//  备    注: 外部调用此函数,实现EXTI的初始化配置
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void exti_init(uint8_t exti_no)
{
	if (exti_no == 0)
	{
		exti_gpio_config();
		exti_config();
		exti_nvic_config();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: EXTI0_IRQHandler
//	功能说明: 外部中断0服务函数
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-25
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)  
	{
		g_exti_cnt++;
		
		if (g_exti_cnt % 2)
		{
			Bsp_LedOn(0);	//点亮OLED0
		}
		else 
		{
			Bsp_LedOff(0);	//熄灭OLED0
		}
		
		if (g_exti_cnt > 200)
		{
			g_exti_cnt = 0;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line0);	//清除EXTI0线路挂起
	}
}

#endif	//__PRJ_STM32F10X_DRVEXTI_C__
