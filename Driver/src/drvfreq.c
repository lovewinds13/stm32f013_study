#ifndef __PRJ_STM32F10X_DRVFREQ_C__
#define __PRJ_STM32F10X_DRVFREQ_C__

#include "drvfreq.h"
#include "logic_grammer.h"

#define FREQ_TEST_NUM	(10)

uint32_t g_over_time =0;	//脉冲计数

uint32_t g_cnt_value = 0;	//时长定时

uint32_t g_timer_cap_value = 0;	//捕获时间间隔区间

uint16_t g_cap_value1 = 0, g_cap_value2 = 0;	//两次捕获值描述

uint8_t TIM_CAP_STA = 0;	//捕获状态描述(8位)

uint8_t g_freq_flag = 0;	//(用于频率范围区分)

uint8_t g_cap_cnt = 0;	//捕获的次数

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
		case 5:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能定时器5的时钟
			
			timer_init.TIM_Period			= timer_arr;	//设置重装数值
			timer_init.TIM_Prescaler		= timer_psc;	//设置分频数值
			timer_init.TIM_ClockDivision	= TIM_CKD_DIV1;	//时钟分割
			timer_init.TIM_CounterMode		= TIM_CounterMode_Up;	//计数方式：向上计数
			
			TIM_TimeBaseInit(TIM5, &timer_init);	//初始化定时器5的参数
			
			TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);	//TIM5允许更新中断和CC1IE捕获中断
			
		#if 0
			TIM_Cmd(TIM5, ENABLE);	//定时器使能
		#endif
		}
		break;
		
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
	else if (temp_value == 5)
	{
		nvic_config.NVIC_IRQChannel						= TIM5_IRQn;	//TIM5中断
		nvic_config.NVIC_IRQChannelPreemptionPriority	= 2;	//抢占优先级
		nvic_config.NVIC_IRQChannelSubPriority			= 0;	//子优先级
		nvic_config.NVIC_IRQChannelCmd					= ENABLE;	//使能中断通道
		
		NVIC_Init(&nvic_config);	//初始化NVIC配置参数
	}
	
	return;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: timer_capture_config
//	功能说明: 定时器输入捕获参数设置
//	形    参: 	timer_no：定时器编号
//	返 回 值: 无
//	日    期: 2020-07-22
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void timer_capture_config(uint8_t timer_no)
{
	uint8_t temp_value = timer_no;
	
	TIM_ICInitTypeDef	timer_ic_init;
	
	switch (temp_value)
	{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
		{
		
		}
		break;
		case 4:
			break;
		case 5:
		{
			timer_ic_init.TIM_Channel		= TIM_Channel_1;	//输入端选择IC1映射到TI1上
			timer_ic_init.TIM_ICPolarity	= TIM_ICPolarity_Rising;	//设置为上升沿捕获
			timer_ic_init.TIM_ICSelection	= TIM_ICSelection_DirectTI;	//映射到TI1
			timer_ic_init.TIM_ICPrescaler	= TIM_CKD_DIV1;	//时钟分割
			timer_ic_init.TIM_ICFilter		= 0x00;	//输入滤波器(0x00：不滤波)
			
			TIM_ICInit(TIM5, &timer_ic_init);	//初始化TIM5输入捕获参数
		}
		break;
		default:
			break;
	}
	
	return;
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	//TIM2更新中断
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	//清除中断标志位
		g_over_time++;	//溢出时间计数(16位计数器在捕获期间计满多少次)
	}
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	//TIM3更新中断
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	//清除TIM3中断标志
		g_cnt_value = TIM_GetCounter(TIM2);	//捕获计数值(捕获时刻TIMx_CNT的值,未计满65536的数值)
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: TIM5_IRQHandler
//	功能说明: 定时器5中断服务函数
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-07-22
//  备    注: 最终的目的是计算出测试方波的周期,进而频率 = 1 / 周期(f = 1 / T)
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void TIM5_IRQHandler(void)
{
	if ((TIM_CAP_STA & 0x80) == 0)
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)	//定时器中断
		{
			if (TIM_CAP_STA & 0x40)	
			{
				if ((TIM_CAP_STA & 0x3F) == 0x3F)	//高电平时间太长了,已经超出设计的范围(0x3F)
				{
					TIM_CAP_STA = 0x80;	//停止捕获
					g_timer_cap_value = 0;
				}
				else 	//第一个上升沿已经捕获,但是第二个还未捕获
				{
					TIM_CAP_STA++;
				}
			}
		}
		
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)	//捕获中断
		{
			if (TIM_CAP_STA & 0x40)	//第二次上升沿捕获
			{
				g_cap_value2 = TIM_GetCapture1(TIM5);
				
				if ((g_cap_value2 > g_cap_value1) && (TIM_CAP_STA != 0x3F))	//高电平时长在0x3F的范围内
				{
					g_timer_cap_value = g_cap_value2 - g_cap_value1;
				}
				else	//定时器溢出了,重新开始了计数
				{
					g_timer_cap_value = 0xFFFF - g_cap_value1 + g_cap_value2;
				}
				
				TIM_CAP_STA |= 0x80;	//两次捕获完成
				g_cap_cnt++;	//成功捕获的次数
			}
			else 	//第一次上升沿捕获
			{
				g_cap_value1 = TIM_GetCapture1(TIM5);
				TIM_CAP_STA |= 0x40;	//捕获到第一个上升沿了
				g_timer_cap_value = 0;
			}
		}
	}
	
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update | TIM_IT_CC1);	//清除对应的中断标志位
}

void freq_get_value(uint8_t freq_rank)
{
	uint8_t temp_value = freq_rank;
	uint32_t freq_buff[10] = {0};
	uint8_t i = 0;
	
	if (temp_value == 0)
	{
		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
		TIM_Cmd(TIM2, DISABLE);
		
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		TIM_Cmd(TIM3, DISABLE);
		
		freq_gpio_input_config();
		timer_counter_config(5, 0xFFFF, 17);
		timer_counter_nvic_config(5);
		timer_capture_config(5);
		TIM_Cmd(TIM5, ENABLE);	//定时器使能
		
		if (TIM_CAP_STA & 0x80)
		{
			if (g_cap_cnt < 10)
			{
				if ((g_cap_value2 > g_cap_value1) && (TIM_CAP_STA & 0x3F) != 0x3F)
				{
					freq_buff[g_cap_cnt] = g_timer_cap_value;	//捕获一次,暂存一次数值
				}
			}
		}
	}
}

#endif	//__PRJ_STM32F10X_DRVFREQ_C__
