#ifndef __PRJ_STM32F10X_HARDWARE_SPI_C__
#define __PRJ_STM32F10X_HARDWARE_SPI_C__

#include "Typedef.h"
#include "declares.h"
#include "hardware_spi.h"
#include "Variable.h"
#include "drvexflash.h"
#include "drvsfspi.h"
#include "drvnrf2401.h"
#include "drvsdcard.h"
#include "stm32f10x_conf.h"
#include <stdio.h>

#ifdef HARD_SPI_COMM

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C							  
//SPI口初始化
//这里针是对SPI1的初始化

//--------------------------------------------------------------------------------------------------------
//	函 数 名: spi_flash_gpio_init
//	功能说明: SPI FLASH 片选信号初始化
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-12
//  备    注:采用 Unix like 方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void spi_flash_gpio_init(void)
{
	GPIO_InitTypeDef gpio_config_init;

#if 0	//调试CS硬件方式
	RCC_APB2PeriphClockCmd(RCC_PCLK_SPIM1_GPIO, ENABLE);		//开启SPIM1 GPIO时钟

	gpio_config_init.GPIO_Pin 	= FLASH_CS_IO;	//SPIM1_CLK_IO IO初始化
	gpio_config_init.GPIO_Mode 	= GPIO_Mode_Out_PP;  //复用推挽输出
	gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPIM1_GPIO_PORT, &gpio_config_init);

	GPIO_SetBits(SPIM1_GPIO_PORT, FLASH_CS_IO);	//IO初始状态都设置为高电平
#else
	RCC_APB2PeriphClockCmd(RCC_PCLK_SPIM1_GPIO, ENABLE);		//开启SPIM1 GPIO时钟

	gpio_config_init.GPIO_Pin 	= FLASH_CS;	//SPIM1_CLK_IO IO初始化
	gpio_config_init.GPIO_Mode 	= GPIO_Mode_Out_PP;  //复用推挽输出
	gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPIM1_GPIO_PORT, &gpio_config_init);

	GPIO_SetBits(SPIM1_GPIO_PORT, FLASH_CS);	//IO初始状态都设置为高电平
