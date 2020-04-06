//--------------------------------------------------------------------------------------------------------
//
//	模块名称 : SPI Flash测试程序
//	文件名称 : app_flash.c
//	版    本 : V1.0
//	说    明 : 
//				(1) 调用 drvexflash.c 驱动接口
//				(2) 
//				(3) 
//				(4) 
//
//	修改记录 :
//		版本号     日期        作者     说明
//		V1.0    2020-03-07   霁风AI  正式发布
//		V1.1    
//		V1.2	
//		V1.3	
//
//	Copyright (C), 2020-2030, 微信公众号——TECHTIMES
//
//--------------------------------------------------------------------------------------------------------


#ifndef __PRJ_STM32F10X_APP_FLASH_C__
#define __PRJ_STM32F10X_APP_FLASH_C__

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "Typedef.h"
#include "declares.h"
#include "Variable.h"
#include "app_flash.h"
#include "drvexflash.h"
#include "delay.h"

#define FALSH_TEST_ADDR		(0x100)

__align(4) uint16_t g_WriteReadcnt = 0;
__align(4) uint8_t g_WriteData[0x100] = {0};
__align(4) uint16_t g_Count = 0;
__align(4) uint8_t g_ucPos = 0;

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_TestDataInit
//	功能说明: FLASH 测试数据初始化(0x00-0xFF交替写入FLASH)
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-07
//  备    注: 
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_TestDataInit(void)
{
	u8 usMinData = 0;
	u8 usMaxData = 0xFF;
	u16 usCnt = 0;

    for(usCnt = 0; usCnt < 0x100 ; usCnt++)
	{
		if(g_WriteReadcnt % 2)
		{
			g_WriteData[usCnt] = usMinData;
			usMinData++;		
		}
		else
		{
			g_WriteData[usCnt] = usMaxData;
			usMaxData--;
		}	
		
	}
}


