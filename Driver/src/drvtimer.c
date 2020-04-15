#ifndef __PRJ_STM32F10X_DRVTIMER_C__
#define __PRJ_STM32F10X_DRVTIMER_C__

#include "Variable.h"
#include "Typedef.h"
#include "drvtimer.h"
#include "drvled.h"
#include "stm32f10x_conf.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: timer_config
//	功能说明: 定时器参数配置
//	形    参: 	timer_no：定时器编号
//				timer_arr：自动重装值
//				timer_psc：时钟分频系数
//	返 回 值: 无
//	日    期: 2020-04-14
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
static void timer_config(uint8_t timer_no, uint16_t timer_arr, uint16_t timer_psc)
{
	TIM_TimeBaseInitTypeDef timer_init_config;
	
	if (timer_no == 3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		
		timer_init_config.TIM_Period 				= timer_arr;	//自动重装载寄存器数值
		timer_init_config.TIM_Prescaler 			= timer_psc;	//时钟频率除数预分频数值
		timer_init_config.TIM_ClockDivision 		= TIM_CKD_DIV1;	//时钟分割
		timer_init_config.TIM_CounterMode 			= TIM_CounterMode_Up;	//向上计数方式
		//timer_init_config.TIM_RepetitionCounter 	= 0x0000;//高级定时器使用
		TIM_TimeBaseInit(TIM3, &timer_init_config);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: timer_nvic_config
//	功能说明: 定时器NVIC参数配置
//	形    参: 	timer_no：定时器编号
//	返 回 值: 无
//	日    期: 2020-04-14
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
static void timer_nvic_config(uint8_t timer_no)
{
	NVIC_InitTypeDef nvic_init_config;
	
	if (timer_no == 3)
	{
		//中断优先级 NVIC 设置
		nvic_init_config.NVIC_IRQChannel 						= TIM3_IRQn; //TIM3 中断
		nvic_init_config.NVIC_IRQChannelPreemptionPriority 		= 2; //先占优先级 2 级
		nvic_init_config.NVIC_IRQChannelSubPriority 			= 3; //从优先级 3 级
		nvic_init_config.NVIC_IRQChannelCmd 					= ENABLE; //IRQ 通道被使能
		NVIC_Init(&nvic_init_config); //④初始化 NVIC 寄存器
		
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//允许更新中断
	}
}	

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: timer_init
//	功能说明: 定时器初始化配置
//	形    参: 	timer_no：定时器编号
//	返 回 值: 无
//	日    期: 2020-04-14
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void timer_init(uint8_t timer_no)
{
	if (timer_no == 3)
	{
		timer_config(timer_no, 4999, 7199);//计数满载5000 = 500ms;计数频率:72M/7200=10KHz
		timer_nvic_config(timer_no);
		
		TIM_Cmd(TIM3, ENABLE);//开启定时器
	}
	
	g_timer3_tick = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: TIM3_IRQHandler
//	功能说明: 定时器3中断服务函数
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-14
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		g_timer3_tick++;
//		if ((g_timer3_tick % 20) == 0)
//		{
//		//	printf("the t vale is %d\n",t);		
//		}
	}
}

void timer_test(void)
{
	if (g_timer3_tick /= 2)
	{
		Bsp_LedOn(0);
		Bsp_LedOff(1);
	}
	else 
	{
		Bsp_LedOn(1);
		Bsp_LedOff(0);
	}
	if (g_timer3_tick > 60000)
	{
		g_timer3_tick = 0;
	}
}


#endif	//__PRJ_STM32F10X_DRVTIMER_C__

