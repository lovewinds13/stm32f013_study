#ifndef _DECLARES_H_
#define _DECLARES_H_


//通信方式选择
#define SOFT_SPI_COMM	//软件模拟SPI
#define HARD_SPI_COMM	//硬件SPI
#define SPI_COMM_MODE	1	//确定是硬件还是软件SPI

#define SOFT_I2C_COMM
//#define I2C_HTU_TEST

#define UART_FUNC_TEST

//模块功能测试
//#define ADC_FUNC_TEST
#define OLED_FUNC_TEST
//#define TIMER_FUNC_TEST

//-------------------------------------------------------------------------
//业务逻辑实现
//#include "typedef.h"	//数据类型定义

//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//底层驱动库
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//C库
//#include "stdio.h"
//#include "string.h"
//#include "stdlib.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//硬件驱动
//#include "sys.h"	//包含 "stm32f10x.h"
//#include "led.h"
//#include "delay.h"
//#include "usart.h"
//#include "hardware_spi.h"
//#include "drvsfspi.h"
//#include "drvsfi2c.h"
//#include "drvadc.h"
//#include "drvexflash.h"
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//应用实现
//#include "variable.h"
//#include "app_flash.h"
//#include "app_adc.h"
//-------------------------------------------------------------------------


#endif



//***************************** 微信公众号 TECHTIMES (END OF FILE) *********************************/
