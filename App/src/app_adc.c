#ifndef __PRJ_STM32F10X_APP_ADC_C__
#define __PRJ_STM32F10X_APP_ADC_C__

#include <stdio.h>
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "declares.h"
#include "Typedef.h"
#include "app_adc.h"
#include "drvadc.h"
#include "delay.h"
#include "usart.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: App_AdcTest
//	功能说明: ADC单通道采样测试
//	形    参: 无
//	返 回 值: 无
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void App_AdcTest(void)
{
	uint16_t usAdcVal = 0;
	float fTmpVal = 0.0;
	
	usAdcVal = Get_AdcAverage(ADC_Channel_1, 10);
//	LCD_ShowxNum(156,130,adcx,4,16,0);//显示ADC的值
	
	//INPUT VOLTAGE = (ADC Value / ADC Resolution) * Reference Voltage
	fTmpVal = (float)usAdcVal * (3.3 / 4096);	//12位ADC
	printf("voltage is %04fv.\r\n", fTmpVal);
	
//	usAdcVal = fTmpVal;	//整数部分
//	printf("Voltage integer of usAdcVal is :%d \r\n", usAdcVal);
//	LCD_ShowxNum(156,150,adcx,1,16,0);//显示电压值
	
//	fTmpVal -= usAdcVal;	//小数部分
//	printf("Voltage float of is :%f \r\n",fTmpVal);
//	fTmpVal *= 1000;
//	LCD_ShowxNum(172, 150, fTmpVal, 3, 16, 0X80);
	
	delay_ms(250);	
}


#endif	//__PRJ_STM32F10X_APP_ADC_C__