#ifdef SOFT_SPI_COMM	//软件模拟SPI
//--------------------------------------------------------------------------------------------------------
//	函 数 名: W25_CircleWriteRead
//	功能说明: FLASH 按页写入读出,遍历整个芯片
//	形    参: 	_ucFlashSize：FLASH页数
//				
//	返 回 值: 无
//	日    期: 2020-03-07
//  备    注: 
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void W25_CircleWriteRead(uint8_t _ucFlashSize,uint32_t _ulAddr, uint8_t *ucpWBuf, uint8_t *ucpRBuf, uint16_t _usLen)
{
    uint16_t usPageSum = 0;	//flash的页数
    uint16_t usCnt = 0;

	switch (_ucFlashSize)
	{
		case FLASH_SIZE_1MBYTE:
					usPageSum = MAX_PAGE_1MBYTE;
					break;
		case FLASH_SIZE_2MBYTE:
					usPageSum = MAX_PAGE_2MBYTE;
					break;
		case FLASH_SIZE_4MBYTE:
					usPageSum = MAX_PAGE_4MBYTE;
					break;
		case FLASH_SIZE_8MBYTE:
					usPageSum = MAX_PAGE_8MBYTE;
		default:
					break;
	}

    printf("Write Data -> \r\n");
	for(usCnt = 0; usCnt < _usLen; usCnt++)
	{
		 printf("0x%02X ", ucpWBuf[usCnt]);	
	}
	printf("\r\n");
	Flash_EraseChip();	//第一次整片擦除
	
	{
		printf("Flash erase is ok! \r\n");
		
		Flash_ReadSomeBytes(ucpRBuf, 0x00, 256);
		
		printf("Data is after flash erase: \r\n");
		for(uint16_t usCnt = 0; usCnt < _usLen ; usCnt++)
		{
			printf("0x%02X ",ucpRBuf[usCnt]);
			
			if (ucpRBuf[usCnt] == 0xFF)	//验证擦除后的数据
			{
//				printf("Flash erase verify is ok! \r\n");
			}
			else 
			{
				printf("Flash erase is failed! \r\n");
				while (1);
			}
		}
		printf("\r\n");
		printf("Flash erase verify is ok! \r\n");
	}
	
    for (uint16_t i = 0; i < usPageSum; i++)
    {
		printf("\r\n");
		printf("************************---------********************************** \r\n");
        printf("Flash Addr is 0x%08x \r\n",_ulAddr);
        Flash_WriteSomeBytes(ucpWBuf, _ulAddr, _usLen);	//写 256 Byte 数据
//		Flash_WritePage(ucpWBuf, _ulAddr, _usLen);
        printf("Write Page %d -> \r\n",i);
		for(usCnt = 0; usCnt < _usLen ; usCnt++)
		{
			printf("0x%02X ", ucpWBuf[usCnt]);	
		}
		printf("\r\n");
		Sys_delay_ms(500);
        Flash_ReadSomeBytes(ucpRBuf, _ulAddr, _usLen);	//read 256 byte data 
        printf("Read Page %d -> \r\n",i);
		for(usCnt = 0; usCnt < _usLen; usCnt++)
		{
			 printf("0x%02X ",ucpRBuf[usCnt]);	
		}
        	printf("\r\n");
        _ulAddr += 0x100;	//地址增加一页

		for(usCnt = 0; usCnt < _usLen ; usCnt++)
		{
			if(ucpWBuf[usCnt] != ucpRBuf[usCnt])	//写入与读出数据作比较
			{
				printf("Flash Write != Read failed!\r\n");
				printf("Check Please!\r\n");
				while (1);
			}	
		}
		printf("Flash Write == Read sucessed!\r\n");
		printf("Flash g_WriteReadcnt is  Running %d \r\n", g_WriteReadcnt);

		if(i >= (usPageSum-1))
		{
			i = 0;
			g_WriteReadcnt ++;
			Flash_TestDataInit();
			printf("Flash 16M Bits is Test Sucessed! \r\n");
			printf("Flash g_WriteReadcnt is %d \r\n", g_WriteReadcnt);
			printf("Flash will go to Next Test\r\n");
			Flash_EraseChip();	//写完整个芯片,擦除一次
			Sys_delay_ms(500);
		}
		printf("************************@@@@@@@@@********************************** \r\n");
		printf("\r\n");
    } 
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: W25_WriteReadBytes
//	功能说明: SPI FLASH 读写测试
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-06
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void W25_WriteReadBytes(void)
{
	uint8_t ucWriteBuf[10] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
	uint8_t ucReadBuf[10] = {0};
	
	if (g_Count)
	{
		if (g_ucPos > 250)
		{
			g_Count = 0;
			g_ucPos = 0;
		}
		
		memset(ucWriteBuf, 0, 10);
		memcpy(ucWriteBuf, g_WriteData + g_ucPos, 10);
		g_ucPos += 10;
	}
	
	Flash_WriteSomeBytes((uint8_t *)ucWriteBuf, FALSH_TEST_ADDR, 10);
	
//	Flash_EraseSector(0);
//	Flash_WriteNoCheck(ucWriteBuf, FALSH_TEST_ADDR, 10);
	printf("Write Data is ---->\r\n");
	for (uint16_t i = 0; i < 10; i++)
	{
		printf("Write Data is  % d ----> 0x%02X \r\n", i, ucWriteBuf[i]);
	}
	printf("\r\n");
	
	Sys_delay_ms(500);
	Flash_ReadSomeBytes(ucReadBuf, FALSH_TEST_ADDR, 10);	//检测是否擦除
	printf("Read Data is ---->\r\n");
	for (uint16_t i = 0; i < 10; i++)
	{
		printf("Read Data is  % d ----> 0x%02X \r\n", i, ucReadBuf[i]);
	}
	printf("Read Data is Over\r\n");
	printf("\r\n");
	
	for (uint16_t i = 0; i < 10; i++)
	{
		if(ucWriteBuf[i] != ucReadBuf[i])	//写入与读出数据作比较
		{
			printf("Flash Write != Read failed!\r\n");
			printf("Check please!\r\n");
			while (1);
		}
	}
	
	printf("W25 operate is ok! \r\n");
	
	memset(ucReadBuf, 0, 10);
	g_Count++;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: W25_EraseTest
//	功能说明: SPI FLASH 擦除测试
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-06
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void W25_EraseTest(void)
{
	uint16_t i = 0;
	uint8_t ucaTmpBuf[0x100] = {0};
	
	Flash_EraseChip();	//整片擦除
	Sys_delay_ms(500);	//擦除之后需要延时
	Flash_ReadSomeBytes(ucaTmpBuf, 0x00, 0x100);	//读取第一页数据验证
	
	for (i = 0; i < 256; i++)
	{
		printf("0x%02X ", ucaTmpBuf[i]);
			
		if (ucaTmpBuf[i] == 0xFF)	//验证擦除后的数据
		{
//				printf("Flash erase verify is ok! \r\n");
		}
		else 
		{
			printf("\r\n");
			printf("Flash erase is failed! \r\n");
			while (1);
		}
	}
	printf("\r\n");
	printf("Flash erase verify is ok! \r\n");
	memset(ucaTmpBuf, 0, 0x100);
}

#endif	//SOFT_SPI_COMM

#endif	//__PRJ_STM32F10X_APP_FLASH_C__
