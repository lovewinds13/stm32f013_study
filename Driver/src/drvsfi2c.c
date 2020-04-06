//--------------------------------------------------------------------------------------------------------
//
//	模块名称 : 软件模拟I2C时序
//	文件名称 : drvsfi2c.c
//	版    本 : V1.0
//	说    明 : 
//				(1) 
//				(2) 
//				(3) 
//				(4) 
//
//	修改记录 :
//		版本号     日期        作者     说明
//		V1.0    2019-12-29  霁风AI  正式发布
//		V1.1    
//		V1.2	
//		V1.3	
//
//	Copyright (C), 2020-2030, 微信公众号——TECHTIMES
//
//--------------------------------------------------------------------------------------------------------


#ifndef __PRJ_STM32F10X_DRVSFI2C_C__
#define __PRJ_STM32F10X_DRVSFI2C_C__

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "declares.h"
#include "Typedef.h"
#include "drvsfi2c.h"//芯片GPIO定义
#include "Variable.h"
#include "delay.h"

#ifdef SOFT_I2C_COMM

St_I2cInfo StI2cInfo;

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_GpioInit
//	功能说明: 软件模拟I2C IO初始化
//	形    参: 无
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2c_GpioInit(void)
{
	GPIO_InitTypeDef gpio_config_init;
	
	RCC_APB2PeriphClockCmd(RCC_PCLK_I2C_GPIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//开启SWD，失能JTAG
	
	gpio_config_init.GPIO_Pin 	= I2C_SCL_IO | I2C_SDA_IO;
	gpio_config_init.GPIO_Speed = GPIO_Speed_2MHz;
	gpio_config_init.GPIO_Mode 	= GPIO_Mode_Out_OD;
	GPIO_Init(PORT_I2C_SCL, &gpio_config_init);
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2C_SDASetInput
//	功能说明: I2C读取数据方向设置为输入(如果开漏,无需切换)
//	形    参: 无
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2C_SDASetInput(void)
{
	GPIO_InitTypeDef gpio_config_init;
	
	gpio_config_init.GPIO_Pin 	= I2C_SDA_IO;
	gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_config_init.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(PORT_I2C_SDA, &gpio_config_init);
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2C_SDASetOutput
//	功能说明: I2C读取数据方向设置为输出
//	形    参: 无
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2C_SDASetOutput(void)
{
	GPIO_InitTypeDef gpio_config_init;
	
	gpio_config_init.GPIO_Pin = I2C_SDA_IO;
	gpio_config_init.GPIO_Speed = GPIO_Speed_2MHz;
	gpio_config_init.GPIO_Mode = GPIO_Mode_Out_OD;	//开漏输出
	GPIO_Init(PORT_I2C_SDA, &gpio_config_init);
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_CommSpeedSet
//	功能说明: I2C通信速度控制
//	形    参: _uiSpeed:延时时长(us)
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2c_CommSpeedSet(uint16_t _usSpeed)
{
	StI2cInfo.uiI2cSpeed = _usSpeed;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_Init
//	功能说明: I2C初始化
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-15
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2c_Init(void)
{
	I2c_GpioInit();
	I2c_CommSpeedSet(4);
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_Delay
//	功能说明: I2C延时
//	形    参: _usTime:延时时长(us)
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2c_Delay(uint16_t _usTime)
{
	Sys_delay_us(_usTime);
}
	
//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_Start
//	功能说明: I2C起始信号
//	形    参: 无
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2c_Start(void)
{
	I2C_SDA_1();						//拉高SDA线
	I2C_SCL_1();						//拉高SCL线
	I2c_Delay(StI2cInfo.uiI2cSpeed);					//延时，速度控制
	
	I2C_SDA_0();						//当SCL线为高时，SDA线一个下降沿代表开始信号
	I2c_Delay(StI2cInfo.uiI2cSpeed);					//延时，速度控制
	I2C_SCL_0();	
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_Stop
//	功能说明: I2C停止信号
//	形    参: 无
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2c_Stop(void)
{
	I2C_SDA_0();
	I2C_SCL_0();
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	
	I2C_SCL_1();
	I2C_SDA_1();
	I2c_Delay(StI2cInfo.uiI2cSpeed);
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_SendOneByte
//	功能说明: I2C发送一个字节数据
//	形    参: 
//				_ucData:发送的一字节数据
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2c_SendOneByte(uint8_t _ucData)
{
	uint8_t ucCnt = 0;
	
	I2C_SDASetOutput();	//SDA设置为输出(若IO为开漏,无需进行方向切换)
	for(ucCnt = 0; ucCnt < 8; ucCnt++)
	{
		I2C_SCL_0();		//SCL低电平,允许数据改变
		I2c_Delay(StI2cInfo.uiI2cSpeed);
		
		if(_ucData & 0x80)		//从高位开始传输
		{
			I2C_SDA_1();		
		}
		else
		{
			I2C_SDA_0();		
		}
		
		_ucData <<= 1;
		I2c_Delay(StI2cInfo.uiI2cSpeed);
		
		I2C_SCL_1();		//数据稳定,发送给从机
		I2c_Delay(StI2cInfo.uiI2cSpeed);
	}
	I2C_SCL_0();		//第9个时钟,SCL低电平,等待应答信号来到
	I2c_Delay(StI2cInfo.uiI2cSpeed);
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_RecvOneByte
//	功能说明: I2C接收一个字节数据
//	形    参: 
//				_ucAck:应答判断(0:给出应答;1:给出非应答)
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint8_t I2c_RecvOneByte(uint8_t _ucAck)
{
	uint8_t ucCnt = 0, ucData = 0;
	
	I2C_SCL_0();
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	
	I2C_SDA_1();
	
	I2C_SDASetInput();		//切换SDA传输方向
	
	for(ucCnt = 0; ucCnt < 8; ucCnt++)
	{
		I2C_SCL_1();		//SCL高电平时SDA上的数据达到稳定
		I2c_Delay(StI2cInfo.uiI2cSpeed);		//延时等待信号稳定
		
		ucData <<= 1;
		if(I2C_SDA_READ)
		{
			ucData |= 0x01;
		}
		else
		{
			ucData &= 0xfe;		
		}
		I2C_SCL_0();		//允许数据改变
		I2c_Delay(StI2cInfo.uiI2cSpeed);
	}
	I2C_SDASetOutput();
	if(_ucAck)
	{
		I2c_GetNack();
	}
	else
	{
		I2c_GetAck();
	}
	
	return ucData;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_Wait_Ack
//	功能说明: I2C等待应答
//	形    参: 
//				_usErrTime:超时时间设置
//	返 回 值: 无
//	日    期: 2019-12-29
//  备    注：注意I2c_Stop(),此函数实现与#else与区别
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
#if 0
uint8_t I2c_WaitAck(uint16_t _usErrTime)
{	
	I2C_SDASetInput();      //SDA设置为输入,开漏时无需设置方向,OD 是双向的IO  

	I2C_SDA_1();
	I2c_Delay(StI2cInfo.uiI2cSpeed);	   
	I2C_SCL_1();
	I2c_Delay(StI2cInfo.uiI2cSpeed);	 
	while (I2C_SDA_READ)
	{
//		_usErrTime++;
		if(_usErrTime--)
		{
			I2c_Stop();
			return I2C_NACK;
		}
	}
	I2C_SCL_0();//时钟拉低,继续收发数据  
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	
	return I2C_ACK;  
} 

#else
//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_Wait_Ack
//	功能说明: I2C等待应答
//	形    参: 
//				_usErrTime:超时时间设置(此函数无作用,仅是为了和#if里面公用声明)
//	返 回 值: 无
//	日    期: 2019-12-29
//  备    注：
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint8_t I2c_WaitAck(uint16_t _usErrTime)
{
	uint8_t ucAck = 0xFF;
	
	I2C_SDA_1();
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	I2C_SCL_1();		//此时判断是否有应答
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	
//	if(I2C_SDA_READ)
	while (I2C_SDA_READ && (_usErrTime--))	//增加一个时长等待,确保应答与非应答信号的稳定
	{
		ucAck = I2C_NACK;	
	}
//	else
	{
		ucAck = I2C_ACK;	
	}
	
	I2C_SCL_0();
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	
	return ucAck;
}
#endif

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_GetAck
//	功能说明: I2C得到应答
//	形    参: 无
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2c_GetAck(void)
{
	I2C_SCL_0();
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	I2C_SDASetOutput();
	I2C_SDA_0();	//第九个时钟,SDA为低应答
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	I2C_SCL_1();		//SCL高电平,高电平时读取SDA的数据
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	I2C_SCL_0();
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	I2C_SDA_1();		//释放SDA
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: I2c_GetNack
//	功能说明: I2C得到非应答()
//	形    参: 无
//	返 回 值: 无
//	日    期: 2019-12-29
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void I2c_GetNack(void)
{
	I2C_SCL_0();
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	I2C_SDASetOutput();
	I2C_SDA_1();	//第九个时钟,SDA为高非应答
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	I2C_SCL_1();		//SCCL高电平,高电平时读取SDA的数据
	I2c_Delay(StI2cInfo.uiI2cSpeed);
	I2C_SCL_0();
	I2c_Delay(StI2cInfo.uiI2cSpeed);
}

#endif	//SOFT_I2C_COMM

#endif /* __PRJ_STM32F10X_DRVSFI2C_C__ */
