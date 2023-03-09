#ifndef __PRJ_STM32F10X_DRVFREQ_C__
#define __PRJ_STM32F10X_DRVFREQ_C__

#include "drvfreq.h"
#include "logic_grammer.h"
#include "drvuart.h"
#include "stdio.h"


#define FREQ_TEST_NUM	(10)

uint32_t g_over_time =0;	//�������

uint32_t g_cnt_value = 0;	//ʱ����ʱ

uint32_t g_timer_cap_value = 0;	//����ʱ��������

uint16_t g_cap_value1 = 0, g_cap_value2 = 0;	//���β���ֵ����

uint8_t TIM_CAP_STA = 0;	//����״̬����(8λ)

uint8_t g_freq_flag = 0;	//(����Ƶ�ʷ�Χ����)

uint8_t g_cap_cnt = 0;	//����Ĵ���

uint32_t g_freq_buff[10] = {0};

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: freq_gpio_input_config
//	����˵��: Ƶ������ GPIO ��������
//	��    ��: ��
//	�� �� ֵ: ��
//	��    ��: 2020-07-21
//  ��    ע: 
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void freq_gpio_input_config(void)
{
	GPIO_InitTypeDef gpio_config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��Ƶ������GPIOA�˿�ʱ��
	
	gpio_config.GPIO_Pin	= FREQ_INPUT_IO;	//����GPIOA PA0
	gpio_config.GPIO_Mode	= GPIO_Mode_IPD;	//PA0 ����Ϊ��������
	
	GPIO_Init(FREQ_GPIO_PORT, &gpio_config);	//��ʼ��GPIO����
	
	GPIO_ResetBits(FREQ_GPIO_PORT, FREQ_INPUT_IO);	//���� PA0 ��ʼ״̬Ϊ�͵�ƽ
	
	return;
}
	
//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: timer_counter_config
//	����˵��: ��ʱ����������
//	��    ��: 	timer_no����ʱ�����
//				timer_arr����ʱ����װֵ
//				timer_psc����ʱ����Ƶֵ
//	�� �� ֵ: ��
//	��    ��: 2020-07-21
//  ��    ע: 
//	��    ��: by ����AI
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
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��2��ʱ��
			
			TIM_DeInit(TIM2);	//��λ��ʱ��2�Ĳ���ֵ
			
			timer_init.TIM_Period			= timer_arr;	//������װ��ֵ
			timer_init.TIM_Prescaler		= timer_psc;	//���÷�Ƶ��ֵ
			timer_init.TIM_ClockDivision	= TIM_CKD_DIV1;	//ʱ�ӷָ�
			timer_init.TIM_CounterMode		= TIM_CounterMode_Up;	//������ʽ�����ϼ���
			
			TIM_TimeBaseInit(TIM2, &timer_init);	//��ʼ����ʱ��2�Ĳ���
			
			TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);	//�ⲿ������ʽ
			TIM_SelectInputTrigger(TIM2, TIM_TS_ETRF);	//�ⲿ��������Դ����
			TIM_SetCounter(TIM2, 0);	//��ʱ���ļ�����ֵ����Ϊ0
			
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	//ʹ�ܶ�ʱ���ж�
			
			TIM_Cmd(TIM2, ENABLE);	//��ʱ��ʹ��
		}
		break;
		case 3:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3��ʱ��
			
			TIM_DeInit(TIM3);	//��λ��ʱ��2�Ĳ���ֵ
			
			timer_init.TIM_Period			= timer_arr;	//������װ��ֵ
			timer_init.TIM_Prescaler		= timer_psc;	//���÷�Ƶ��ֵ
			timer_init.TIM_ClockDivision	= TIM_CKD_DIV1;	//ʱ�ӷָ�
			timer_init.TIM_CounterMode		= TIM_CounterMode_Up;	//������ʽ�����ϼ���
			
			TIM_TimeBaseInit(TIM3, &timer_init);	//��ʼ����ʱ��2�Ĳ���
			
		#if 0
			TIM_ETRClockMode2Config(TIM3, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);	//�ⲿ������ʽ
			TIM_SelectInputTrigger(TIM3, TIM_TS_ETRF);	//�ⲿ��������Դ����
			TIM_SetCounter(TIM3, 0);	//��ʱ���ļ�����ֵ����Ϊ0
			
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	//ʹ�ܶ�ʱ���ж�
		#endif
			TIM_Cmd(TIM3, ENABLE);	//��ʱ��ʹ��
		}
		case 5:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ�ܶ�ʱ��5��ʱ��
			
			timer_init.TIM_Period			= timer_arr;	//������װ��ֵ
			timer_init.TIM_Prescaler		= timer_psc;	//���÷�Ƶ��ֵ
			timer_init.TIM_ClockDivision	= TIM_CKD_DIV1;	//ʱ�ӷָ�
			timer_init.TIM_CounterMode		= TIM_CounterMode_Up;	//������ʽ�����ϼ���
			
			TIM_TimeBaseInit(TIM5, &timer_init);	//��ʼ����ʱ��5�Ĳ���
			
			TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);	//TIM5��������жϺ�CC1IE�����ж�
			
		#if 0
			TIM_Cmd(TIM5, ENABLE);	//��ʱ��ʹ��
		#endif
		}
		break;
		
		default:
			break;
	}
	return;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: timer_counter_nvic_config
