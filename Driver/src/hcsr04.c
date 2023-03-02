#include "hcsr04.h"
#include "delay.h"


TIM_ICInitTypeDef  tim_ic_init;
//g_cap_state:����״̬��־(bit7:��ɲ���; bit6:����ߵ�ƽ;bit5-bit0:����ߵ�ƽ�������)
u16 g_cap_state, g_cap_val;
u32 g_cap_distance;	//��������������

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: timer4_cap_init
//	����˵��: TIM4��ʼ��
//	��    ��: timer_arr���Զ���װֵ; timer_psc��ʱ�ӷ�Ƶϵ��
//	�� �� ֵ: ��
//	��    ��: 
//	��    ��: 
//  ��    ע: 
//---------------------------------------------------------------------------------------------------------------------------------------------
void timer4_cap_init(u16 timer_arr, u16 timer_psc)
{
	GPIO_InitTypeDef gp_init;
	TIM_TimeBaseInitTypeDef  tim_base_init;
	NVIC_InitTypeDef nvic_init_config;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ��TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��

	gp_init.GPIO_Pin  	= GPIO_Pin_8; 
	gp_init.GPIO_Mode 	= GPIO_Mode_IPD; //PB8 ���� (ECHO) 
	GPIO_Init(GPIOB, &gp_init);

	gp_init.GPIO_Pin  	= GPIO_Pin_9;     
	gp_init.GPIO_Mode 	= GPIO_Mode_Out_PP;     //PB9 ��� (TRIG)
	gp_init.GPIO_Speed 	= GPIO_Speed_2MHz;     //2M
	GPIO_Init(GPIOB, &gp_init);

	//��ʼ����ʱ��4 TIM4	 
	tim_base_init.TIM_Period 		= timer_arr; //�趨�������Զ���װֵ 
	tim_base_init.TIM_Prescaler 	= timer_psc; 	//Ԥ��Ƶ��   
	tim_base_init.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	tim_base_init.TIM_CounterMode 	= TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &tim_base_init); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��ʼ��TIM4���벶�����
	tim_ic_init.TIM_Channel 	= TIM_Channel_3; //CC1S=03 	ѡ������� IC3ӳ�䵽TI1��
	tim_ic_init.TIM_ICPolarity 	= TIM_ICPolarity_Rising;	//�����ز���
	tim_ic_init.TIM_ICSelection = TIM_ICSelection_DirectTI;
	tim_ic_init.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	tim_ic_init.TIM_ICFilter 	= 0x00;//���������˲��� ���˲�
	TIM_ICInit(TIM4, &tim_ic_init);

	//�жϷ����ʼ��
	nvic_init_config.NVIC_IRQChannel 					= TIM4_IRQn;  //TIM4�ж�
	nvic_init_config.NVIC_IRQChannelPreemptionPriority 	= 3;  //��ռ���ȼ�2��
	nvic_init_config.NVIC_IRQChannelSubPriority 		= 3;  //�����ȼ�0��
	nvic_init_config.NVIC_IRQChannelCmd 				= ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&nvic_init_config);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 	
	
	TIM_ITConfig(TIM4, TIM_IT_Update|TIM_IT_CC3, ENABLE);//��������ж� ,����CC3IE�����ж�	
	TIM_Cmd(TIM4, ENABLE); 	//ʹ�ܶ�ʱ��3
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	�� �� ��: hcsr04_read_distance
//	����˵��: ��������������
//	��    ��: ��
//	�� �� ֵ: ��
//	��    ��: 
//	��    ��: 
//  ��    ע: 
//---------------------------------------------------------------------------------------------------------------------------------------------
void hcsr04_read_distance(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);	//��������������
	delay_us(15);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	
	if (g_cap_state & 0X80)	//����һ�θߵ�ƽ
	{
		g_cap_distance = g_cap_state & 0X3f;
		g_cap_distance *= 65535;
		g_cap_distance += g_cap_val;
		g_cap_distance = g_cap_distance * 170 / 1000;	//�������(mm)
		g_cap_state = 0X00;
	}
}

void TIM4_IRQHandler(void)
{
	u16 sta_val;
	
	sta_val = TIM4->SR;
	if ((g_cap_state & 0X80) == 0X00)	//δ����
	{
		if (sta_val & 0X01)	//���
		{
			if (g_cap_state & 0X40)	//���񵽸ߵ�ƽ
			{
				if ((g_cap_state & 0X3f) == 0X3f)	//�ߵ�ƽʱ�����
				{
					g_cap_state |= 0X80;	//��¼һ�β���
					g_cap_val = 0Xffff;	//���ʱ��
				}
				else 
				{
					g_cap_state++;	//��������
				}
			}
		}
		
		if (sta_val & 0X08) //����3(ͨ��3)���������¼�
		{
			if (g_cap_state & 0X40)	//�����½���
			{
				g_cap_state |= 0X80;	//��ǳɹ�����һ�θߵ�ƽ
				g_cap_val = TIM4->CCR3;	//��ȡ��ǰ����ֵ
				TIM4->CCER &= ~(1<<9);	//CC1P=0 ����Ϊ�����ز���
			}
			else 
			{
				g_cap_state = 0;
				g_cap_val = 0;
				g_cap_state |= 0X40;	//��ǲ���������
				TIM4->CNT = 0;	//��ռ�����
				TIM4->CCER |= (1<<9);	//CC1P=1 ����Ϊ�½��ز���
			}
		}
	}
	
	TIM4->SR = 0;	//����жϱ�־λ
}
