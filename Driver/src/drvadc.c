#ifndef __PRJ_STM32F10X_DRVADC_C__
#define __PRJ_STM32F10X_DRVADC_C__

//---------------------------------------------------------------------------------------------------------------------------------------------
//平    台:				STM32F10X
//文    件:    		 	drvadc.c
//作    者:       		霁风AI
//库版 本:   			Vxxx
//文件版本:   			V1.0.0
//日   期:      		2020年03月11日
//说   明:      	 	ADC驱动实现
//----------------------------------------------------------------------------------------------------------------------------------------------

#include "Typedef.h"
#include "drvadc.h"
#include "delay.h"
#include "stm32f10x_conf.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Adc_GpioConfig
//	功能说明: ADC输入采集引脚配置
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-11
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void Adc_GpioConfig(void)
{	
	GPIO_InitTypeDef gpio_init;
	
	RCC_APB2PeriphClockCmd(RCC_PCLK_ADC_GPIO, ENABLE );	  //使能ADC1通道时钟
	
	//PA1 作为模拟通道输入引脚                         
	gpio_init.GPIO_Pin = ADC_IN_PIN;	//ADC输入引脚
	gpio_init.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(PORT_ADC_IN, &gpio_init);	
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Adc_Config
//	功能说明: ADC输入采集功能配置
//	形    参: 无
//	返 回 值: 无
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void  Adc_Config(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_PCLK_ADC_CHL, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	ADC_DeInit(ADC_CHL);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode 					= ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode 			= DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode 	= DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign 			= ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel			= 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC_CHL, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	ADC_Cmd(ADC_CHL, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC_CHL);	//使能复位校准  
	while(ADC_GetResetCalibrationStatus(ADC_CHL));	//等待复位校准结束
	
	ADC_StartCalibration(ADC_CHL);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC_CHL));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Adc_Init
//	功能说明: ADC初始化
//	形    参: 无
//	返 回 值: 无
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void Adc_Init(void)
{
	Adc_GpioConfig();	//IO初始化
	Adc_Config();	//ADC功能配置
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Get_AdcConvertVal
//	功能说明: 输出ADC转换结果
//	形    参: 	_ucChl：ADC 采集通道
//	返 回 值: 采集数据结果
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint16_t Get_AdcConvertVal(uint8_t _ucChl)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC_CHL, _ucChl, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC_CHL, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC_CHL, ADC_FLAG_EOC));	//等待转换结束

	return ADC_GetConversionValue(ADC_CHL);		//返回最近一次ADC1规则组的转换结果
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Get_Adc_Average
//	功能说明: 转换出 ADC 采集数据
//	形    参: 	_ucChl：ADC 通道号
//				_ucTimes：采集次数
//	返 回 值: 转换数据
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint16_t Get_AdcAverage(uint8_t _ucChl, uint8_t _ucTimes)
{
	uint32_t ulTmpVal = 0;
	uint8_t i = 0;
	
	for(i = 0; i < _ucTimes; i++)
	{
		ulTmpVal += Get_AdcConvertVal(_ucChl);
		delay_ms(5);
	}
	return ulTmpVal / _ucTimes;
} 	

#endif /* __PRJ_STM32F10X_DRVADC_C__ */
