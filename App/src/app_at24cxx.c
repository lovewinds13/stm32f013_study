#ifndef __PRJ_STM32F10X_APP_AT24CXX_C__
#define __PRJ_STM32F10X_APP_AT24CXX_C__

#include <stdio.h>
#include "declares.h"
#include "Typedef.h"
#include "drvsfi2c.h"
#include "app_at24cxx.h"
#include "delay.h"
#include "usart.h"
#include "Variable.h"

#define EEP_TYPE TYPE_AT24C02
#define I2C_WAIT_TIME	200

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: ee_at24cxx_init
//	功能说明: eeprom初始化
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: 调用I2c_Init
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------------------------
void ee_at24cxx_init(void)
{
    I2c_Init();
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: ee_at24cxx_write_byte
//	功能说明: eeprom写一个字节数据
//	形    参: 	write_addr：写入地址
//              write_byte：写入数据
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: eeprom小于16K bits
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------------------------
bool ee_at24cxx_write_byte(uint16_t write_addr, uint8_t write_byte)
{
    I2c_Start();

    I2c_SendOneByte(AT24CXX_ADDR | ((write_addr / 256) << 1));    //发送设备地址+数据地址高3位(<16K:1010 + p2p1p0 + r/w)
    if (I2c_WaitAck(I2C_WAIT_TIME) == I2C_NACK)
    {
        return false;
    }
    I2c_SendOneByte(write_addr % 256);	//设备地址低8位
    if (I2c_WaitAck(I2C_WAIT_TIME) == I2C_NACK)
    {
        return false;
    }

    I2c_SendOneByte(write_byte);    //写入的数据
    if (I2c_WaitAck(I2C_WAIT_TIME) == I2C_NACK)
    {
        return false;
    }

    I2c_Stop();
	
	delay_ms(5  );	//写完一个字节需要延时重新开启总线

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: ee_at24cxx_read_byte
//	功能说明: eeprom读一个字节数据
//	形    参: 	write_addr：读出地址
//              write_byte：读出数据
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: eeprom小于16K bits(先确定读数据地址,再进行读数据)
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------------------------
bool ee_at24cxx_read_byte(uint16_t read_addr)
{
    uint8_t ret = 0;

    I2c_Start();

    I2c_SendOneByte(AT24CXX_ADDR | ((read_addr / 256) << 1));    //发送地址高3位(<16K:1010 + p2p1p0 + r/w)
    if (I2c_WaitAck(I2C_WAIT_TIME) == I2C_NACK)
    {
        return false;
    }
    I2c_SendOneByte(read_addr % 256);
    if (I2c_WaitAck(I2C_WAIT_TIME) == I2C_NACK)
    {
        return false;
    }

	 I2c_Start();
	
    I2c_SendOneByte(AT24CXX_ADDR | 0x01);  //设备地址 + 读操作
    if (I2c_WaitAck(I2C_WAIT_TIME) == I2C_NACK)
    {
        return false;
    }

    ret = I2c_RecvOneByte(I2C_NACK);

    I2c_Stop();
	
	return ret;
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: ee_at24cxx_write_some_bytes
//	功能说明: eeprom写一些字节数据
//	形    参: 	write_addr：写入设备地址
//				pbdata：写入数据地址
//              write_length：写入数据长度
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: eeprom小于16K bits,循环调用ee_at24cxx_write_byte
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------------------------
void ee_at24cxx_write_some_bytes(uint16_t write_addr, uint8_t *pbdata, uint16_t write_length)
{
    for (uint16_t i = 0; i < write_length; i++)
    {
        ee_at24cxx_write_byte(write_addr, pbdata[i]);
		write_addr++;	//写入地址递增
    }
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: ee_at24cxx_read_some_bytes
//	功能说明: eeprom读出一些字节数据
//	形    参: 	read_addr：读出设备地址
//				pbdata：读出数据地址
//              read_length：读出数据长度
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: eeprom小于16K bits,循环调用ee_at24cxx_read_byte
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------------------------
uint16_t ee_at24cxx_read_some_bytes(uint16_t read_addr, uint8_t *pbdata, uint16_t read_length)
{
    uint16_t temp = read_length;

    while (temp--)
    {
        *pbdata++ = ee_at24cxx_read_byte(read_addr);	
		read_addr++;	//读出地址递增
    }

    return read_length;
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: ee_at24cxx_test
//	功能说明: eeprom测试
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: eeprom小于16K bits
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------------------------
#define AT_TEST_ADDR 	0x10
void ee_at24cxx_test(void)
{
    uint8_t test_val = 0x55;
    uint8_t ret = 0;
	uint8_t write_buf[10] = {0xcc, 0xaa, 0x66, 0x5a, 0x77, 0x89, 0x33, 0xbb, 0xac, 0xe9};
	uint8_t read_buf[10] = {0};

    ee_at24cxx_write_byte(0x00, test_val);
	
	Sys_delay_ms(10);

    ret = ee_at24cxx_read_byte(0x00);

    printf("ee at24cxx test val :0x%02x \r\n", ret);
	
	ee_at24cxx_write_some_bytes(AT_TEST_ADDR, write_buf, sizeof(write_buf));
	Sys_delay_ms(50);
	ee_at24cxx_read_some_bytes(AT_TEST_ADDR, read_buf, sizeof(write_buf));
	
	for (uint16_t i = 0; i < sizeof(write_buf); i++)
	{
		printf("ee test some byte ---> 0x%02x \r\n", read_buf[i]);
	}
	
	printf("\r\n");
}

#endif  //__PRJ_STM32F10X_APP_AT24CXX_C__
