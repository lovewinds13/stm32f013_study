#ifndef __PRJ_STM32F10X_DRVFREQ_C__
#define __PRJ_STM32F10X_DRVFREQ_C__

#include "drvfreq.h"
#include "delay.h"



//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: freq_gpio_input_config
//	功能说明: 频率输入 GPIO 参数配置
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-07-21
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void freq_gpio_input_config(void)
{
	GPIO_InitTypeDef gpio_config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能频率输入GPIOA端口时钟
	
	gpio_config.GPIO_Pin	= FREQ_INPUT_IO;	//设置GPIOA PA0
	gpio_config.GPIO_Mode	= GPIO_Mode_IPD;	//PA0 设置为下拉输入
	
	GPIO_Init(FREQ_GPIO_PORT, &gpio_config);	//初始化GPIO参数
	
	GPIO_ResetBits(FREQ_GPIO_PORT, FREQ_INPUT_IO);	//设置 PA0 初始状态为低电平
	
	return;
}
	
//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: timer_counter_config
//	功能说明: 定时器参数设置
//	形    参: 	timer_no：定时器编号
//				timer_arr：定时器重装值
//				timer_psc：定时器分频值
//	返 回 值: 无
//	日    期: 2020-07-21
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void timer_counter_config(uint8_t timer_no, uint16_t timer_arr, uint16_t timer_psc)
{
	uint8_t temp_value = timer_no;
	
	TIM_TimeBaseInitTypeDef timer_init;
	
	switch (temp_value)
	{
		case 0:
			break;
		case 1:
			break;
		case 2:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2的时钟
			
			TIM_DeInit(TIM2);	//复位定时器2的参数值
			
			timer_init.TIM_Period			= timer_arr;	//设置重装数值
			timer_init.TIM_Prescaler		= timer_psc;	//设置分频数值
			timer_init.TIM_ClockDivision	= TIM_CKD_DIV1;	//时钟分割
			timer_init.TIM_CounterMode		= TIM_CounterMode_Up;	//计数方式：向上计数
			
			TIM_TimeBaseInit(TIM2, &timer_init);	//初始化定时器2的参数
			
			TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);	//外部触发方式
			TIM_SelectInputTrigger(TIM2, TIM_TS_ETRF);	//外部触发输入源设置
			TIM_SetCounter(TIM2, 0);	//定时器的计数初值设置为0
			
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	//使能定时器中断
			
			TIM_Cmd(TIM2, ENABLE);	//定时器使能
		}
		break;
		case 3:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3的时钟
			
			TIM_DeInit(TIM3);	//复位定时器2的参数值
			
			timer_init.TIM_Period			= timer_arr;	//设置重装数值
			timer_init.TIM_Prescaler		= timer_psc;	//设置分频数值
			timer_init.TIM_ClockDivision	= TIM_CKD_DIV1;	//时钟分割
			timer_init.TIM_CounterMode		= TIM_CounterMode_Up;	//计数方式：向上计数
			
			TIM_TimeBaseInit(TIM3, &timer_init);	//初始化定时器2的参数
			
		#if 0
			TIM_ETRClockMode2Config(TIM3, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);	//外部触发方式
			TIM_SelectInputTrigger(TIM3, TIM_TS_ETRF);	//外部触发输入源设置
			TIM_SetCounter(TIM3, 0);	//定时器的计数初值设置为0
			
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	//使能定时器中断
		#endif
			TIM_Cmd(TIM3, ENABLE);	//定时器使能
		}
		
		default:
			break;
	}
	return;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: timer_counter_nvic_config
//	功能说明: 定时器中断向量参数设置
//	形    参: 	timer_no：定时器编号
//	返 回 值: 无
//	日    期: 2020-07-21
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void timer_counter_nvic_config(uint8_t timer_no)
{
	uint8_t temp_value = timer_no;
	
	NVIC_InitTypeDef nvic_config;
	
	if (temp_value == 2)
	{
		nvic_config.NVIC_IRQChannel						= TIM2_IRQn;	//TIM2中断
		nvic_config.NVIC_IRQChannelPreemptionPriority	= 0;	//抢占优先级
		nvic_config.NVIC_IRQChannelSubPriority			= 0;	//子优先级
		nvic_config.NVIC_IRQChannelCmd					= ENABLE;	//使能中断通道
		
		NVIC_Init(&nvic_config);	//初始化NVIC配置参数
	}
	else if (temp_value == 3)
	{
		nvic_config.NVIC_IRQChannel						= TIM3_IRQn;	//TIM3中断
		nvic_config.NVIC_IRQChannelPreemptionPriority	= 0;	//抢占优先级
		nvic_config.NVIC_IRQChannelSubPriority			= 1;	//子优先级
		nvic_config.NVIC_IRQChannelCmd					= ENABLE;	//使能中断通道
		
		NVIC_Init(&nvic_config);	//初始化NVIC配置参数
	}
	
	return;
}

#endif	//__PRJ_STM32F10X_DRVFREQ_C__