//	����˵��: ��ʱ���ж�������������
//	��    ��: 	timer_no����ʱ�����
//	�� �� ֵ: ��
//	��    ��: 2020-07-21
//  ��    ע: 
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void timer_counter_nvic_config(uint8_t timer_no)
{
	uint8_t temp_value = timer_no;
	
	NVIC_InitTypeDef nvic_config;
	
	if (temp_value == 2)
	{
		nvic_config.NVIC_IRQChannel						= TIM2_IRQn;	//TIM2�ж�
		nvic_config.NVIC_IRQChannelPreemptionPriority	= 0;	//��ռ���ȼ�
		nvic_config.NVIC_IRQChannelSubPriority			= 0;	//�����ȼ�
		nvic_config.NVIC_IRQChannelCmd					= ENABLE;	//ʹ���ж�ͨ��
		
		NVIC_Init(&nvic_config);	//��ʼ��NVIC���ò���
	}
	else if (temp_value == 3)
	{
		nvic_config.NVIC_IRQChannel						= TIM3_IRQn;	//TIM3�ж�
		nvic_config.NVIC_IRQChannelPreemptionPriority	= 0;	//��ռ���ȼ�
		nvic_config.NVIC_IRQChannelSubPriority			= 1;	//�����ȼ�
		nvic_config.NVIC_IRQChannelCmd					= ENABLE;	//ʹ���ж�ͨ��
		
		NVIC_Init(&nvic_config);	//��ʼ��NVIC���ò���
	}
	else if (temp_value == 5)
	{
		nvic_config.NVIC_IRQChannel						= TIM5_IRQn;	//TIM5�ж�
		nvic_config.NVIC_IRQChannelPreemptionPriority	= 2;	//��ռ���ȼ�
		nvic_config.NVIC_IRQChannelSubPriority			= 0;	//�����ȼ�
		nvic_config.NVIC_IRQChannelCmd					= ENABLE;	//ʹ���ж�ͨ��
		
		NVIC_Init(&nvic_config);	//��ʼ��NVIC���ò���
	}
	
	return;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: timer_capture_config
//	����˵��: ��ʱ�����벶���������
//	��    ��: 	timer_no����ʱ�����
//	�� �� ֵ: ��
//	��    ��: 2020-07-22
//  ��    ע: 
//	��    ��: by ����AI
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
			timer_ic_init.TIM_Channel		= TIM_Channel_1;	//�����ѡ��IC1ӳ�䵽TI1��
			timer_ic_init.TIM_ICPolarity	= TIM_ICPolarity_Rising;	//����Ϊ�����ز���
			timer_ic_init.TIM_ICSelection	= TIM_ICSelection_DirectTI;	//ӳ�䵽TI1
			timer_ic_init.TIM_ICPrescaler	= TIM_CKD_DIV1;	//ʱ�ӷָ�
			timer_ic_init.TIM_ICFilter		= 0x00;	//�����˲���(0x00�����˲�)
			
			TIM_ICInit(TIM5, &timer_ic_init);	//��ʼ��TIM5���벶�����
		}
		break;
		default:
			break;
	}
	
	return;
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	//TIM2�����ж�
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	//����жϱ�־λ
		g_over_time++;	//���ʱ�����(16λ�������ڲ����ڼ�������ٴ�)
	}
}

