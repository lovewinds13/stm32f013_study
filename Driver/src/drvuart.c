//--------------------------------------------------------------------------------------------------------
//
//	模块名称 : uart收发操作
//	文件名称 : drvuart.c
//	版    本 : V1.0
//	说    明 : 
//				(1) drvuart.c 收发,轮询+中断
//				(2) 
//				(3) 
//				(4) 
//
//	修改记录 :
//		版本号     日期        作者     说明
//		V1.0    2020-03-28   霁风AI  正式发布
//		V1.1    
//		V1.2	
//		V1.3	
//
//	Copyright (C), 2020-2030, 微信公众号——TECHTIMES
//
//--------------------------------------------------------------------------------------------------------

#ifndef __PRJ_STM32F10X_DRVUART_C__
#define __PRJ_STM32F10X_DRVUART_C__

#include "Typedef.h"
#include "drvuart.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <string.h>

int fputc(int ch, FILE *f)
{      
//	while((USART1->SR & 0X40) == 0);//循环发送,直到发送完毕   
//    USART1->DR = (uint8_t)ch;      
	
//	 uart_send_byte(1, (uint8_t)ch);

	uart_send_serial_bytes(1, (uint8_t *)&ch, 1);
	
	return ch;
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: uart_gpio_init
//	功能说明: 串口GPIO初始化
//	形    参: 	uart_chl：串口编号
//	返 回 值: 无
//	日    期: 2020-03-25
//  备    注: 串口GPIO初始化
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
static void uart_gpio_init(uint8_t uart_chl)
{
    GPIO_InitTypeDef gpio_config_init;

    if (uart_chl == 1)
    {
        RCC_APB2PeriphClockCmd(RCC_PCLK_USART1_GPIO, ENABLE);	//使能UART1 GPIO时钟,APB2总线

		gpio_config_init.GPIO_Pin 	= USART1_TXD_IO; //PA9
		gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
		gpio_config_init.GPIO_Mode 	= GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(USART1_TXD_PORT, &gpio_config_init);//初始化GPIOA9

		gpio_config_init.GPIO_Pin 	= USART1_RXD_IO;//PA10
		gpio_config_init.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(USART1_RXD_PORT, &gpio_config_init);//初始化GPIOA10
    }
    else if (uart_chl == 2)
    {
        RCC_APB2PeriphClockCmd(RCC_PCLK_USART2_GPIO, ENABLE);	//使能UART2 GPIO时钟,ABP2总线

		gpio_config_init.GPIO_Pin = USART2_TXD_IO; //PA2
		gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
		gpio_config_init.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(USART2_TXD_PORT, &gpio_config_init);//初始化GPIOA2

		gpio_config_init.GPIO_Pin = USART2_RXD_IO;//PA3
		gpio_config_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(USART2_RXD_PORT, &gpio_config_init);//初始化GPIOA3  
    }
    else if (uart_chl == 3)
    {
        RCC_APB2PeriphClockCmd(RCC_PCLK_USART3_GPIO, ENABLE);	//使能UART3 GPIO时钟,ABP2总线

		gpio_config_init.GPIO_Pin = USART3_TXD_IO; //PB10
		gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
		gpio_config_init.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(USART3_TXD_PORT, &gpio_config_init);//初始化GPIOB10

		gpio_config_init.GPIO_Pin = USART3_RXD_IO;//PB11
		gpio_config_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(USART3_RXD_PORT, &gpio_config_init);//初始化GPIOB11 
    }
    else if (uart_chl == 4)
    {
        RCC_APB2PeriphClockCmd(RCC_PCLK_USART4_GPIO, ENABLE);	//使能UART4 GPIO时钟,ABP2总线

		gpio_config_init.GPIO_Pin = USART4_TXD_IO; //PC10
		gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
		gpio_config_init.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(USART4_TXD_PORT, &gpio_config_init);//初始化GPIOC10

		gpio_config_init.GPIO_Pin = USART4_RXD_IO;//PC11
		gpio_config_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(USART4_RXD_PORT, &gpio_config_init);//初始化GPIOC11 
    }
     else if (uart_chl == 5)
    {
        RCC_APB2PeriphClockCmd(RCC_PCLK_USART5_GPIO, ENABLE);	//使能UART4 GPIO时钟,ABP2总线

		gpio_config_init.GPIO_Pin = USART5_TXD_IO; //PC12
		gpio_config_init.GPIO_Speed = GPIO_Speed_50MHz;
		gpio_config_init.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(USART5_TXD_PORT, &gpio_config_init);	//初始化GPIOC12

		gpio_config_init.GPIO_Pin = USART5_RXD_IO;	//PC11
		gpio_config_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
		GPIO_Init(USART5_RXD_PORT, &gpio_config_init);	//初始化GPIOD2
    }
    else 
    {
//        printf("error! \r\n");
    }
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: uart_config
//	功能说明: 串口配置参数初始化
//	形    参: 	uart_chl：串口编号
//              uart_bound：波特率
//	返 回 值: 无
//	日    期: 2020-03-25
//  备    注: 串口编号从1-5(UART1-5),串口中断使能在Nvic设置里面配置
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
static void uart_config(uint8_t uart_chl, uint32_t uart_bound)
{
    USART_InitTypeDef uart_config_init;

    if (uart_chl == 1)
    {
		RCC_APB2PeriphClockCmd(RCC_PCLK_USART1, ENABLE);
		
		uart_config_init.USART_BaudRate              = uart_bound;	//串口波特率
		uart_config_init.USART_WordLength            = USART_WordLength_8b;	//字长为8位数据格式
		uart_config_init.USART_StopBits              = USART_StopBits_1;	//一个停止位
		uart_config_init.USART_Parity                = USART_Parity_No;	//无奇偶校验位
		uart_config_init.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;	//无硬件数据流控制
		uart_config_init.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		USART_Init(USART1, &uart_config_init); //初始化串口1
		// USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断(串口参数初始化之后才能开启中断)
		USART_Cmd(USART1, ENABLE); 		//使能串口1 
    }
    else if (uart_chl == 2)
    {
		RCC_APB2PeriphClockCmd(RCC_PCLK_USART2, ENABLE);
		
		uart_config_init.USART_BaudRate             = uart_bound;//串口波特率
		uart_config_init.USART_WordLength           = USART_WordLength_8b;//字长为8位数据格式
		uart_config_init.USART_StopBits             = USART_StopBits_1;//一个停止位
		uart_config_init.USART_Parity               = USART_Parity_No;//无奇偶校验位
		uart_config_init.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;//无硬件数据流控制
		uart_config_init.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		USART_Init(USART2, &uart_config_init); //初始化串口1
		// USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_Cmd(USART2, ENABLE); 		//使能串口1 
    }
    else if (uart_chl == 3)
    {
		RCC_APB2PeriphClockCmd(RCC_PCLK_USART3, ENABLE);
		
		uart_config_init.USART_BaudRate             = uart_bound;//串口波特率
		uart_config_init.USART_WordLength           = USART_WordLength_8b;//字长为8位数据格式
		uart_config_init.USART_StopBits             = USART_StopBits_1;//一个停止位
		uart_config_init.USART_Parity               = USART_Parity_No;//无奇偶校验位
		uart_config_init.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;//无硬件数据流控制
		uart_config_init.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		USART_Init(USART3, &uart_config_init); //初始化串口1
		// USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_Cmd(USART3, ENABLE); 		//使能串口1 
    }
    else if (uart_chl == 4)
    {
		RCC_APB2PeriphClockCmd(RCC_PCLK_USART4, ENABLE);
		
		uart_config_init.USART_BaudRate             = uart_bound;//串口波特率
		uart_config_init.USART_WordLength           = USART_WordLength_8b;//字长为8位数据格式
		uart_config_init.USART_StopBits             = USART_StopBits_1;//一个停止位
		uart_config_init.USART_Parity               = USART_Parity_No;//无奇偶校验位
		uart_config_init.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;//无硬件数据流控制
		uart_config_init.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		USART_Init(UART4, &uart_config_init); //初始化串口1
//		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_Cmd(UART4, ENABLE); 		//使能串口1 
    }
    else if (uart_chl == 5)
    {
		RCC_APB2PeriphClockCmd(RCC_PCLK_USART5, ENABLE);
		
		uart_config_init.USART_BaudRate             = uart_bound;//串口波特率
		uart_config_init.USART_WordLength           = USART_WordLength_8b;//字长为8位数据格式
		uart_config_init.USART_StopBits             = USART_StopBits_1;//一个停止位
		uart_config_init.USART_Parity               = USART_Parity_No;//无奇偶校验位
		uart_config_init.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;//无硬件数据流控制
		uart_config_init.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

		USART_Init(UART5, &uart_config_init); //初始化串口1
//		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
		USART_Cmd(UART5, ENABLE); 		//使能串口1 
    }
    else 
    {
//        printf("error! \r\n");
    }
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: uart_irq_set
//	功能说明: 串口中断初始化配置
//	形    参: 	uart_chl：串口编号
//	返 回 值: 无
//	日    期: 2020-03-27
//  备    注: 无
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
static void uart_irq_set(uint8_t uart_chl)
{
	NVIC_InitTypeDef uart_nvic_config = {0};

	if (uart_chl == 1)
    {
        uart_nvic_config.NVIC_IRQChannel 					= USART1_IRQn;
		uart_nvic_config.NVIC_IRQChannelPreemptionPriority 	= 3 ;//抢占优先级3
		uart_nvic_config.NVIC_IRQChannelSubPriority 		= 3;		//子优先级3
		uart_nvic_config.NVIC_IRQChannelCmd 				= ENABLE;			//IRQ通道使能
		NVIC_Init(&uart_nvic_config);	//根据指定的参数初始化NVIC寄存器

		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
    }
    else if (uart_chl == 2)
    {
        uart_nvic_config.NVIC_IRQChannel 					= USART2_IRQn;
		uart_nvic_config.NVIC_IRQChannelPreemptionPriority 	= 3 ;//抢占优先级3
		uart_nvic_config.NVIC_IRQChannelSubPriority 		= 3;		//子优先级3
		uart_nvic_config.NVIC_IRQChannelCmd 				= ENABLE;			//IRQ通道使能
		NVIC_Init(&uart_nvic_config);	//根据指定的参数初始化NVIC寄存器

		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
    }
    else if (uart_chl == 3)
    {
        uart_nvic_config.NVIC_IRQChannel 					= USART3_IRQn;
		uart_nvic_config.NVIC_IRQChannelPreemptionPriority 	= 3 ;//抢占优先级3
		uart_nvic_config.NVIC_IRQChannelSubPriority 		= 3;		//子优先级3
		uart_nvic_config.NVIC_IRQChannelCmd 				= ENABLE;			//IRQ通道使能
		NVIC_Init(&uart_nvic_config);	//根据指定的参数初始化NVIC寄存器

		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
    }
    else if (uart_chl == 4)
    {
        uart_nvic_config.NVIC_IRQChannel 					= UART4_IRQn;
		uart_nvic_config.NVIC_IRQChannelPreemptionPriority 	= 3 ;//抢占优先级3
		uart_nvic_config.NVIC_IRQChannelSubPriority 		= 3;		//子优先级3
		uart_nvic_config.NVIC_IRQChannelCmd 				= ENABLE;			//IRQ通道使能
		NVIC_Init(&uart_nvic_config);	//根据指定的参数初始化NVIC寄存器

		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接受中断
    }
    else if (uart_chl == 5)
    {
        uart_nvic_config.NVIC_IRQChannel 					= UART5_IRQn;
		uart_nvic_config.NVIC_IRQChannelPreemptionPriority 	= 3 ;//抢占优先级3
		uart_nvic_config.NVIC_IRQChannelSubPriority 			= 3;		//子优先级3
		uart_nvic_config.NVIC_IRQChannelCmd 				= ENABLE;			//IRQ通道使能
		NVIC_Init(&uart_nvic_config);	//根据指定的参数初始化NVIC寄存器

		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
    }
    else 
    {
//        printf("error! \r\n");
    }
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: uart_init
//	功能说明: 串口G配置参数初始化
//	形    参: 	uart_chl：串口编号
//              uart_bound：波特率
//	返 回 值: 无
//	日    期: 2020-03-25
//  备    注: 统一调用底层接口(三部分：GPIO + UART + NVIC)
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
void uart_init(uint8_t uart_chl, uint32_t uart_bound)
{
    if (uart_chl == 1)
    {
        uart_gpio_init(uart_chl);
        uart_config(uart_chl, uart_bound);
		uart_irq_set(uart_chl);
    }
    else if (uart_chl == 2)
    {
        uart_gpio_init(uart_chl);
        uart_config(uart_chl, uart_bound);
		uart_irq_set(uart_chl);
    }
    else if (uart_chl == 3)
    {
        uart_gpio_init( uart_chl);
        uart_config(uart_chl, uart_bound);
		uart_irq_set(uart_chl);
    }
    else if (uart_chl == 4)
    {
        uart_gpio_init(uart_chl);
        uart_config(uart_chl, uart_bound);
		uart_irq_set(uart_chl);
    }
    else if (uart_chl == 5)
    {
        uart_gpio_init(uart_chl);
        uart_config(uart_chl, uart_bound);
		uart_irq_set(uart_chl);
    }
    else 
    {
        printf("error! \r\n");
    }
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: uart_send_byte
//	功能说明: 串口发送一字节数据
//	形    参: 	uart_chl：串口编号
//              send_byte：发送的数据
//	返 回 值: 无
//	日    期: 2020-03-27
//  备    注: 无
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
bool uart_send_byte(uint8_t uart_chl, uint8_t send_byte)
{
	if (uart_chl == 1)
    {
	#if 0
		while((USART1->SR & 0X40) == 0);//循环发送,直到发送完毕   
		USART1->DR = (uint8_t)send_byte; 
	#else
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//等待串口发送标志位置位

		USART_SendData(USART1, (uint8_t)send_byte);	//发送数据

	#endif
    }
    else if (uart_chl == 2)
    {
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	//等待串口发送标志位置位

		USART_SendData(USART2, (uint8_t)send_byte);	//发送数据
    }
    else if (uart_chl == 3)
    {
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	//等待串口发送标志位置位

		USART_SendData(USART3, (uint8_t)send_byte);	//发送数据
    }
    else if (uart_chl == 4)
    {
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);	//等待串口发送标志位置位

		USART_SendData(UART4, (uint8_t)send_byte);	//发送数据
    }
    else if (uart_chl == 5)
    {
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);	//等待串口发送标志位置位

		USART_SendData(UART5, (uint8_t)send_byte);	//发送数据
    }
    else 
    {
        printf("error! \r\n");
    }

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: uart_send_serial_bytes
//	功能说明: 串口发送多字节数据
//	形    参: 	uart_chl：串口编号
//              pbdata：发送的数据地址
//				length：发送数据的长度
//	返 回 值: 无
//	日    期: 2020-03-28
//  备    注: 无
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
bool uart_send_serial_bytes(uint8_t uart_chl, uint8_t *pbdata, uint16_t length)
{
	for (uint16_t i = 0; i < length; i++)
	{
		uart_send_byte(uart_chl, pbdata[i]);
	}
	
	// while (length--)
	// {
	// 	uart_send_byte(uart_chl, *pbdata++);
	// }

	return true;
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: uart_recv_byte
//	功能说明: 串口接收一字节数据
//	形    参: 	uart_chl：串口编号
//	返 回 值: recv_byte：接收的数据
//	日    期: 2020-03-28
//  备    注: 无
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
uint8_t uart_recv_byte(uint8_t uart_chl)
{
	uint8_t recv_byte = 0;

	if (uart_chl == 1)
    {
	#if 1
		while((USART1->SR & 0X20) == 0);//循环发送,直到发送完毕   
		recv_byte = USART1->DR; 
	#else
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);	//等待串口接收标志位置位
		recv_byte = USART_ReceiveData(USART1);	//接收数据

	#endif
    }
    else if (uart_chl == 2)
    {
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);	//等待串口接收标志位置位
		recv_byte = USART_ReceiveData(USART2);	//接收数据
    }
    else if (uart_chl == 3)
    {
		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);	//等待串口接收标志位置位
		recv_byte = USART_ReceiveData(USART3);	//接收数据
    }
    else if (uart_chl == 4)
    {
		while (USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET);	//等待串口接收标志位置位
		recv_byte = USART_ReceiveData(UART4);	//接收数据
    }
    else if (uart_chl == 5)
    {
		while (USART_GetFlagStatus(UART5, USART_FLAG_RXNE) == RESET);	//等待串口接收标志位置位
		recv_byte = USART_ReceiveData(UART5);	//接收数据
    }
    else 
    {
        printf("error! \r\n");
    }
	
	return recv_byte;
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: uart_recv_serial_bytes
//	功能说明: 串口接收多字节数据
//	形    参: 	uart_chl：串口编号
//              pbdata：发送的数据地址
//				length：发送数据的长度
//	返 回 值: 无
//	日    期: 2020-03-28
//  备    注: 无
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
bool uart_recv_serial_bytes(uint8_t uart_chl, uint8_t *pbdata, uint16_t length)
{
	for (uint16_t i = 0; i < length; i++)
	{
		*pbdata++ = uart_recv_byte(1);
	}
	
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: USART1_IRQHandler
//	功能说明: 串口中断服务函数
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-28
//  备    注: 无
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)	//检查接收中断标志位
	{	
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
		uart_test();	//接收的数据处理

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	//清除串口接收中断标志
	}
}

//--------------------------------------------------------------------------------------------------------------------------
//	函 数 名: uart_test
//	功能说明: 串口收发测试函数
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-28
//  备    注: 无
//	作    者: by 霁风AI
//-------------------------------------------------------------------------------------------------------------------------
void uart_test(void)
{
	uint8_t tmp_buff[6] = {0};
	uint8_t err_flag[6] = "error";

	uart_recv_serial_bytes(1, tmp_buff, sizeof(tmp_buff));

	if (tmp_buff[0] == 0x55)
	{
		uart_send_serial_bytes(1, tmp_buff, sizeof(tmp_buff));
	}
	else
	{
		uart_send_serial_bytes(1, err_flag, 6);
	}
}
#endif /* __PRJ_STM32F10X_DRVUART_C__ */
