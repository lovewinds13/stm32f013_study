#ifndef __PRJ_STM32F10X_HAL_SPI_C__
#define __PRJ_STM32F10X_HAL_SPI_C__

#include "Typedef.h"
#include "declares.h"
#include "variable.h"
#include "hal_spi.h"
#include "hardware_spi.h"
#include "drvsfspi.h"

#ifdef SOFT_SPI_COMM
#ifdef HARD_SPI_COMM

//--------------------------------------------------------------------------------------------------------
//	函 数 名: hal_spi_send_bytes
//	功能说明: SPI 发送数据,包含软件和硬件通信方式
//	形    参: 	mode：通信方式选择(0：软件SPI;1：硬件SPI)
//				pbdata：发送数据的首地址
//				send_length：发送数据长度
//	返 回 值: 执行状态(true or false)
//	日    期: 2020-03-12
//  备    注: 中间层封装底层接口
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
bool hal_spi_send_bytes(uint8_t mode, uint8_t *pbdata, uint16_t send_length)
{
    if (mode == 0)
    {
        for (uint16_t i = 0; i < send_length; i++)
        {
            Spi_WriteByte(pbdata[i]);
        }
		
		return true;
    }
    else if (mode == 1)
    {
        spi_master_send_some_bytes(1, pbdata, send_length);
		
//		for (uint16_t i = 0; i < send_length; i++)
//        {
//            spi_master_send_recv_byte(1, pbdata[i]);
//        }
		
		return true;
    }
	else 
	{
		return false;
	}
    
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: hal_spi_recv_bytes
//	功能说明: SPI 接收数据,包含软件和硬件通信方式
//	形    参: 	mode：通信方式选择(0：软件SPI;1：硬件SPI)
//				pbdata：发送数据的首地址
//				send_length：发送数据长度
//	返 回 值: 执行状态(true or false)
//	日    期: 2020-03-12
//  备    注: 中间层封装底层接口
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
bool hal_spi_recv_bytes(uint8_t mode, uint8_t *pbdata, uint16_t recv_length)
{
    if (mode == 0)
    {
        for (uint16_t i = 0; i < recv_length; i++)
        {
             *pbdata++ = Spi_ReadByte();	//软件模拟SPI
        }   
		
		return true;
    }
    else if (mode == 1)
    {
        spi_master_recv_some_bytes(1, pbdata, recv_length);	//硬件SPI
		
//		for (uint16_t i = 0; i < recv_length; i++)
//        {
//            *pbdata++ = spi_master_send_recv_byte(1, 0xFF);
//        }
		
		return true;
    }
	else 
	{
		return false;
	}
    
}

#endif	// SOFT_SPI_COMM

#endif	// HARD_SPI_COMM

#endif	//__PRJ_STM32F10X_HAL_SPI_C__
