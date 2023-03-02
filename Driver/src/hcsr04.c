#include "hcsr04.h"
#include "delay.h"


TIM_ICInitTypeDef  tim_ic_init;
//g_cap_state:捕获状态标志(bit7:完成捕获; bit6:捕获高电平;bit5-bit0:捕获高电平溢出次数)
u16 g_cap_state, g_cap_val;
u32 g_cap_distance;	//超声波测量距离

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: timer4_cap_init
//	功能说明: TIM4初始化
//	形    参: timer_arr：自动重装值; timer_psc：时钟分频系数
//	返 回 值: 无
//	日    期: 
//	作    者: 
//  备    注: 
//---------------------------------------------------------------------------------------------------------------------------------------------
void timer4_cap_init(u16 timer_arr, u16 timer_psc)
{
	GPIO_InitTypeDef gp_init;
	TIM_TimeBaseInitTypeDef  tim_base_init;
	NVIC_InitTypeDef nvic_init_config;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟

	gp_init.GPIO_Pin  	= GPIO_Pin_8; 
	gp_init.GPIO_Mode 	= GPIO_Mode_IPD; //PB8 输入 (ECHO) 
	GPIO_Init(GPIOB, &gp_init);

	gp_init.GPIO_Pin  	= GPIO_Pin_9;     
	gp_init.GPIO_Mode 	= GPIO_Mode_Out_PP;     //PB9 输出 (TRIG)
	gp_init.GPIO_Speed 	= GPIO_Speed_2MHz;     //2M
	GPIO_Init(GPIOB, &gp_init);

	//初始化定时器4 TIM4	 
	tim_base_init.TIM_Period 		= timer_arr; //设定计数器自动重装值 
	tim_base_init.TIM_Prescaler 	= timer_psc; 	//预分频器   
	tim_base_init.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	tim_base_init.TIM_CounterMode 	= TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &tim_base_init); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM4输入捕获参数
	tim_ic_init.TIM_Channel 	= TIM_Channel_3; //CC1S=03 	选择输入端 IC3映射到TI1上
	tim_ic_init.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	//上升沿捕获
	tim_ic_init.TIM_ICSelection = TIM_ICSelection_DirectTI;
	tim_ic_init.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	tim_ic_init.TIM_ICFilter 	= 0x00;//配置输入滤波器 不滤波
	TIM_ICInit(TIM4, &tim_ic_init);

	//中断分组初始化
	nvic_init_config.NVIC_IRQChannel 					= TIM4_IRQn;  //TIM4中断
	nvic_init_config.NVIC_IRQChannelPreemptionPriority 	= 3;  //先占优先级2级
	nvic_init_config.NVIC_IRQChannelSubPriority 		= 3;  //从优先级0级
	nvic_init_config.NVIC_IRQChannelCmd 				= ENABLE; //IRQ通道被使能
	NVIC_Init(&nvic_init_config);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 	
	
	TIM_ITConfig(TIM4, TIM_IT_Update|TIM_IT_CC3, ENABLE);//允许更新中断 ,允许CC3IE捕获中断	
	TIM_Cmd(TIM4, ENABLE); 	//使能定时器3
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: hcsr04_read_distance
//	功能说明: 超声波测量距离
//	形    参: 无
//	返 回 值: 无
//	日    期: 
//	作    者: 
//  备    注: 
//---------------------------------------------------------------------------------------------------------------------------------------------
void hcsr04_read_distance(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);	//启动超声波测量
	delay_us(15);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	
	if (g_cap_state & 0X80)	//捕获一次高电平
	{
		g_cap_distance = g_cap_state & 0X3f;
		g_cap_distance *= 65535;
		g_cap_distance += g_cap_val;
		g_cap_distance = g_cap_distance * 170 / 1000;	//计算距离(mm)
		g_cap_state = 0X00;
	}
}

void TIM4_IRQHandler(void)
{
	u16 sta_val;
	
	sta_val = TIM4->SR;
	if ((g_cap_state & 0X80) == 0X00)	//未捕获
	{
		if (sta_val & 0X01)	//溢出
		{
			if (g_cap_state & 0X40)	//捕获到高电平
			{
				if ((g_cap_state & 0X3f) == 0X3f)	//高电平时间过长
				{
					g_cap_state |= 0X80;	//记录一次捕获
					g_cap_val = 0Xffff;	//溢出时间
				}
				else 
				{
					g_cap_state++;	//继续捕获
				}
			}
		}
		
		if (sta_val & 0X08) //捕获3(通道3)发生捕获事件
		{
			if (g_cap_state & 0X40)	//捕获下降沿
			{
				g_cap_state |= 0X80;	//标记成功捕获一次高电平
				g_cap_val = TIM4->CCR3;	//获取当前捕获值
				TIM4->CCER &= ~(1<<9);	//CC1P=0 设置为上升沿捕获
			}
			else 
			{
				g_cap_state = 0;
				g_cap_val = 0;
				g_cap_state |= 0X40;	//标记捕获到上升沿
				TIM4->CNT = 0;	//清空计数器
				TIM4->CCER |= (1<<9);	//CC1P=1 设置为下降沿捕获
			}
		}
	}
	
	TIM4->SR = 0;	//清除中断标志位
}
