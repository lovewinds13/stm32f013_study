#ifndef __PRJ_STM32F10X_DRVNRF2401_C__
#define __PRJ_STM32F10X_DRVNRF2401_C__

//---------------------------------------------------------------------------------------------------------------------------------------------
//平    台:				STM32F10X
//文    件:    		 	drvadc.c
//作    者:       		霁风AI
//库版 本:   			Vxxx
//文件版本:   			V1.0.0
//日   期:      		2020年03月11日
//说   明:      	 	ADC驱动实现
//----------------------------------------------------------------------------------------------------------------------------------------------

#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h" 
#include "stm32f10x_rcc.h"
#include "drvnrf2401.h"
#include "hardware_spi.h"
#include "delay.h"

const uint8_t TX_ADDRESS_X[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};//发送端地址
const uint8_t RX_ADDRESS_X[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};//接收端地址

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_gpio_init
//	功能说明: nrf24l01 gpio 初始化
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-11
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void nrf24l01_gpio_init(void)
{
    GPIO_InitTypeDef gpio_config_init;

	RCC_APB2PeriphClockCmd(RCC_PCLK_NRF_GPIO_CS, ENABLE);		//开启NRF CS GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_PCLK_NRF_GPIO_CE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_PCLK_NRF_GPIO_IRQ, ENABLE);

	gpio_config_init.GPIO_Pin 	= NRF_CS_IO;	//NRF_CS_IO IO初始化
	gpio_config_init.GPIO_Mode 	= GPIO_Mode_Out_PP;  //推挽输出
	gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_NRF_CS, &gpio_config_init);

	GPIO_SetBits(PORT_NRF_CS, NRF_CS_IO);	//IO初始状态都设置为高电平
	
	gpio_config_init.GPIO_Pin 	= NRF_CE_IO;
	gpio_config_init.GPIO_Mode 	= GPIO_Mode_Out_PP;
	gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_NRF_CE, &gpio_config_init);
	
	GPIO_SetBits(PORT_NRF_CE, NRF_CE_IO);
	
	gpio_config_init.GPIO_Pin 	= NRF_IRQ_IO;
	gpio_config_init.GPIO_Mode 	= GPIO_Mode_IPU;//上拉输入
	gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_NRF_IRQ, &gpio_config_init);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_write_reg
