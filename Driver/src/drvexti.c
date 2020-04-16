#ifndef __PRJ_STM32F10X_DRVEXTI_C__
#define __PRJ_STM32F10X_DRVEXTI_C__

#include "Typedef.h"
#include "drvexti.h"
#include "stm32f10x_conf.h"

void exti_config(void)
{
	EXTI_InitTypeDef exti_init_config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//外部中断，需要使能AFIO时钟
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);	//PC5设置为外部中断
	
	exti_init_config.EXTI_Line		= EXTI_Line5;
  	exti_init_config.EXTI_Mode 		= EXTI_Mode_Interrupt;	
  	exti_init_config.EXTI_Trigger 	= EXTI_Trigger_Falling;//下降沿触发
  	exti_init_config.EXTI_LineCmd 	= ENABLE;
  	EXTI_Init(&exti_init_config);	 	//根据EXTI_InitTypeDef中指定的参数初始化外设EXTI寄存器
}


#endif	//__PRJ_STM32F10X_DRVEXTI_C__
