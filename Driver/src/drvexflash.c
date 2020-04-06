//--------------------------------------------------------------------------------------------------------
//
//	模块名称 : 外挂Flash读写操作
//	文件名称 : drvexflash.c
//	版    本 : V1.0
//	说    明 : 
//				(1) drvexflash.c调用芯片Spi通信接口
//				(2) drvexflash.c对外挂W25X华邦Flash的读写操作
//				(3) 
//				(4) 
//
//	修改记录 :
//		版本号     日期        作者     说明
//		V1.0    2020-03-07  霁风AI  正式发布
//		V1.1    
//		V1.2	
//		V1.3	
//
//	Copyright (C), 2020-2030, 微信公众号——TECHTIMES
//
//--------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------
//说明:SPI驱动Flash(25Q16)
//W25X16芯片容量：2MB (16Mbit)
//          页数：16*16*32 (2M/256)
//          扇区数：16*32
//          块数：32

//2、读写操作：
//读 ------------ 一次最大读一页(256Bytes)
//写 ------------ 页
//擦除 ---------- 扇区、块、整个芯片

//3、控制和状态寄存器命令(默认:0x00)
//BIT位  7   6   5   4   3   2   1   0
//      SPR  RV  TB  BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1--忙;0--空闲)
//--------------------------------------------------------------------------------------------------------

#ifndef __PRJ_STM32F10X_DEVEXFLASH_C__
#define __PRJ_STM32F10X_DEVEXFLASH_C__

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "Typedef.h"
#include "declares.h"
#include "app_flash.h"
#include "drvexflash.h"
#include "drvsfspi.h"
#include "delay.h"
#include "Variable.h"
#include "hal_spi.h"

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------


#ifdef SOFT_SPI_COMM	//软件模拟SPI标志
// #ifdef HARD_SPI_PICOMM	//硬件SPI标志

#define SectorBuf  g_DataTmpBuffer

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteEnable
//	功能说明: 写使能,置位 WEL 位 WEL 位(WEL-->1)
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-07
//  备    注: 
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WriteEnable(void)
{
	uint8_t command = FLASH_WRITE_ENABLE_CMD;

	FLASH_CS_LOW;
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);//开启写使能
	FLASH_CS_HIGH;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteDisable
//	功能说明: 写失能,复位 WEL 位(WEL-->0)
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-07
//  备    注: 
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WriteDisable(void)
{
	uint8_t command = FLASH_WRITE_DISABLE_CMD;
	FLASH_CS_LOW;
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);
	// Spi_WriteByte(FLASH_WRITE_DISABLE_CMD);	//开启写失能 04h
	FLASH_CS_HIGH;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteSR
//	功能说明: 读状态寄存器
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-07
//  备    注: 多用于检查 BUSY 位
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint8_t Flash_ReadSR(void)
{
	uint8_t ucTmpVal = 0;
	uint8_t command = FLASH_READ_SR_CMD;

	FLASH_CS_LOW;
	
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);	//05h
	hal_spi_recv_bytes(SPI_COMM_MODE, &ucTmpVal, 1);

	// ucTmpVal = Spi_ReadByte();
	
	FLASH_CS_HIGH;
	
	return ucTmpVal;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteSR
//	功能说明: 写状态寄存器
//	形    参: 	_ucByte:写入状态寄存器的数值
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WriteSR(uint8_t _ucByte)
{
	uint8_t command = FLASH_WRITE_SR_CMD;

	Flash_WriteEnable();	
	Flash_WaitNobusy();

	FLASH_CS_LOW;
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);	//01h
	hal_spi_send_bytes(SPI_COMM_MODE, &_ucByte, 1);	//写入一个字节
	FLASH_CS_HIGH;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WaitNobusy