//	功能说明: nrf24l01 写寄存器
//	形    参: 	reg_no：寄存器
//				reg_val：写入寄存器数值
//	返 回 值: 	status：寄存器状态值
//	日    期: 2020-04-15
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint8_t nrf24l01_write_reg(uint8_t reg_no, uint8_t reg_val)
{
	uint8_t status = 0;
	
	NRF_CS_0();
	status = spi_master_send_recv_byte(1, reg_no);
	spi_master_send_recv_byte(1, reg_val);
	NRF_CS_1();
	
	return status;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_read_reg
//	功能说明: nrf24l01 读寄存器
//	形    参: 	reg_no：寄存器
//	返 回 值: 	reg_val：寄存器值
//	日    期: 2020-04-15
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint8_t nrf24l01_read_reg(uint8_t reg_no)
{
	uint8_t reg_val = 0;
	
	NRF_CS_0();
	spi_master_send_recv_byte(1, reg_no);
	reg_val = spi_master_send_recv_byte(1, 0xff);	
	NRF_CS_1();
	
	return reg_val;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_read_buff
//	功能说明: nrf24l01 读数据
//	形    参: 	reg_no：寄存器
//				pbdata：数据指针
//				read_length：读取数据长度
//	返 回 值: 	status：寄存器状态值
//	日    期: 2020-04-16
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint8_t nrf24l01_read_buff(uint8_t reg_no, uint8_t *pbdata, uint16_t read_length)
{
	uint8_t status = 0;
	uint16_t i = 0;
	
	NRF_CS_0();
	
	status = spi_master_send_recv_byte(1, reg_no);
	
	for (i = 0; i < read_length; i++)
	{
		*pbdata++ = spi_master_send_recv_byte(1, 0xff);	//发送ff得到spi的时钟
	}
	
	NRF_CS_1();
	
	return status;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_write_buff
//	功能说明: nrf24l01 写数据
//	形    参: 	reg_no：寄存器
//				pbdata：数据指针
//				write_length：读取数据长度
//	返 回 值: 	status：寄存器状态值
//	日    期: 2020-04-16
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint8_t nrf24l01_write_buff(uint8_t reg_no, uint8_t *pbdata, uint16_t write_length)
{
	uint8_t status = 0;
	uint16_t i = 0;
	
	NRF_CS_0();
	
	status = spi_master_send_recv_byte(1, reg_no);
	
	for (i = 0; i < write_length; i++)
	{
		spi_master_send_recv_byte(1, pbdata[i]);	//连续写入数据
	}
	
	NRF_CS_1();
	
	return status;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_tx_mode
//	功能说明: nrf24l01 发送模式设置
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-16
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void nrf24l01_tx_mode(void)
{
	NRF_CE_0();
	
	nrf24l01_write_buff(W_REGISTER + TX_ADDR, (uint8_t *)TX_ADDRESS_X, TX_ADR_WIDTH);	//写TX节点地址
	nrf24l01_write_buff(W_REGISTER + RX_ADDR_P0, (uint8_t *) RX_ADDRESS_X, RX_ADR_WIDTH);	//写RX节点地址,为了自动使能ACK
	nrf24l01_write_reg(W_REGISTER + EN_AA, 0x01);	//使能通道0自动应答
	nrf24l01_write_reg(W_REGISTER + EN_RXADDR, 0x01);	//使能通道0接收地址
	nrf24l01_write_reg(W_REGISTER + SETUP_PETR, 0x1a);	//设置自动重发间隔时间:500us+86us,最大重大次数:10次
	nrf24l01_write_reg(W_REGISTER + RF_CH, 40);	//设置通道为40
	nrf24l01_write_reg(W_REGISTER + RF_SETUP, 0x0f);	//设置发射参数:0dB增益;2Mnps;低噪声增益开启
	nrf24l01_write_reg(W_REGISTER + NRF24L01_CONFIG, 0x0e);	//基本参数:PWR_UP;EN_CRC;16BIT_CRC;发送模式;开启所有中断
	
	NRF_CE_1();
	
	delay_us(12);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_rx_mode
//	功能说明: nrf24l01 接收模式设置
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-16
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void nrf24l01_rx_mode(void)
{
	NRF_CE_0();
	
	nrf24l01_write_buff(W_REGISTER + RX_ADDR_P0, (uint8_t *) RX_ADDRESS_X, RX_ADR_WIDTH);	//写RX地址节点
	nrf24l01_write_reg(W_REGISTER + EN_AA, 0x01);	//使能通道0自动应答
	nrf24l01_write_reg(W_REGISTER + EN_RXADDR, 0x01);	//使能通道0接收地址
	nrf24l01_write_reg(W_REGISTER + RF_CH, 40);	//设置RF通信频率
	nrf24l01_write_reg(W_REGISTER + RX_PW_P0, RX_PLOAD_WIDTH);	//选择通道0有效数据宽度
	nrf24l01_write_reg(W_REGISTER + RF_SETUP, 0x0f); //设置TX发射参数:0db增益,2Mbps,低噪声增益开启
	nrf24l01_write_reg(W_REGISTER + NRF24L01_CONFIG, 0x0f);	//基本参数:PWR_UP;EN_CRC;16BIT_CRC;接收模式;开启所有中断
	
	NRF_CE_1();//CE为高,进入接收模式
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_tx_packet
//	功能说明: nrf24l01 发送数据设置
//	形    参: 	tx_buff：发送的数据
//	返 回 值: 操作状态(MAX_TX：发送到最大次数;TX_OK：发送完成；TX_FAIL：发送失败)
//	日    期: 2020-04-16
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint8_t nrf24l01_tx_packet(uint8_t *tx_buff)
{
	uint8_t ret_val = 0;

	NRF_CE_0();
	
	nrf24l01_write_buff(W_TX_PAYLOAD, tx_buff, TX_PLOAD_WIDTH);	//写数据到txbuf,32字节
	
	NRF_CE_1();	//启动发送
	
	while (GET_NRF_IRQ_STATUS);//等待发送完成
	
	ret_val = nrf24l01_read_reg(NRF24L01_STATUS);	//读取状态寄存器的值
	nrf24l01_write_reg(W_REGISTER + NRF24L01_STATUS, ret_val);	//清除TX_DS or MAX_RT的中断标志
	
	if (ret_val & MAX_TX)	//达到最大重发次数
	{
		nrf24l01_write_reg(FLUSH_TX, 0xff);	//清除TX FIFO寄存器
		
		return MAX_TX;
	}
	
	if(ret_val & TX_OK)	//发送完成
	{
		return TX_OK;
	}
	
	return TX_FAIL;//发送失败
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_rx_packet
//	功能说明: nrf24l01 接收数据设置
//	形    参: 	rx_buff：发送的数据
//	返 回 值: 操作状态(RX_SUCCESS：接收成功;RX_FAIL：接收失败)
//	日    期: 2020-04-16
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint8_t nrf24l01_rx_packet(uint8_t *rx_buf)
{
	uint8_t ret_val = 0;

	NRF_CE_1();
	
	while(GET_NRF_IRQ_STATUS);
	
	NRF_CE_0();

	ret_val = nrf24l01_read_reg(NRF24L01_STATUS);	//读取状态寄存器的值
	nrf24l01_write_reg(W_REGISTER+NRF24L01_STATUS, ret_val);	//清除TX_DS or MAX_RT的中断标志
	
	if(ret_val & RX_OK)	//接收到数据
	{
		nrf24l01_read_buff(R_RX_PAYLOAD, rx_buf, RX_PLOAD_WIDTH);	//读取数据
		nrf24l01_write_reg(FLUSH_RX, 0xff);	//清除RX FIFO寄存器
		
		return RX_SUCCESS;
	}
	
	return RX_FAIL;	//没有接收到数据
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_check_state
//	功能说明: nrf24l01 状态检测
//	形    参: 无
//	返 回 值: 操作状态(0：ok;1：fail)
//	日    期: 2020-04-16
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
uint8_t nrf24l01_check_state(void)
{
	uint8_t buff[5]={0xa5, 0xa5, 0xa5, 0xa5, 0xa5};
	uint8_t i = 0;

	nrf24l01_write_buff(W_REGISTER+TX_ADDR, buff, 5);//写入5字节的地址
	nrf24l01_read_buff(TX_ADDR, buff, 5);
	
	for(i = 0; i < 5; i++)
	{
		if(buff[i] != 0xa5)
		{
			break;
		}
	}
	
	if(i != 5)
	{
		return 1;
	}
	
	return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_power_down_mode
//	功能说明: nrf24l01 进入下电模式
//	形    参: 无
//	返 回 值: 操作状态(0：ok;1：fail)
//	日    期: 2020-04-16
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void nrf24l01_power_down_mode(void)
{
	NRF_CS_0();
	nrf24l01_write_reg(NRF24L01_CONFIG, 0x00);
	NRF_CS_1();
	
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//	函 数 名: nrf24l01_init
//	功能说明: nrf24l01 初始化
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-16
//  备    注: 
//	作    者: by 霁风AI
//---------------------------------------------------------------------------------------------------------------------------------------------
void nrf24l01_init(void)
{
	nrf24l01_gpio_init();
	
	NRF_CS_1();
	NRF_CE_0();
}

#endif /* __PRJ_STM32F10X_DRVNRF2401_C__ */
