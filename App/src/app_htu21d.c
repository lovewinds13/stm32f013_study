#ifndef __PRJ_STM32F10X_APP_HTU21D_C__
#define __PRJ_STM32F10X_APP_HTU21D_C__

#include <stdio.h>
#include <math.h>
#include "declares.h"
#include "Typedef.h"
#include "drvsfi2c.h"
#include "app_htu21d.h"
#include "delay.h"
#include "usart.h"
#include "Variable.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Htu_Init
//	功能说明: 传感器初始化
//	形    参: 无
//	返 回 值: 无
//  备    注: 
//	日    期: 2020-03-11
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void Htu_Init(void)
{
	  
	I2c_Init();
	I2c_Start();
	I2c_SendOneByte(HTU_ADDR_WR);	//写I2C器件地址
	I2c_WaitAck(200);
	I2c_SendOneByte(HTU_SOFTWARE_RESET);		//软复位
	I2c_WaitAck(200);
	I2c_Stop();
	delay_ms(15);		//软复位时间最多需要15ms
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: Htu_Measure
//	功能说明: Htu21d 温湿度读取
//	形    参: 	_ucOrder：温度 or 湿度读取命令
//	返 回 值: 无
//	日    期: 2020-03-16
//  备    注: 
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------------------------
#if 1
float Htu_Measure(uint8_t _ucOrder)
{
	 uint8_t ucMsb = 0;
	 uint8_t ucLsb = 0;
	 volatile float fTemp = 0.0;
	 volatile float fHumi = 0.0;
	 volatile float fRetVal = 0.0;
	 
	 I2c_Start();
	 
	 I2c_SendOneByte(HTU_ADDR_WR);		//地址+写命令
	 if(I2c_WaitAck(200)==I2C_NACK)
	{
		 return false;
	}
	 
	I2c_SendOneByte(_ucOrder);		//触发测量命令
	if(I2c_WaitAck(200)==I2C_NACK)
	{
		 return false;
	}
	
	Sys_delay_ms(50);	//14位测量时间范围(44-58ms)
	 
	do{
	//	delay_ms(5);
		I2c_Start();	//重启总线
		I2c_SendOneByte(HTU_ADDR_RD);		//地址+读命令
	}while(I2c_WaitAck(1)==I2C_NACK);

	ucMsb = I2c_RecvOneByte(I2C_ACK);	//主机给出ACK
	ucLsb = I2c_RecvOneByte(I2C_NACK);	//读取的最后一字节发送NACK

	I2c_Stop();

	ucLsb &= 0xFC;		//设置分辨率,最低两位为0,温度:14位;湿度:12位 	
	fRetVal = ucMsb * 256 + ucLsb;/*MSB=(MSB<<=8)+LSB;即将MSB移位到高8位*/

	if(_ucOrder == HTU_TEMP)
	{
		fTemp = (175.72) * fRetVal / 65536 - 46.85;//温度:T= -46.85 + 175.72 * ST/2^16

		return fTemp;
	}
	else if(_ucOrder == HTU_HUMI)
	{
		fHumi = (fRetVal * 125) / 65536 - 6;//湿度: RH%= -6 + 125 * SRH/2^16

		return fHumi;
	}
	else
	{
		return false;
	}
} 

#else

float Htu_Measure(uint8_t _ucOrder)
{
	uint8_t ucaRecvBuf[2] = {0};
	uint8_t ucTmpVal = _ucOrder;
	volatile float fTemp = 0.0;
	volatile float fHumi = 0.0;
	volatile float fRetVal = 0.0;
	 
	htu_write_some_bytes(&ucTmpVal, 1);		//写如操作命令

	delay_ms(50);	//14位测量时间范围(44-58ms)
	
	htu_read_some_bytes(ucaRecvBuf, 2);	//接收两字节数据
			 
	ucaRecvBuf[1] &= 0xFC;		//设置分辨率,最低两位为0,温度:14位;湿度:12位 	
	fRetVal = (ucaRecvBuf[0] << 8) | ucaRecvBuf[1];	// MSB=(MSB<<=8)+LSB;即将MSB移位到高8位

	if(_ucOrder == HTU_TEMP)
	{
		 fTemp = (175.72) * fRetVal / 65536 - 46.85;//温度:T= -46.85 + 175.72 * ST/2^16
		 
		 return fTemp;
	}
	else if(_ucOrder == HTU_HUMI)
	{
		 fHumi = (fRetVal * 125) / 65536 - 6.00;//湿度: RH%= -6 + 125 * SRH/2^16

		 return fHumi;
	}
	else
	{
		return false;
	}
} 
#endif

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: htu_write_some_bytes
//	功能说明: htu21d 通过IC写入多字节数据
//	形    参: 	pbdata：写入的数据
//				write_length：写入数据的长度
//	返 回 值: 无
//	日    期: 2020-03-19
//  备    注: 测试I2C发送多字节数据时序
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
bool htu_write_some_bytes(uint8_t *pbdata, uint16_t write_length)
{
	I2c_Start();

	I2c_SendOneByte(HTU_ADDR_WR);
	if (I2C_NACK == I2c_WaitAck(200))
	{
		return false;
	}
	
	//for循环发送多个字节数据
	for (uint16_t i = 0; i < write_length; i++)
	{
		I2c_SendOneByte(pbdata[i]);
		if (I2C_NACK == I2c_WaitAck(200))
		{
			return false;
		}
	}

	//while循环发送多个字节数据
//	while (write_length--)
//	{
//		I2c_SendOneByte(*pbdata++);
//		if (I2C_NACK == I2c_WaitAck(200))
//		{
//			return false;
//		}
//	}

//	I2c_Stop();

	return true;
	
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: htu_read_some_bytes
//	功能说明: htu21d 通过IC读取多字节数据
//	形    参: 	pbdata：写入的数据
//				read_length：写入数据的长度
//	返 回 值: 无
//	日    期: 2020-03-19
//  备    注: 测试I2C接收多字节数据时序
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
bool htu_read_some_bytes(uint8_t *pbdata, uint16_t read_length)
{
	I2c_Start();

	I2c_SendOneByte(HTU_ADDR_RD);
	if (I2C_NACK == I2c_WaitAck(200))
	{
		return false;
	}

	for (uint16_t i = 0; i < read_length - 1; i++)
	{
		*pbdata++ = I2c_RecvOneByte(I2C_ACK);
	}
	*pbdata++ = I2c_RecvOneByte(I2C_NACK);	//接收最后一个字节发送NACK,告诉从机读操作已经完成

	I2c_Stop();

	return true;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Htu_Display
//	功能说明: 测量数据显示
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-16
//  备    注: 
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
 void Htu_Display(void)
{
	u16 usTemp = 0;
	volatile double f_RetVal = 0.0;
	u8 ucTest[10] = {0};
	
	f_RetVal = Htu_Measure(HTU_TEMP);//得到温度值
	printf("The htu measure temp is :%4.2fC \r\n", f_RetVal);
	
	sprintf((char*)ucTest,"%4.2f", f_RetVal);		//LCD显示方式1：sprintf函数将结果打印到test数组里,转换成字符串
	printf("test is %sC \r\n", ucTest);
	printf("\r\n");

	
	usTemp = f_RetVal;			//LCD显示方式2:将得到的数值拆分成整数和小数直接显示在液晶
	f_RetVal -= usTemp;
	f_RetVal *= 100;		//保留两位小数

		
	f_RetVal= Htu_Measure(HTU_HUMI);		//得到湿度值
	printf("The htu measure humi is :%4.2fRH \r\n", f_RetVal);
	usTemp = f_RetVal;
	f_RetVal -= usTemp;
	f_RetVal *= 100;
	printf("\r\n");
	
}

#endif	//__PRJ_STM32F10X_APP_HTU21D_C__