#if 0
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	//TIM3�����ж�
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	//���TIM3�жϱ�־
		g_cnt_value = TIM_GetCounter(TIM2);	//�������ֵ(����ʱ��TIMx_CNT��ֵ,δ����65536����ֵ)
	}
}
#endif

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: TIM5_IRQHandler
//	����˵��: ��ʱ��5�жϷ�����
//	��    ��: ��
//	�� �� ֵ: ��
//	��    ��: 2020-07-22
//  ��    ע: ���յ�Ŀ���Ǽ�������Է���������,����Ƶ�� = 1 / ����(f = 1 / T)
//	��    ��: by ����AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void TIM5_IRQHandler(void)
{
	if ((TIM_CAP_STA & 0x80) == 0)
	{
		if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)	//��ʱ���ж�
		{
			if (TIM_CAP_STA & 0x40)	
			{
				if ((TIM_CAP_STA & 0x3F) == 0x3F)	//�ߵ�ƽʱ��̫����,�Ѿ�������Ƶķ�Χ(0x3F)
				{
					TIM_CAP_STA = 0x80;	//ֹͣ����
					g_timer_cap_value = 0;
				}
				else 	//��һ���������Ѿ�����,���ǵڶ�����δ����
				{
					TIM_CAP_STA++;
				}
			}
		}
		
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)	//�����ж�
		{
			if (TIM_CAP_STA & 0x40)	//�ڶ��������ز���
			{
				g_cap_value2 = TIM_GetCapture1(TIM5);
				
				if ((g_cap_value2 > g_cap_value1) && (TIM_CAP_STA != 0x3F))	//�ߵ�ƽʱ����0x3F�ķ�Χ��
				{
					g_timer_cap_value = g_cap_value2 - g_cap_value1;
				}
				else	//��ʱ�������,���¿�ʼ�˼���
				{
					g_timer_cap_value = 0xFFFF - g_cap_value1 + g_cap_value2;
				}
				
				TIM_CAP_STA |= 0x80;	//���β������
				g_cap_cnt++;	//�ɹ�����Ĵ���
			}
			else 	//��һ�������ز���
			{
				g_cap_value1 = TIM_GetCapture1(TIM5);
				TIM_CAP_STA |= 0x40;	//���񵽵�һ����������
				g_timer_cap_value = 0;
			}
		}
	}
	
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update | TIM_IT_CC1);	//�����Ӧ���жϱ�־λ
}

void freq_get_value(uint8_t freq_rank)
{
	uint8_t temp_value = freq_rank;
//	uint32_t freq_buff[10] = {0};
	uint8_t i = 0;
	float freq_value = 0.0;
	
	if (temp_value == 0)
	{
		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
		TIM_Cmd(TIM2, DISABLE);
		
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		TIM_Cmd(TIM3, DISABLE);
		
		freq_gpio_input_config();
		timer_counter_config(5, 0xFFFF, 17);	//����Ƶ�� = 72/17 = 4MHz
		timer_counter_nvic_config(5);
		timer_capture_config(5);
		TIM_Cmd(TIM5, ENABLE);	//��ʱ��ʹ��
		
		if (TIM_CAP_STA & 0x80)
		{
			if (g_cap_cnt < 10)	//����10��,�������
			{
				if ((g_cap_value2 > g_cap_value1) && (TIM_CAP_STA & 0x3F) != 0x3F)
				{
					g_freq_buff[g_cap_cnt] = g_cap_value2 - g_cap_value1;	//����һ��,�ݴ�һ����ֵ
				}
				else 
				{
					g_freq_buff[g_cap_cnt] = 65536 * ((TIM_CAP_STA & 0x3F) - 1) + (65536 - g_cap_value1 + g_cap_value2);
				}
			}
			else 
			{
				bubble_sort(g_freq_buff, 10);	//����
			
				for (i = 0; i < 8; i++)
				{
					g_freq_buff[0] += g_freq_buff[i+1];	//ȥ�����,��Сֵ
				}
				
				freq_value = g_freq_buff[0] / 8;	//�õ�8�β����ƽ��ֵ
				
				freq_value = 1 / (freq_value / 4000000);	//����Ƶ��(f=1/T)
				
				//��ʾ����
				//printf("freq_value = %f \r\n", freq_value);
				
				TIM_CAP_STA = 0;	//һ�β�������,��ر�����0
				g_cap_cnt = 0;
				g_cap_value1 = 0;
				g_cap_value2 = 0;
				g_timer_cap_value = 0;
			}
		}
	}
	else if (freq_rank == 1)
	{
		freq_value = g_cnt_value + g_over_time * 65536;
		
		TIM_Cmd(TIM3, DISABLE);
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
		
		TIM_Cmd(TIM2, DISABLE);
		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
		TIM_SetCounter(TIM2, 0);
		
		TIM_Cmd(TIM3, ENABLE);
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		
		TIM_Cmd(TIM2, ENABLE);
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		
		//��ʾ���ִ���
		//printf();
	}
}

#endif	//__PRJ_STM32F10X_DRVFREQ_C__
