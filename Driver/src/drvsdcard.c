#ifndef __PRJ_STM32F10X_DRVSDCARD_C__
#define __PRJ_STM32F10X_DRVSDCARD_C__

//---------------------------------------------------------------------------------------------------------------------------------------------
//平    台:				STM32F10X
//文    件:    		 	drvadc.c
//作    者:       		霁风AI
//库版 本:   			Vxxx
//文件版本:   			V1.0.0
//日   期:      		2020年03月11日
//说   明:      	 	ADC驱动实现
//----------------------------------------------------------------------------------------------------------------------------------------------

#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h" 
#include "stm32f10x_rcc.h"
#include "drvsdcard.h"
#include "delay.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: sd_gpio_init
//	功能说明: SD卡gpio初始化
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-14
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void sd_gpio_init(void)
{
    GPIO_InitTypeDef gpio_config_init;

	RCC_APB2PeriphClockCmd(RCC_PCLK_SD_GPIO, ENABLE);		//开启SD CS GPIO时钟

	gpio_config_init.GPIO_Pin = SD_CS_IO;	//SD_CS_IO IO初始化
	gpio_config_init.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_SD_GPIO, &gpio_config_init);

	GPIO_SetBits(PORT_SD_GPIO, SD_CS_IO);	//IO初始状态都设置为高电平
}

#endif /* __PRJ_STM32F10X_DRVSDCARD_C__ */
