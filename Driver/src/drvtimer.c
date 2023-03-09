#ifndef __PRJ_STM32F10X_DRVTIMER_C__
#define __PRJ_STM32F10X_DRVTIMER_C__

#include "Variable.h"
#include "Typedef.h"
#include "drvtimer.h"
#include "drvled.h"
#include "stm32f10x_conf.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: timer_config
//	����˵��: ��ʱ����������
//	��    ��: 	timer_no����ʱ�����
//				timer_arr���Զ���װֵ
//				timer_psc��ʱ�ӷ�Ƶϵ��
//	�� �� ֵ: ��
//	��    ��: 2020-04-14
//  ��    ע: 
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
static void timer_config(uint8_t timer_no, uint16_t timer_arr, uint16_t timer_psc)
{
	TIM_TimeBaseInitTypeDef timer_init_config;
	
	if (timer_no == 3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		
		timer_init_config.TIM_Period 				= timer_arr;	//�Զ���װ�ؼĴ�����ֵ
		timer_init_config.TIM_Prescaler 			= timer_psc;	//ʱ��Ƶ�ʳ���Ԥ��Ƶ��ֵ
		timer_init_config.TIM_ClockDivision 		= TIM_CKD_DIV1;	//ʱ�ӷָ�
		timer_init_config.TIM_CounterMode 			= TIM_CounterMode_Up;	//���ϼ�����ʽ
		//timer_init_config.TIM_RepetitionCounter 	= 0x0000;//�߼���ʱ��ʹ��
		TIM_TimeBaseInit(TIM3, &timer_init_config);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: timer_nvic_config
//	����˵��: ��ʱ��NVIC��������
//	��    ��: 	timer_no����ʱ�����
//	�� �� ֵ: ��
//	��    ��: 2020-04-14
//  ��    ע: 
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
static void timer_nvic_config(uint8_t timer_no)
{
	NVIC_InitTypeDef nvic_init_config;
	
	if (timer_no == 3)
	{
		//�ж����ȼ� NVIC ����
		nvic_init_config.NVIC_IRQChannel 						= TIM3_IRQn; //TIM3 �ж�
		nvic_init_config.NVIC_IRQChannelPreemptionPriority 		= 2; //��ռ���ȼ� 2 ��
		nvic_init_config.NVIC_IRQChannelSubPriority 			= 3; //�����ȼ� 3 ��
		nvic_init_config.NVIC_IRQChannelCmd 					= ENABLE; //IRQ ͨ����ʹ��
		NVIC_Init(&nvic_init_config); //�ܳ�ʼ�� NVIC �Ĵ���
		
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//��������ж�
	}
}	

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: timer_init
//	����˵��: ��ʱ����ʼ������
//	��    ��: 	timer_no����ʱ�����
//	�� �� ֵ: ��
//	��    ��: 2020-04-14
//  ��    ע: 
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void timer_init(uint8_t timer_no)
{
	if (timer_no == 3)
	{
		timer_config(timer_no, 4999, 7199);//��������5000 = 500ms;����Ƶ��:72M/7200=10KHz
		timer_nvic_config(timer_no);
		
		TIM_Cmd(TIM3, ENABLE);//������ʱ��
	}
	
	g_timer3_tick = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: TIM3_IRQHandler
//	����˵��: ��ʱ��3�жϷ�����
//	��    ��: ��
//	�� �� ֵ: ��
//	��    ��: 2020-04-14
//  ��    ע: 
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
#if 1
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
#endif

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