//	功能说明: 检查 FLASH BUSY 位状态
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 调用Flash_ReadSR(),判断状态寄存器的R0位,执行结束操作清零
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WaitNobusy(void)
{
	//FLASH_READ_SR_CMD 指令的发送,有的FLASH仅需发送一次,FLASH自动回复,有的FLASH无法自动回复,需要循环一直发送等待
	while(((Flash_ReadSR()) & 0x01)==0x01);	//等待BUSY位清空
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_FastReadByte
//	功能说明: flash 都数据(快速读取：Fast read operate at the highest poossible frequency)
//	形    参: 	ucpBuffer：数据存储区首地址
//				_ulReadAddr: 要读出Flash的首地址
//				_usNByte： 要读出的字节数(最大65535B)
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 从_ulReadAddr地址,连续读出_usNByte长度的字节
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_ReadSomeBytes(uint8_t *ucpBuffer, uint32_t _ulReadAddr, uint16_t _usNByte)
{
	uint8_t command = FLASH_READ_DATA;
	uint8_t temp_buff[3] = {0};

	temp_buff[0] = (uint8_t)(_ulReadAddr >> 16);
	temp_buff[1] = (uint8_t)(_ulReadAddr >> 8);
	temp_buff[2] = (uint8_t)(_ulReadAddr >> 0);

	FLASH_CS_LOW;
	
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[0], 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[1], 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[2], 1);

	hal_spi_recv_bytes(SPI_COMM_MODE, ucpBuffer, _usNByte);

	// Spi_WriteByte(FLASH_READ_DATA);	//连续读取数据 03h
	// Spi_WriteByte((uint8_t)(_ulReadAddr>>16));	//写入24位地址
	// Spi_WriteByte((uint8_t)(_ulReadAddr>>8));
	// Spi_WriteByte((uint8_t)(_ulReadAddr>>0));

	// while(_usNByte--)
	// {
	// 	*ucpBuffer = Spi_ReadByte();
	// 	ucpBuffer++;
	// }
	
	FLASH_CS_HIGH;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_FastReadByte
//	功能说明: flash 都数据(快速读取：Fast read operate at the highest poossible frequency)
//	形    参: 	ucpBuffer：数据存储区首地址
//				_ulReadAddr: 要读出Flash的首地址
//				_usNByte： 要读出的字节数(最大65535B)
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 从_ulReadAddr地址,连续读出_usNByte长度的字节
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_FastReadByte(uint8_t *ucpBuffer, uint32_t _ulReadAddr, uint16_t _usNByte)
{
	uint8_t command = FLASH_FASTREAD_DATA;
	uint8_t temp_buff[3] = {0};

	temp_buff[0] = (uint8_t)(_ulReadAddr >> 16);
	temp_buff[1] = (uint8_t)(_ulReadAddr >> 8);
	temp_buff[2] = (uint8_t)(_ulReadAddr >> 0);

	FLASH_CS_LOW;
	
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[0], 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[1], 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[2], 1);

	hal_spi_recv_bytes(SPI_COMM_MODE, ucpBuffer, _usNByte);
	
	// Spi_WriteByte(FLASH_FASTREAD_DATA);//快速读取数据 0bh
	// Spi_WriteByte((uint8_t)(_ulReadAddr>>16));//写入24位地址
	// Spi_WriteByte((uint8_t)(_ulReadAddr>>8));
	// Spi_WriteByte((uint8_t)(_ulReadAddr>>0));
	// Spi_WriteByte(0xFF);//等待8个时钟(dummy byte)
	// while(_usNByte--)
	// {
	// 	*ucpBuffer = Spi_ReadByte();
	// 	ucpBuffer++;
	// }
	
	FLASH_CS_HIGH;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WritePage
//	功能说明: flash 写数据(按页写入,一页256字节,写入之前FLASH地址上必须为0xFF)
//	形    参: 	ucpBuffer：数据存储区首地址
//				_ulWriteAddr: 要读写入Flash的首地址
//				_usNByte： 要写入的字节数(最大65535B = 64K 块)
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: _ulWriteAddr,连续写入_usNByte长度的字节
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WritePage(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte)
{
	uint8_t command = FLASH_WRITE_PAGE;
	uint8_t temp_buff[3] = {0};

	temp_buff[0] = (uint8_t)(_ulWriteAddr >> 16);
	temp_buff[1] = (uint8_t)(_ulWriteAddr >> 8);
	temp_buff[2] = (uint8_t)(_ulWriteAddr >> 0);
	
	Flash_WriteEnable();	//写使能
	Flash_WaitNobusy();	//等待写入结束
	
	FLASH_CS_LOW;
	
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[0], 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[1], 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[2], 1);

	hal_spi_send_bytes(SPI_COMM_MODE, ucpBuffer, _usNByte);

	// Spi_WriteByte(FLASH_WRITE_PAGE);	//02h
	// Spi_WriteByte((uint8_t)(_ulWriteAddr>>16));	//写入24位地址
	// Spi_WriteByte((uint8_t)(_ulWriteAddr>>8));
	// Spi_WriteByte((uint8_t)(_ulWriteAddr>>0));
	// while(_usNByte--)
	// {
	// 	Spi_WriteByte(*ucpBuffer);	//SPI 写入单个字节
	// 	ucpBuffer++;
	// }
	
	FLASH_CS_HIGH;
	
	Flash_WaitNobusy();	//等待写入结束
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteNoCheck
//	功能说明: flash 写数据(不带擦除,写入之前必须确保写入部分FLASH的数据全为0xFf,否则写入失败)
//	形    参: 	ucpBuffer：数据存储区首地址
//				_ulWriteAddr: 要读写入Flash的首地址
//				_usNByte： 要写入的字节数(最大65535B = 64K 块)
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: _ulWriteAddr,连续写入_usNByte长度的字节，程序带FLASH数据检查写入
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WriteNoCheck(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte)
{
	uint16_t PageByte = 256 - _ulWriteAddr % 256;//单页剩余可写字节数

	if(_usNByte <= PageByte)	//不大于256字节
	{
		PageByte = _usNByte;
	}
	
	while(1)
	{
		Flash_WritePage(ucpBuffer, _ulWriteAddr, PageByte);
		if(_usNByte == PageByte)	//写入结束
			break;
		else
		{
			ucpBuffer += PageByte;	//下一页写入的数据
			_ulWriteAddr += PageByte;	//下一页写入的地址
			_usNByte -= PageByte;	//待写入的字节数递减
			if(_usNByte > 256)
			{
				PageByte = 256;
			}
			else
			{
				PageByte = _usNByte;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteSomeBytes
//	功能说明: flash 写数据
//	形    参: 	ucpBuffer：数据存储区首地址
//				_ulWriteAddr: 要读写入Flash的首地址
//				_usNByte： 要写入的字节数(最大65535B = 64K 块)
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: _ulWriteAddr,连续写入_usNByte长度的字节，程序带FLASH数据检查写入
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WriteSomeBytes(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte)
{
	uint32_t ulSecPos = 0;				//得到扇区位置
	uint16_t usSecOff = 0;				//扇区偏移
	uint16_t usSecRemain = 0;		//剩余扇区
	uint32_t i = 0;

	ulSecPos = _ulWriteAddr / 4096;//地址所在扇区(0--511)
	usSecOff = _ulWriteAddr % 4096;//扇区内地址偏移
	usSecRemain = 4096 - usSecOff;//扇区除去偏移，还剩多少字节

	if(_usNByte <= usSecRemain)	//写入数据大小 < 剩余扇区空间大小
	{
		usSecRemain = _usNByte;
	}

	while(1)
	{
		Flash_ReadSomeBytes(SectorBuf, ulSecPos*4096, 4096);//读出整个扇区的内容
		for (i = 0; i < usSecRemain; i++)	//校验数据
		{
			if (SectorBuf[usSecOff + i] != 0xFF)//储存数据不为0xFF，需要擦除
				break;
		}
		
		if(i < usSecRemain)	//需要擦除
		{
			Flash_EraseSector(ulSecPos);	//擦除这个扇区
			for(i = 0; i < usSecRemain; i++)	//保存写入的数据
			{
				SectorBuf[usSecOff + i] = ucpBuffer[i];
			}
			Flash_WriteNoCheck(SectorBuf, ulSecPos*4096, 4096);	//写入整个扇区(扇区=老数据+新写入数据)
		}
		else
		{
			Flash_WriteNoCheck(ucpBuffer, _ulWriteAddr, usSecRemain);//不需要擦除,直接写入扇区
		}
		if(_usNByte == usSecRemain)	//写入结束
		{
			Flash_WriteDisable();
			break;
		}
		else
		{
			ulSecPos++;		//扇区地址增加1
			usSecOff = 0;		//扇区偏移归零
			ucpBuffer += usSecRemain;	//指针偏移
			_ulWriteAddr += usSecRemain;	//写地址偏移
			_usNByte -= usSecRemain;	//待写入的字节递减

			if(_usNByte > 4096)
			{
				usSecRemain = 4096;	//待写入一扇区(4096字节大小)
			}
			else
			{
				usSecRemain = _usNByte;		//待写入少于一扇区的数据
			}
		}
		
	}
	
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_ErasePage
//	功能说明: flash erase page
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 有的 FLASH 支持
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_ErasePage(uint32_t _ulPageAddr)
{
	_ulPageAddr *= 256;
	
	Flash_WriteEnable();
	Flash_WaitNobusy();
	
	FLASH_CS_LOW;
	Spi_WriteByte(FLASH_ERASE_PAGE);	//页擦除指令
	Spi_WriteByte((uint8_t)(_ulPageAddr>>16));	//写入24位地址
	Spi_WriteByte((uint8_t)(_ulPageAddr>>8));
	Spi_WriteByte((uint8_t)(_ulPageAddr>>0));
	FLASH_CS_HIGH;
	
	Flash_WaitNobusy();	//等待写入结束
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_EraseSector
//	功能说明: flash erase sector
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 1扇区 = 4K Bytes
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_EraseSector(uint32_t _ulSectorAddr)
{
	uint8_t command = FLASH_ERASE_SECTOR;
	uint8_t temp_buff[3] = {0};
	
	temp_buff[0] = (uint8_t)(_ulSectorAddr >> 16);
	temp_buff[1] = (uint8_t)(_ulSectorAddr >> 8);
	temp_buff[2] = (uint8_t)(_ulSectorAddr >> 0);
	
	_ulSectorAddr *= 4096;	//1个扇区 4 KBytes
	
	Flash_WriteEnable();
	Flash_WaitNobusy();
	
	FLASH_CS_LOW;
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[0], 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[1], 1);
	hal_spi_send_bytes(SPI_COMM_MODE, &temp_buff[2], 1);


//	Spi_WriteByte(FLASH_ERASE_SECTOR);	//20h
//	Spi_WriteByte((uint8_t)(_ulSectorAddr>>16));	//写入24位地址
//	Spi_WriteByte((uint8_t)(_ulSectorAddr>>8));
//	Spi_WriteByte((uint8_t)(_ulSectorAddr));
	FLASH_CS_HIGH;
	
	Flash_WaitNobusy();	//等待写入结束
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_EraseBlock
//	功能说明: flash erase block 
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 1块 = 64K Bytes
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_EraseBlock(uint32_t _ulBlockAddr)
{
	uint8_t command = FLASH_ERASE_BLOCK;
	_ulBlockAddr *= 65536;	//块地址,一块64K
	
	Flash_WriteEnable();
	Flash_WaitNobusy();

	FLASH_CS_LOW;
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);
	hal_spi_send_bytes(SPI_COMM_MODE, (uint8_t *)(_ulBlockAddr>>16), 1);
	hal_spi_send_bytes(SPI_COMM_MODE, (uint8_t *)(_ulBlockAddr>>8), 1);
	hal_spi_send_bytes(SPI_COMM_MODE, (uint8_t *)(_ulBlockAddr>>0), 1);

	// Spi_WriteByte(FLASH_ERASE_BLOCK);	//d8h
	// Spi_WriteByte((uint8_t)(_ulBlockAddr>>16));	//写入24位地址
	// Spi_WriteByte((uint8_t)(_ulBlockAddr>>8));
	// Spi_WriteByte((uint8_t)(_ulBlockAddr));
	FLASH_CS_HIGH;

	Flash_WaitNobusy();	//等待写入结束
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_EraseChip
//	功能说明: flash erase chip , it makes flash  recovery FF
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_EraseChip(void)
{
	uint8_t command = FLASH_ERASE_CHIP;

	Flash_WriteEnable();	//flash芯片写使能
	Flash_WaitNobusy();	//等待写操作完成
	
	FLASH_CS_LOW;
	hal_spi_recv_bytes(SPI_COMM_MODE, &command, 1);
	// Spi_WriteByte(FLASH_ERASE_CHIP);	//c7h
	FLASH_CS_HIGH;
	
	Flash_WaitNobusy();	//等待写入结束
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_PowerDown
//	功能说明: flash into power down mode 
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_PowerDown(void)
{
	uint8_t command = FLASH_POWER_DOWN; 

	FLASH_CS_LOW;
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);
	// Spi_WriteByte(FLASH_POWER_DOWN);	//b9h
	FLASH_CS_HIGH;
	Sys_delay_us(3);	// cs go high , need to delay 3us
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WakeUp
//	功能说明: wake up flash from power down mode or hign performance mode
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WakeUp(void)
{
	uint8_t command = FLASH_RELEASE_POWER_DOWN; 

	FLASH_CS_LOW;
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);
	// Spi_WriteByte(FLASH_RELEASE_POWER_DOWN);//abh
	FLASH_CS_HIGH;
	Sys_delay_us(3);	//CS go high , need delay 3us
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_ReadDeviceID
//	功能说明: 读取FLASH ID(manufacturer ID-1Byte + Device ID-2Byte:type+density)
//	形    参: 无
//	返 回 值: ulJedId：FLASH ID 3字节
//	日    期: 2020-03-06
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint16_t Flash_ReadDeviceID(void)
{
	uint8_t command = FLASH_READ_DEVICE_ID;
	uint16_t usFlashId = 0;
	uint8_t temp_buff[3] = {0};
	
	FLASH_CS_LOW;
	
	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);	//90h
	hal_spi_send_bytes(SPI_COMM_MODE, temp_buff, 3);	//写入24位地址；假地址
	hal_spi_recv_bytes(SPI_COMM_MODE, temp_buff, 2);

	// Spi_WriteByte(FLASH_READ_DEVICE_ID);	//90h
	// Spi_WriteByte(0x00);//写入24位地址；假地址
	// Spi_WriteByte(0x00);
	// Spi_WriteByte(0x00);	//如果0x01,先输出 Device ID
	// usFlashId |= Spi_ReadByte()<<8;
	// usFlashId |= Spi_ReadByte();
	
	FLASH_CS_HIGH;
	
	usFlashId = (uint16_t)(temp_buff[0] << 8) | (temp_buff[1] << 0);

	return usFlashId;
}
 
//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_ReadJEDECID
//	功能说明: 读取FLASH ID(manufacturer ID-1Byte + Device ID-2Byte:type+density)
//	形    参: 无
//	返 回 值: ulJedId：FLASH ID 3字节
//	日    期: 2020-03-06
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint32_t Flash_ReadJEDECID(void)
{
	uint8_t command = FLASH_READ_JEDEC_ID;
	uint32_t flash_jed_id = 0;
	uint8_t recv_buff[3] = {0};
	
	FLASH_CS_LOW;

	hal_spi_send_bytes(SPI_COMM_MODE, &command, 1);	//9fh
	hal_spi_recv_bytes(SPI_COMM_MODE, recv_buff, 3);
	
	FLASH_CS_HIGH;

	flash_jed_id = (recv_buff[0] << 16) | (recv_buff[1] << 8) | (recv_buff[2] << 0);
	
	return flash_jed_id;
}

#endif	//SOFT_SPI_COMM
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

#if 0

#ifdef SOFT_SPI_COMM	//软件模拟SPI标志

#define SectorBuf  g_DataTmpBuffer

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteEnable
//	功能说明: 写使能,置位 WEL 位 WEL 位(WEL-->1)
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-07
//  备    注: 
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WriteEnable(void)
{
	FLASH_CS_LOW;
	Spi_WriteByte(FLASH_WRITE_ENABLE_CMD);//开启写使能
	FLASH_CS_HIGH;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteDisable
//	功能说明: 写失能,复位 WEL 位(WEL-->0)
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-07
//  备    注: 
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WriteDisable(void)
{
	FLASH_CS_LOW;
	Spi_WriteByte(FLASH_WRITE_DISABLE_CMD);	//开启写失能 04h
	FLASH_CS_HIGH;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteSR
//	功能说明: 读状态寄存器
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-07
//  备    注: 多用于检查 BUSY 位
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint8_t Flash_ReadSR(void)
{
	uint8_t ucTmpVal = 0;
	
	FLASH_CS_LOW;
	
	Spi_WriteByte(FLASH_READ_SR_CMD);	//05h
	ucTmpVal = Spi_ReadByte();
	
	FLASH_CS_HIGH;
	
	return ucTmpVal;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteSR
//	功能说明: 写状态寄存器
//	形    参: 	_ucByte:写入状态寄存器的数值
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WriteSR(uint8_t _ucByte)
{
	Flash_WriteEnable();	
	Flash_WaitNobusy();

	FLASH_CS_LOW;
	Spi_WriteByte(FLASH_WRITE_SR_CMD);	//01h
	Spi_WriteByte(_ucByte);	//写入一个字节
	FLASH_CS_HIGH;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WaitNobusy
//	功能说明: 检查 FLASH BUSY 位状态
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 调用Flash_ReadSR(),判断状态寄存器的R0位,执行结束操作清零
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WaitNobusy(void)
{
	//FLASH_READ_SR_CMD 指令的发送,有的FLASH仅需发送一次,FLASH自动回复,有的FLASH无法自动回复,需要循环一直发送等待
	while(((Flash_ReadSR()) & 0x01)==0x01);	//等待BUSY位清空
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_FastReadByte
//	功能说明: flash 都数据(快速读取：Fast read operate at the highest poossible frequency)
//	形    参: 	ucpBuffer：数据存储区首地址
//				_ulReadAddr: 要读出Flash的首地址
//				_usNByte： 要读出的字节数(最大65535B)
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 从_ulReadAddr地址,连续读出_usNByte长度的字节
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_ReadSomeBytes(uint8_t *ucpBuffer, uint32_t _ulReadAddr, uint16_t _usNByte)
{
	FLASH_CS_LOW;
	
	Spi_WriteByte(FLASH_READ_DATA);	//连续读取数据 03h
	Spi_WriteByte((uint8_t)(_ulReadAddr>>16));	//写入24位地址
	Spi_WriteByte((uint8_t)(_ulReadAddr>>8));
	Spi_WriteByte((uint8_t)(_ulReadAddr>>0));
	while(_usNByte--)
	{
		*ucpBuffer = Spi_ReadByte();
		ucpBuffer++;
	}
	
	FLASH_CS_HIGH;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_FastReadByte
//	功能说明: flash 都数据(快速读取：Fast read operate at the highest poossible frequency)
//	形    参: 	ucpBuffer：数据存储区首地址
//				_ulReadAddr: 要读出Flash的首地址
//				_usNByte： 要读出的字节数(最大65535B)
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 从_ulReadAddr地址,连续读出_usNByte长度的字节
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_FastReadByte(uint8_t *ucpBuffer, uint32_t _ulReadAddr, uint16_t _usNByte)
{
	FLASH_CS_LOW;
	
	Spi_WriteByte(FLASH_FASTREAD_DATA);//快速读取数据 0bh
	Spi_WriteByte((uint8_t)(_ulReadAddr>>16));//写入24位地址
	Spi_WriteByte((uint8_t)(_ulReadAddr>>8));
	Spi_WriteByte((uint8_t)(_ulReadAddr>>0));
	Spi_WriteByte(0xFF);//等待8个时钟(dummy byte)
	while(_usNByte--)
	{
		*ucpBuffer = Spi_ReadByte();
		ucpBuffer++;
	}
	
	FLASH_CS_HIGH;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WritePage
//	功能说明: flash 写数据(按页写入,一页256字节,写入之前FLASH地址上必须为0xFF)
//	形    参: 	ucpBuffer：数据存储区首地址
//				_ulWriteAddr: 要读写入Flash的首地址
//				_usNByte： 要写入的字节数(最大65535B = 64K 块)
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: _ulWriteAddr,连续写入_usNByte长度的字节
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WritePage(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte)
{
	Flash_WriteEnable();	//写使能
	Flash_WaitNobusy();	//等待写入结束
	
	FLASH_CS_LOW;
	
	Spi_WriteByte(FLASH_WRITE_PAGE);	//02h
	Spi_WriteByte((uint8_t)(_ulWriteAddr>>16));	//写入24位地址
	Spi_WriteByte((uint8_t)(_ulWriteAddr>>8));
	Spi_WriteByte((uint8_t)(_ulWriteAddr>>0));
	while(_usNByte--)
	{
		Spi_WriteByte(*ucpBuffer);	//SPI 写入单个字节
		ucpBuffer++;
	}
	
	FLASH_CS_HIGH;
	
	Flash_WaitNobusy();	//等待写入结束
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteNoCheck
//	功能说明: flash 写数据(不带擦除,写入之前必须确保写入部分FLASH的数据全为0xFf,否则写入失败)
//	形    参: 	ucpBuffer：数据存储区首地址
//				_ulWriteAddr: 要读写入Flash的首地址
//				_usNByte： 要写入的字节数(最大65535B = 64K 块)
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: _ulWriteAddr,连续写入_usNByte长度的字节，程序带FLASH数据检查写入
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WriteNoCheck(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte)
{
	uint16_t PageByte = 256 - _ulWriteAddr % 256;//单页剩余可写字节数

	if(_usNByte <= PageByte)	//不大于256字节
	{
		PageByte = _usNByte;
	}
	
	while(1)
	{
		Flash_WritePage(ucpBuffer, _ulWriteAddr, PageByte);
		if(_usNByte == PageByte)	//写入结束
			break;
		else
		{
			ucpBuffer += PageByte;	//下一页写入的数据
			_ulWriteAddr += PageByte;	//下一页写入的地址
			_usNByte -= PageByte;	//待写入的字节数递减
			if(_usNByte > 256)
			{
				PageByte = 256;
			}
			else
			{
				PageByte = _usNByte;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WriteSomeBytes
//	功能说明: flash 写数据
//	形    参: 	ucpBuffer：数据存储区首地址
//				_ulWriteAddr: 要读写入Flash的首地址
//				_usNByte： 要写入的字节数(最大65535B = 64K 块)
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: _ulWriteAddr,连续写入_usNByte长度的字节，程序带FLASH数据检查写入
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WriteSomeBytes(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte)
{
	uint32_t ulSecPos = 0;				//得到扇区位置
	uint16_t usSecOff = 0;				//扇区偏移
	uint16_t usSecRemain = 0;		//剩余扇区
	uint32_t i = 0;

	ulSecPos = _ulWriteAddr / 4096;//地址所在扇区(0--511)
	usSecOff = _ulWriteAddr % 4096;//扇区内地址偏移
	usSecRemain = 4096 - usSecOff;//扇区除去偏移，还剩多少字节

	if(_usNByte <= usSecRemain)	//写入数据大小 < 剩余扇区空间大小
	{
		usSecRemain = _usNByte;
	}

	while(1)
	{
		Flash_ReadSomeBytes(SectorBuf, ulSecPos*4096, 4096);//读出整个扇区的内容
		for (i = 0; i < usSecRemain; i++)	//校验数据
		{
			if (SectorBuf[usSecOff + i] != 0xFF)//储存数据不为0xFF，需要擦除
				break;
		}
		
		if(i < usSecRemain)	//需要擦除
		{
			Flash_EraseSector(ulSecPos);	//擦除这个扇区
			for(i = 0; i < usSecRemain; i++)	//保存写入的数据
			{
				SectorBuf[usSecOff + i] = ucpBuffer[i];
			}
			Flash_WriteNoCheck(SectorBuf, ulSecPos*4096, 4096);	//写入整个扇区(扇区=老数据+新写入数据)
		}
		else
		{
			Flash_WriteNoCheck(ucpBuffer, _ulWriteAddr, usSecRemain);//不需要擦除,直接写入扇区
		}
		if(_usNByte == usSecRemain)	//写入结束
		{
			Flash_WriteDisable();
			break;
		}
		else
		{
			ulSecPos++;		//扇区地址增加1
			usSecOff = 0;		//扇区偏移归零
			ucpBuffer += usSecRemain;	//指针偏移
			_ulWriteAddr += usSecRemain;	//写地址偏移
			_usNByte -= usSecRemain;	//待写入的字节递减

			if(_usNByte > 4096)
			{
				usSecRemain = 4096;	//待写入一扇区(4096字节大小)
			}
			else
			{
				usSecRemain = _usNByte;		//待写入少于一扇区的数据
			}
		}
		
	}
	
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_ErasePage
//	功能说明: flash erase page
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 有的 FLASH 支持
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_ErasePage(uint32_t _ulPageAddr)
{
	_ulPageAddr *= 256;
	
	Flash_WriteEnable();
	Flash_WaitNobusy();
	
	FLASH_CS_LOW;
	Spi_WriteByte(FLASH_ERASE_PAGE);	//页擦除指令
	Spi_WriteByte((uint8_t)(_ulPageAddr>>16));	//写入24位地址
	Spi_WriteByte((uint8_t)(_ulPageAddr>>8));
	Spi_WriteByte((uint8_t)(_ulPageAddr>>0));
	FLASH_CS_HIGH;
	
	Flash_WaitNobusy();	//等待写入结束
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_EraseSector
//	功能说明: flash erase sector
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 1扇区 = 4K Bytes
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_EraseSector(uint32_t _ulSectorAddr)
{
	_ulSectorAddr *= 4096;	//1个扇区 4 KBytes
	
	Flash_WriteEnable();
	Flash_WaitNobusy();
	
	FLASH_CS_LOW;
	Spi_WriteByte(FLASH_ERASE_SECTOR);	//20h
	Spi_WriteByte((uint8_t)(_ulSectorAddr>>16));	//写入24位地址
	Spi_WriteByte((uint8_t)(_ulSectorAddr>>8));
	Spi_WriteByte((uint8_t)(_ulSectorAddr));
	FLASH_CS_HIGH;
	
	Flash_WaitNobusy();	//等待写入结束
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_EraseBlock
//	功能说明: flash erase block 
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 1块 = 64K Bytes
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_EraseBlock(uint32_t _ulBlockAddr)
{
	_ulBlockAddr *= 65536;	//块地址,一块64K
	
	Flash_WriteEnable();
	Flash_WaitNobusy();

	FLASH_CS_LOW;
	Spi_WriteByte(FLASH_ERASE_BLOCK);	//d8h
	Spi_WriteByte((uint8_t)(_ulBlockAddr>>16));	//写入24位地址
	Spi_WriteByte((uint8_t)(_ulBlockAddr>>8));
	Spi_WriteByte((uint8_t)(_ulBlockAddr));
	FLASH_CS_HIGH;

	Flash_WaitNobusy();	//等待写入结束
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_EraseChip
//	功能说明: flash erase chip , it makes flash  recovery FF
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_EraseChip(void)
{
	Flash_WriteEnable();	//flash芯片写使能
	Flash_WaitNobusy();	//等待写操作完成
	
	FLASH_CS_LOW;
	Spi_WriteByte(FLASH_ERASE_CHIP);	//c7h
	FLASH_CS_HIGH;
	
	Flash_WaitNobusy();	//等待写入结束
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_PowerDown
//	功能说明: flash into power down mode 
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_PowerDown(void)
{
	FLASH_CS_LOW;
	Spi_WriteByte(FLASH_POWER_DOWN);	//b9h
	FLASH_CS_HIGH;
	Sys_delay_us(3);	// cs go high , need to delay 3us
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_WakeUp
//	功能说明: wake up flash from power down mode or hign performance mode
//	形    参: no
//	返 回 值: no
//	日    期: 2020-03-07
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void Flash_WakeUp(void)
{
	FLASH_CS_LOW;
	Spi_WriteByte(FLASH_RELEASE_POWER_DOWN);//abh
	FLASH_CS_HIGH;
	Sys_delay_us(3);	//CS go high , need delay 3us
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_ReadDeviceID
//	功能说明: 读取FLASH ID(manufacturer ID-1Byte + Device ID-2Byte:type+density)
//	形    参: 无
//	返 回 值: ulJedId：FLASH ID 3字节
//	日    期: 2020-03-06
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint16_t Flash_ReadDeviceID(void)
{
	uint16_t usFlashId = 0;
	
	FLASH_CS_LOW;
	
	Spi_WriteByte(FLASH_READ_DEVICE_ID);	//90h
	Spi_WriteByte(0x00);//写入24位地址；假地址
	Spi_WriteByte(0x00);
	Spi_WriteByte(0x00);	//如果0x01,先输出 Device ID
	usFlashId |= Spi_ReadByte()<<8;
	usFlashId |= Spi_ReadByte();
	
	FLASH_CS_HIGH;
	
	return usFlashId;
}
 
//--------------------------------------------------------------------------------------------------------
//	函 数 名: Flash_ReadJEDECID
//	功能说明: 读取FLASH ID(manufacturer ID-1Byte + Device ID-2Byte:type+density)
//	形    参: 无
//	返 回 值: ulJedId：FLASH ID 3字节
//	日    期: 2020-03-06
//  备    注: 软件模拟SPI
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint32_t Flash_ReadJEDECID(void)
{
	uint32_t ulJedId = 0;
	
	FLASH_CS_LOW;

	Spi_WriteByte(FLASH_READ_JEDEC_ID);	//9fh
	ulJedId |= Spi_ReadByte()<<16;
	ulJedId |= Spi_ReadByte()<<8;
	ulJedId |= Spi_ReadByte();
	
	FLASH_CS_HIGH;
	
	return ulJedId;
}

#endif	//SOFT_SPI_COMM

#endif	//0

#endif	//__PRJ_STM32F10X_DEVEXFLASH_C__