#endif
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: spi_gpio_init
//	功能说明: SPI 硬件IO初始化
//	形    参: 	spi_chl：SPIM 通道
//	返 回 值: 无
//	日    期: 2020-03-12
//  备    注:采用 Unix like 方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void spi_gpio_init(uint8_t spi_chl)
{
	GPIO_InitTypeDef gpio_config_init;

	if (spi_chl == 1)
	{
		RCC_APB2PeriphClockCmd(RCC_PCLK_SPIM1_GPIO, ENABLE);		//开启SPIM1 GPIO时钟、
		
//		gpio_config_init.GPIO_Pin 		= SPIM1_CLK_IO | SPIM1_MISO_IO | SPIM1_MOSI_IO;	//SPIM1_CLK_IO IO初始化
		gpio_config_init.GPIO_Pin 		= SPIM1_CLK_IO | SPIM1_MOSI_IO;
		gpio_config_init.GPIO_Mode 		= GPIO_Mode_AF_PP;  //复用推挽输出
		gpio_config_init.GPIO_Speed 	= GPIO_Speed_50MHz;
		
		GPIO_Init(SPIM1_GPIO_PORT, &gpio_config_init);
		
		gpio_config_init.GPIO_Pin 		= SPIM1_MISO_IO;	//SPIM1_MISO_IO IO初始化
		gpio_config_init.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;  //MISO浮空输入
		gpio_config_init.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_Init(SPIM1_GPIO_PORT, &gpio_config_init);

		GPIO_SetBits(SPIM1_GPIO_PORT, SPIM1_CLK_IO | SPIM1_MISO_IO | SPIM1_MOSI_IO);	//IO初始状态都设置为高电平
	}		
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: spi_master_init
//	功能说明: SPI 硬件配置参数初始化
//	形    参: 	spi_chl：SPIM 通道
//	返 回 值: 无
//	日    期: 2020-03-12
//  备    注:采用 Unix like 方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void spi_master_init(uint8_t spi_chl)
{
	SPI_InitTypeDef  spi_config_init;
#if 1  
	if(spi_chl == 1)
	{	
		spi_flash_gpio_init();	//spi flash cs 初始化
		sd_gpio_init();	//spi sd cs 初始化
		nrf24l01_gpio_init();//spi nrf24l01 cs 初始化
		
		spi_gpio_init(1);	//spi gpio 初始化

		RCC_APB2PeriphClockCmd(RCC_PCLK_SPIM1_HD, ENABLE);	//SPI1时钟使能

		spi_config_init.SPI_Direction 			= SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
		spi_config_init.SPI_Mode 				= SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
		spi_config_init.SPI_DataSize 			= SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
		spi_config_init.SPI_CPOL 				= SPI_CPOL_Low;		//选择了串行时钟的稳态:空闲时钟低
		spi_config_init.SPI_CPHA 				= SPI_CPHA_1Edge;	//数据捕获(采样)于第1个时钟沿
		spi_config_init.SPI_NSS					= SPI_NSS_Soft;//SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
		spi_config_init.SPI_BaudRatePrescaler 	= SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
		spi_config_init.SPI_FirstBit 			= SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
		spi_config_init.SPI_CRCPolynomial 		= 7;	//CRC值计算的多项式
		
		SPI_Init(SPI1, &spi_config_init);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	 
		SPI_Cmd(SPI1, ENABLE); //使能SPI外设
		
//		spi_master_send_recv_byte(1, 0xFF);	//启动传输	
	
	}
#endif
} 

//--------------------------------------------------------------------------------------------------------
//	函 数 名: spi_master_send_recv_byte
//	功能说明: SPI 收发数据
//	形    参: 	spi_chl：SPIM 通道
//				send_byte：发送的数据
//	返 回 值: 无
//	日    期: 2020-03-14
//  备    注:采用 Unix like 方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint8_t spi_master_send_recv_byte(uint8_t spi_chl, uint8_t spi_byte)
{		
	uint8_t time = 0;
	
	if (spi_chl == 1)			    
	{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
			time++;
			if(time>200)
			{
				return false;
			}
		}			  
		SPI_I2S_SendData(SPI1, spi_byte); //通过外设SPIx发送一个数据
	
		time = 0;

		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
			time++;
			if(time>200)
			{
				return false;
			}
		}	  						    
			return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
	}
	else 
	{
		return false;
	}
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: spi_master_speed_set
//	功能说明: SPI 通信速度设置
//	形    参: 	spi_chl：SPI通道
//				spi_speed： 通信速度
//	返 回 值: 无
//	日    期: 2020-03-12
//  备    注: 
//			SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//			SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//			SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//			SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void spi_master_speed_set(uint8_t spi_chl, uint8_t spi_speed)
{
	SPI_InitTypeDef  spi_config_init;

	if (spi_chl == 1)
	{
		spi_config_init.SPI_BaudRatePrescaler = spi_speed ;

		SPI_Init(SPI1, &spi_config_init);
		SPI_Cmd(SPI1, ENABLE);
	}
} 
#if 0
//--------------------------------------------------------------------------------------------------------
//	函 数 名: spi_master_send_byte
//	功能说明: SPI 发送一个字节数据
//	形    参: 	spi_chl：SPIM 通道
//				send_byte：发送的数据
//	返 回 值: 无
//	日    期: 2020-03-12
//  备    注:采用 Unix like 方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint8_t spi_master_send_byte(uint8_t spi_chl, uint8_t send_byte)
{		
	
	if (spi_chl == 1)
	{
		spi_master_send_recv_byte(spi_chl, send_byte);
		
	}
	
	return true;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: spi_master_recv_byte
//	功能说明: SPI 接收一个字节数据
//	形    参: 	spi_chl：SPIM 通道
//	返 回 值: 接收的数据
//	日    期: 2020-03-12
//  备    注:采用 Unix like 方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint8_t spi_master_recv_byte(uint8_t spi_chl)
{
	
	if (spi_chl == 1)
	{
		spi_master_send_recv_byte(spi_chl, 0xFF);
		
	}
	
	return true;
}
#endif	//spi的收发是同时进行的,并且发送方要为接收方提供时钟,收发分开会导致传输过程中时序的问题

//--------------------------------------------------------------------------------------------------------
//	函 数 名: spi_master_send_some_bytes
//	功能说明: SPI 发送多个字节数据
//	形    参: 	spi_chl：SPIM 通道
//				pbdata：发送的数据首地址
//				send_length：发送数据长度
//	返 回 值: 无
//	日    期: 2020-03-12
//  备    注:采用 Unix like 方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void spi_master_send_some_bytes(uint8_t spi_chl, uint8_t *pbdata, uint16_t send_length)
{
	uint16_t i = 0;

	for (i = 0; i < send_length; i++)
	{
		spi_master_send_recv_byte(spi_chl, pbdata[i]);
	}
	
//	while (send_length--)
//	{
//		spi_master_send_byte(spi_chl, *pbdata++);
//	}
	
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: spi_master_recv_some_bytes
//	功能说明: SPI 接收多个字节数据
//	形    参: 	spi_chl：SPIM 通道
//				pbdata：接收的数据首地址
//				send_length：接收数据长度
//	返 回 值: 无
//	日    期: 2020-03-12
//  备    注:采用 Unix like 方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void spi_master_recv_some_bytes(uint8_t spi_chl, uint8_t *pbdata, uint16_t recv_length)
{
	uint8_t *temp_data = pbdata;

	while (recv_length--)
	{
		*temp_data++ = spi_master_send_recv_byte(spi_chl, 0xFF);	//发送 0xff 为从设备提供时钟
	}
	
}


//--------------------------------------------------------------------------------------------------------
//	函 数 名: spi_master_nvic_set
//	功能说明: SPI 中断设置
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-12
//  备    注:采用 Unix like 方式
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void spi_master_nvic_set(void)
{
	NVIC_InitTypeDef nvic_config_init;
	
	nvic_config_init.NVIC_IRQChannel 						= SPI1_IRQn;
	nvic_config_init.NVIC_IRQChannelPreemptionPriority 	= 2;
	nvic_config_init.NVIC_IRQChannelSubPriority 			= 2;
	nvic_config_init.NVIC_IRQChannelCmd 					= ENABLE;

	NVIC_Init(&nvic_config_init);
	
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);	//开启SPI1主设备接收使能
}

uint32_t spi_flash_read_id(void)
{
	uint32_t ulJedId = 0;
	uint8_t recv_buff[5] = {0};
	
	 FLASH_CS_0();

#if 0
	 spi_master_send_recv_byte(1, FLASH_READ_JEDEC_ID);	//9fh
	 ulJedId |= spi_master_send_recv_byte(1, 0xFF)<<16;
	 ulJedId |= spi_master_send_recv_byte(1, 0xFF)<<8;
	 ulJedId |= spi_master_send_recv_byte(1, 0xFF);
#endif
	
#if 1
	spi_master_send_recv_byte(1, FLASH_READ_JEDEC_ID);	//9fh
	 
	spi_master_recv_some_bytes(1, recv_buff, sizeof(recv_buff));
	
	ulJedId = (recv_buff[0] <<16) | (recv_buff[1] <<8) | (recv_buff[2]);
	
//	spi_master_send_byte(1, 0xff);
//	recv_buff[0] = spi_master_recv_byte(1);
//	spi_master_send_byte(1, 0xff);
//	recv_buff[1] = spi_master_recv_byte(1);
//	spi_master_send_byte(1, 0xff);
//	recv_buff[2] = spi_master_recv_byte(1);
//	spi_master_send_byte(1, 0xff);
//	recv_buff[3] = spi_master_recv_byte(1);
//	spi_master_send_byte(1, 0xff);
//	recv_buff[4] = spi_master_recv_byte(1);
	
	for (uint16_t i = 0; i < sizeof(recv_buff); i++)
	{
		printf("recv ---> %d 0x%02X \r\n", i, recv_buff[i]);
	}
	printf("\r\n");
	
#endif

	 FLASH_CS_1();
	
	return ulJedId;
}

//-------------------------------------------------------------------------------------------------------
//	Author:By 霁风AI
//	Date:2019.12.22
//	Name: SPI1_IRQHandler
//	Function:SPI1 中断服务函数
//	Parameter:  无
//	Rerurn: 无
//	Note:
//		(1)g_SpiTxRxLen:全局变量,spi接收数据长度
//		(2)g_byTmpBuffer:全局变量,spi接收的数据暂存
//-------------------------------------------------------------------------------------------------------
void SPI1_IRQHandler(void)
{
	uint16_t i = 0;
	
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) != RESET)	//产生接收中断
	{
		SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);	//清除中断标志
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);	//禁止中断使能
		for (i = 0; i < g_SpiTxRxLen; i++)
		{
//			while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
			g_DataTmpBuffer[i] = (uint8_t)SPI_I2S_ReceiveData(SPI1);	//接收数据
		}
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);	//重新使能中断
	}
}




