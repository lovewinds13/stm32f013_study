//	文件名称 : drvsfspi.c
//	版    本 : V1.0
//	说    明 : 
//				(1) 初始版本,软件模拟SPI通信,包含模式0和模式3
//				(2) 
//				(3) 
//				(4) 
//
//	修改记录 :
//		版本号     日期        作者     说明
//		V1.0    2020-03-05   霁风AI  正式发布
//		V1.1    
//		V1.2	
//		V1.3	
//
//	Copyright (C), 2020-2030, 
//
//--------------------------------------------------------------------------------------------------------

#ifndef __PRJ_STM32F10X_DRVSFSPI_C__
#define __PRJ_STM32F10X_DRVSFSPI_C__

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "declares.h"
#include "Typedef.h"
#include "drvsfspi.h"//芯片GPIO定义
#include "Variable.h"

#ifdef SOFT_SPI_COMM

#define _CPOL	0	//时钟极性
#define	_CPHA	0	//时钟相位

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Spi_GpioInit
//	功能说明: SPI 软件模拟IO初始化
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-05
//  备    注:注意MISO的管脚配置,浮空输入.STM32 SPI1 时钟需要关闭
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Spi_GpioInit(void)
{
	GPIO_InitTypeDef gpio_init;
	
	RCC_APB2PeriphClockCmd(RCC_PCLK_SPI1_IO, ENABLE);	//GPIOA时钟使能
	RCC_APB2PeriphClockCmd(RCC_PCLK_SPI1_HD, DISABLE);	//SPI1时钟不使能
	
	gpio_init.GPIO_Pin = FLASH_CS | SPI_CLK | SPI_MOSI;	//CS,CLK,MOSI IO 端口设置
	gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORT_SPI1_IO, &gpio_init);
	
	gpio_init.GPIO_Pin = SPI_MISO;	//MISO IO 端口设置
	gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//MISO 设置为输入
	GPIO_Init(PORT_SPI1_IO, &gpio_init);
	
#if _CPOL==0
	SPI_CLK_LOW;
#else
	SPI_CLK_HIGH;
#endif
}

//模式0:CPOL=0;CPAH=0
//模拟SPI写数据
//--------------------------------------------------------------------------------------------------------
//	函 数 名: Spi_WriteByte
//	功能说明: spi写一字节数据(模式0)
//	形    参: 	_ucByte：写入的数据
//	返 回 值: 无
//	日    期: 2020-03-07
//  备    注: 软件模拟方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
#if _CPOL==0&&_CPHA==0  
void Spi_WriteByte(uint8_t _ucByte)
{
    uint8_t ucDataLen = 0;
  
    for (ucDataLen = 0; ucDataLen < 8; ucDataLen++)
    {
        SPI_CLK_LOW;//时钟上升沿采样传输数据
				//nrf_delay_ms(1);	//延时决定了SPI的CLK速率
        if(_ucByte & 0x80)	//发送数据
		{
			SPI_MOSI_HIGH;
		} 
        else
        {
			SPI_MOSI_LOW;
		}
        SPI_CLK_HIGH;
			//	nrf_delay_ms(1);	//延时决定了SPI的CLK速率
        _ucByte <<= 1;
    }
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Spi_ReadByte
//	功能说明: spi读一字节数据(模式0)
//	形    参: 无
//	返 回 值: 	ucTmpVal：读取的数据
//	日    期: 2020-03-07
//  备    注: 软件模拟方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint8_t Spi_ReadByte(void)
{
	uint8_t ucDataLen = 0;
	uint8_t ucTmpVal = 0;

	SPI_CLK_HIGH;
	for(ucDataLen = 0; ucDataLen < 8; ucDataLen++)	
	{
		SPI_CLK_LOW;	//下降沿后读取数据
		//nrf_delay_ms(1);
		ucTmpVal <<= 1;	//最高位已经在数据线上了
		if(SPI_MISO_READ)	//
		{
			ucTmpVal |= 0x01;
		}
		else
		{
			ucTmpVal &= 0xfe;
		}
		//dat <<= 1;注:此处最高位被移除了
		SPI_CLK_HIGH;
		//nrf_delay_ms(1);
	}
	
	return ucTmpVal;
}
#endif


//--------------------------------------------------------------------------------------------------------
//	函 数 名: Spi_WriteByte
//	功能说明: spi写一字节数据(模式3:CPOL=1;CPAH=1,上升沿发送数据)
//	形    参: _ucByte：写入的数据
//	返 回 值: 无
//	日    期: 2020-03-07
//  备    注: 软件模拟方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
#if _CPOL==1&&_CPHA==1            //MODE  1  1
void Spi_WriteByte(uint8_t _ucByte)
{
    uint8_t ucDataLen = 0;
	
//	SPI_CLK_HIGH;
    for(ucDataLen = 0; ucDataLen < 8; ucDataLen++)
    {
        SPI_CLK_LOW;//时钟上升沿输出数据
				//nrf_delay_ms(1);
        if(_ucByte & 0x80)	//发送数据
		{
			SPI_MOSI_HIGH;
		} 
        else
        {
			SPI_MOSI_LOW;
		}
        SPI_CLK_HIGH;
			//	nrf_delay_ms(1);
        _ucByte <<= 1;
    }
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Spi_ReadByte
//	功能说明: spi读一字节数据(模式3:CPOL=1;CPAH=1,上升沿接收数据)
//	形    参: 无
//	返 回 值: 	ucTmpVal：读取的数据
//	日    期: 2020-03-07
//  备    注: 软件模拟方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint8_t Spi_ReadByte(void)
{
	uint8_t ucDataLen = 0;
	uint8_t ucTmpVal = 0;
	
	SPI_CLK_HIGH;
	for(ucDataLen = 0; ucDataLen < 8; ucDataLen++)	
	{
		SPI_CLK_LOW;
		//nrf_delay_ms(1);
		ucTmpVal <<= 1;
		if(SPI_MISO_READ)	
		{
			ucTmpVal |= 0x01;
		}
		else
		{
			ucTmpVal &= 0xfe;
		}
		SPI_CLK_HIGH;
	//	nrf_delay_ms(1);
	}
	SPI_CLK_HIGH;
	
	return ucTmpVal;
}
#endif

#endif	//SOFT_SPI_COMM

#endif	//__PRJ_STM32F10X_DRV_DRVSFSPI_C__
