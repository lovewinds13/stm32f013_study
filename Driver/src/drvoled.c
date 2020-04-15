//--------------------------------------------------------------------------------------------------------
//
//	模块名称 : OLED操作
//	文件名称 : drvoled.c
//	版    本 : V1.0
//	说    明 : 
//				(1) drvoled.c调用芯片Spi通信接口
//				(2) 
//				(3) 
//				(4) 
//
//	修改记录 :
//		版本号     日期        作者     说明
//		V1.0    2020-04-11  霁风AI  正式发布
//		V1.1    
//		V1.2	
//		V1.3	
//
//	Copyright (C), 2020-2030, 微信公众号——TECHTIMES
//
//--------------------------------------------------------------------------------------------------------

#ifndef __PRJ_STM32F10X_DRVOLED_C__
#define __PRJ_STM32F10X_DRVOLED_C__

#include "stm32f10x_conf.h"
#include "oled_font.h"
#include "bmp.h"
#include "drvoled.h"
#include "bmp.h"
#include "hardware_spi.h"
#include "hal_spi.h"
#include "delay.h"


//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_gpio_init
//	功能说明: oled gpio 初始化
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
static void oled_gpio_init(void)
{
    GPIO_InitTypeDef gpio_config_init;

	RCC_APB2PeriphClockCmd(OLED_GPIO_CLK, ENABLE);		//开启OLED GPIO时钟

	gpio_config_init.GPIO_Pin = OLED_CS_PIN | OLED_DC_PIN | OLED_RST_PIN;	//OLED IO初始化
	gpio_config_init.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OLED_PORT, &gpio_config_init);

	GPIO_SetBits(OLED_PORT, OLED_CS_PIN | OLED_DC_PIN | OLED_RST_PIN);	//IO初始状态都设置为高电平
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_write_byte
//	功能说明: oled 写一字节数据
//	形    参: write_byte：写入的数据
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 调用SPI发送数据函数
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
static void oled_write_byte(uint8_t write_byte)
{
//	hal_spi_send_bytes(1, &write_byte, 1);	//硬件方式SPI(中间层函数封装)
	spi_master_send_recv_byte(1, write_byte);	//底层硬件SPI
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_write_operate
//	功能说明: oled 写操作
//	形    参: mode：写入的是数据还是命令(0：命令;1：数据)
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 包含写数据和写命令,通过OLED DC 引脚高低电平区分
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
static void oled_write_operate(uint8_t mode, uint8_t dat)
{
	OLED_CS_LOW;
	
	if (mode)	//写入数据
	{
		OLED_DC_HIGH;
	}
	else 	//写入命令
	{
		OLED_DC_LOW;
	}
	
	oled_write_byte(dat);
	OLED_CS_HIGH;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_set_pos
//	功能说明: oled 显示坐标设置
//	形    参: x:横坐标;y：纵坐标
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 无
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_set_pos(uint8_t x, uint8_t y)
{
	oled_write_operate(OLED_COMM, 0xb0 + y);	//设置页(page)地址
	oled_write_operate(OLED_COMM, ((x & 0xf0) >> 4) | 0x10);	//列高四位地址
	oled_write_operate(OLED_COMM, (x & 0x0f) | 0x01);	//列低四位地址
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_dis_on
//	功能说明: oled 打开显示
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 写入AF指令打开
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_dis_on(void)
{
	oled_write_operate(OLED_COMM, 0x8d);	//打开DCDC
	oled_write_operate(OLED_COMM, 0x14);	//打开DCDC
	oled_write_operate(OLED_COMM, 0xaf);	//打开OLED
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_dis_off
//	功能说明: oled 关闭显示
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 写入AE指令关闭
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_dis_off(void)
{
	oled_write_operate(OLED_COMM, 0x8d);//设置DCDC
	oled_write_operate(OLED_COMM, 0x10);//关闭DCDC
	oled_write_operate(OLED_COMM, 0xae);//关闭OLED
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_dis_clear
//	功能说明: oled 清除显示
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 写入00清除
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_dis_clear(void)
{
	uint8_t page = 0;
	uint8_t i = 0;
	
	for (page = 0; page < 8; page++)
	{
		oled_write_operate(OLED_COMM, 0xb0 + page);	//设置页地址(0--7)
		oled_write_operate(OLED_COMM, 0x00);	//设置显示位置—列低地址
		oled_write_operate(OLED_COMM, 0x10); 	//设置显示位置—列高地址
	
		for(i = 0; i < 128; i++)
		{
			oled_write_operate(OLED_DATA, 0x00);	//0x00清屏
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_dis_set
//	功能说明: oled 显示出可视区域
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 写入ff设置
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_dis_set(void)
{
	uint8_t page = 0;
	uint8_t i = 0;
	
	for (page = 0; page < 8; page++)
	{
		oled_write_operate(OLED_COMM, 0xb0 + page);	//设置页地址(0--7)
		oled_write_operate(OLED_COMM, 0x00);	//设置显示位置—列低地址
		oled_write_operate(OLED_COMM, 0x10);	//设置显示位置—列高地址
	
		for (i = 0; i < 128; i++)
		{
			oled_write_operate(OLED_DATA, 0xff);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_dis_one_char
//	功能说明: oled 显示单个字符
//	形    参: x：横坐标(列);y:纵坐标(行);str：显示字符
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 无
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_dis_one_char(uint8_t x, uint8_t y, uint8_t str)
{
	uint8_t i = 0;
	uint8_t ret = 0;
	//ret = str -32;
	ret = str - ' ';//得到偏移后的值,对ASCLL码进行一个减法.即在二维数组里找它的位置  
	
	if (x > (MAX_COLUMN - 1))	//列超过最大,更换到下一行
	{
		x = 0;
		if (SIZE == 8 )
		{
			y = y + 1;//针对8号的字符
		}
		if (SIZE == 16 )
		{
			y = y + 2;//针对16号的字符
		}
	}
	if (SIZE == 16 )
	{
		oled_set_pos(x, y);
		//16的字体分成两部分写入
		for (i = 0; i < 8; i++)
		{
			oled_write_operate(OLED_DATA, F8X16[ret*16+i]);
		}
		
		oled_set_pos(x, y + 1);
		for	(i = 0; i < 8; i++)
		{
			oled_write_operate(OLED_DATA, F8X16[ret*16+i+8]);
		}
	}
	else 
	{
		oled_set_pos(x, y + 1);
		for(i = 0; i < 6; i++)
		{
			oled_write_operate(OLED_DATA, F6x8[ret][i]);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_dis_str
//	功能说明: oled 显示多个字符
//	形    参: x：横坐标(列);y:纵坐标(行);str：显示字符串
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 无
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_dis_str(uint8_t x, uint8_t y, uint8_t *str)
{
	uint8_t i = 0;
	
	while(str[i] != '\0')
	{
		oled_dis_one_char(x, y, str[i]);
		x += 8;
		
		if(x > 120)
		{
			x = 0;
			y += 2;
		}
		i++;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_dis_chinese
//	功能说明: oled 显示汉字
//	形    参: x：横坐标(列);y:纵坐标(行);no：显示汉字位于编码位置
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 无
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_dis_chinese(uint8_t x, uint8_t y, uint8_t no)
{
	uint8_t i = 0;
	uint8_t addr = 0;
	
	oled_set_pos(x, y);
	for (i = 0; i < 16; i++)//数组行列寻址
	{
		oled_write_operate(OLED_DATA, TEST[2*no][i]);
		addr += 1;
	}
	
	oled_set_pos(x, y + 1);
	for (i = 0; i < 16; i++)
	{
		oled_write_operate(OLED_DATA, TEST[2*no+1][i]);
		addr += 1;
	}
	
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_dis_picture
//	功能说明: oled 显示图片
//	形    参: x：横坐标(列);y:纵坐标(行);no：显示汉字位于编码位置
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 无
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_dis_picture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *bmp)
{
	uint8_t x = 0;
	uint8_t y = 0;
	uint32_t i = 0;
	
	if ((y1 % 8) == 0)
	{
		y = y1 / 8;
	}
	else
	{
		y = y1 /8 + 1;
	}
	
	for (y = y0; y < y1; y++)	//每行(0 - 7 page)
	{
		oled_set_pos(x0, y);
		
		for (x = x0; x < x1; x++)	//0~128
		{
			oled_write_operate(OLED_DATA, bmp[i++]);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_dis_logo
//	功能说明: oled 显示图片logo
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 整屏幕显示
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_dis_logo(void)
{
	const uint8_t *pdata = dis_tmp_buff;
	uint8_t page = 0;
	uint8_t seg = 0;
	
	for (page = 0xb0; page < 0xb8; page++)
  	{
	    oled_write_operate(OLED_COMM, page);
	    oled_write_operate(OLED_COMM, 0x10);
	    oled_write_operate(OLED_COMM, 0x00);
		
		for (seg = 0; seg < 128; seg++)
		{
			oled_write_operate(OLED_DATA, *pdata++);
		}

//		if (page >= 0xB2 && page <= 0xB5)	//调整图片区域
//		{
//			for (seg = 0; seg < 128; seg++)
//			{
//				oled_write_operate(OLED_DATA, *pdata++);
//			}
//		}
//		else
//		{
//			for (seg = 0; seg < 128; seg++)
//			{
//				oled_write_operate(OLED_DATA, 0x00);
//			}
//		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_pow
//	功能说明: 计算 m的n次方
//	形    参: m：底数;n：指数
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 无
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint32_t oled_pow(uint8_t m, uint8_t n)
{
	uint32_t ret = 1;
	
	while(n--)
	{
		ret *= m;
	}
	
	return ret;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_dis_picture
//	功能说明: oled 数字
//	形    参: x：横坐标(列);y:纵坐标(行);num：显示数字;len：数字长度;size_num：数字大小
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 无
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_dis_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size_num)
{
	uint8_t i = 0;
	uint8_t temp = 0;
	uint8_t enshow = 0;
	
	for (i = 0; i < len; i++)
	{
		temp = (num / (oled_pow(10,len-i-1))) % 10;//把显示的数字一位一位取出来
		if((enshow == 0) && (i < (len-1)))
		{
			if(temp == 0)
			{
				oled_dis_one_char(x + (size_num / 2) * i, y, ' ');
				continue;
			}
			else
			{
				enshow = 1;
			}
		}
		oled_dis_one_char(x + (size_num / 2) * i, y, temp + '0');
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: oled_init
//	功能说明: oled 初始化
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-11
//  备    注: 外部调用初始化OLED
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void oled_init(void)
{
	oled_gpio_init();	//oled cs/dc/rst 三个io初始化
	
	OLED_RST_HIGH;
	delay_ms(200);
	OLED_RST_LOW;
	delay_ms(200);
	OLED_RST_HIGH; 
		
	oled_write_operate(OLED_COMM, 0xAE);	//关闭OLED
	oled_write_operate(OLED_COMM, 0x00);//设置列低位地址
	oled_write_operate(OLED_COMM, 0x10);//设置列高位地址
	oled_write_operate(OLED_COMM, 0x40);	//设置起始行地址及映射RAM显示起始行 (0x00~0x3F)
	oled_write_operate(OLED_COMM, 0x81);	//对比度设置
	oled_write_operate(OLED_COMM, 0xCF); 	// Set SEG Output Current Brightness
	oled_write_operate(OLED_COMM, 0xA1);	//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	oled_write_operate(OLED_COMM, 0xC8);	//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	oled_write_operate(OLED_COMM, 0xA8);	//设置驱动路数(1 to 64)
	oled_write_operate(OLED_COMM, 0x3f);	//--1/64 duty
	oled_write_operate(OLED_COMM, 0xD3);	//-设置显示偏移(0x00~0x3F)
	oled_write_operate(OLED_COMM, 0x00);	//-not offset
	oled_write_operate(OLED_COMM, 0xd5);	//--set display clock divide ratio/oscillator frequency
	oled_write_operate(OLED_COMM, 0x80);	//--set divide ratio, Set Clock as 100 Frames/Sec
	oled_write_operate(OLED_COMM, 0xD9);	//--set pre-charge period
	oled_write_operate(OLED_COMM, 0xF1);	//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_write_operate(OLED_COMM, 0xDA);	//--set com pins hardware configuration
	oled_write_operate(OLED_COMM, 0x12);
	oled_write_operate(OLED_COMM, 0xDB);	//--set vcomh
	oled_write_operate(OLED_COMM, 0x40);	//Set VCOM Deselect Level
	oled_write_operate(OLED_COMM, 0x20);	//设置页地址模式(0x00/0x01/0x02)
	oled_write_operate(OLED_COMM, 0x02);	//
	oled_write_operate(OLED_COMM, 0x8D);	//--set Charge Pump enable/disable
	oled_write_operate(OLED_COMM, 0x14);	//--set(0x10) disable
	oled_write_operate(OLED_COMM, 0xA4);	//显示开启(显示:A4;无显示:A5)
	oled_write_operate(OLED_COMM, 0xA7);	// 背景正反向显示 (0xa6:正显;a7:反显) 
	oled_write_operate(OLED_COMM, 0xAF);	//打开显示
	
	oled_write_operate(OLED_COMM, 0xAF); 	//display ON(on:AF;off:AE)
	oled_dis_clear();
	oled_set_pos(0, 0); 	
}  

#endif /* __PRJ_STM32F10X_DRVOLED_C__ */