uint8_t SPI_WriteReadByte(uint8_t TxData)
{
  while((SPI1->SR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);
  SPI1->DR = TxData;

  while((SPI1->SR & SPI_I2S_FLAG_RXNE) == (uint16_t)RESET);
  return SPI1->DR;
}

uint16_t SFLASH_ReadID(void)
{
  uint16_t ID = 0;
   FLASH_CS_0();                              //使能器件

//  SPI_WriteReadByte(90);           //《设备ID》指令
//  SPI_WriteReadByte(0x00);
//  SPI_WriteReadByte(0x00);
//  SPI_WriteReadByte(0x00);
	
//	SPI1_ReadWriteByte(0x90);//发送读取ID命令	    
//	SPI1_ReadWriteByte(0x00); 	    
//	SPI1_ReadWriteByte(0x00); 	    
//	SPI1_ReadWriteByte(0x00); 

//  ID |= SPI_WriteReadByte(0xFF)<<8;              //读取ID
//  ID |= SPI_WriteReadByte(0xFF);
	

//	spi_master_send_byte(1, 0x90);//发送读取ID命令	    
//	spi_master_send_byte(1, 0x00); 	    
//	spi_master_send_byte(1, 0x00); 	    
//	spi_master_send_byte(1, 0x00); 

//	ID |= spi_master_recv_byte(0xFF)<<8;              //读取ID
//	ID |= spi_master_recv_byte(0xFF);

	spi_master_send_recv_byte(1, 0x90);//发送读取ID命令	    
	spi_master_send_recv_byte(1, 0x00); 	    
	spi_master_send_recv_byte(1, 0x00); 	    
	spi_master_send_recv_byte(1, 0x00); 

	ID |= spi_master_send_recv_byte(1, 0xFF)<<8;              //读取ID
	ID |= spi_master_send_recv_byte(1, 0xFF);
  
   FLASH_CS_1();                             //失能器件
	
  return ID;
}

uint8_t SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}

uint16_t SPI_Flash_ReadID(void)
{
	u16 Temp = 0;	  
	 FLASH_CS_0();			    
	SPI1_ReadWriteByte(0x90);//发送读取ID命令	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	 FLASH_CS_1();				    
	return Temp;
}  


#endif	//HARD_SPI_COMM

#endif	//__PRJ_STM32F10X_HARDWARE_SPI_C__
