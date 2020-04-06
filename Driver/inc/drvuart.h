#ifndef __DRVUART_H__
#define __DRVUART_H__


//GPIO 部分
#define RCC_PCLK_USART1_GPIO		RCC_APB2Periph_GPIOA
#define USART1_TXD_IO				GPIO_Pin_9
#define USART1_RXD_IO				GPIO_Pin_10
#define USART1_TXD_PORT				GPIOA
#define USART1_RXD_PORT				GPIOA

#define RCC_PCLK_USART2_GPIO		RCC_APB2Periph_GPIOA
#define USART2_TXD_IO				GPIO_Pin_2
#define USART2_RXD_IO				GPIO_Pin_3
#define USART2_TXD_PORT				GPIOA
#define USART2_RXD_PORT				GPIOA

#define RCC_PCLK_USART3_GPIO		RCC_APB2Periph_GPIOB
#define USART3_TXD_IO				GPIO_Pin_10
#define USART3_RXD_IO				GPIO_Pin_11
#define USART3_TXD_PORT				GPIOB
#define USART3_RXD_PORT				GPIOB

#define RCC_PCLK_USART4_GPIO		RCC_APB2Periph_GPIOC
#define USART4_TXD_IO				GPIO_Pin_10
#define USART4_RXD_IO				GPIO_Pin_11
#define USART4_TXD_PORT				GPIOC
#define USART4_RXD_PORT	            GPIOC

#define RCC_PCLK_USART5_GPIO		(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
#define USART5_TXD_IO				GPIO_Pin_12
#define USART5_RXD_IO				GPIO_Pin_2
#define USART5_TXD_PORT				GPIOC
#define USART5_RXD_PORT	            GPIOD

//UART 部分
#define RCC_PCLK_USART1				RCC_APB2Periph_USART1
#define RCC_PCLK_USART2				RCC_APB1Periph_USART2
#define RCC_PCLK_USART3				RCC_APB1Periph_USART3
#define RCC_PCLK_USART4				RCC_APB1Periph_UART4
#define RCC_PCLK_USART5				RCC_APB1Periph_UART5

//函数声明部分
extern void uart_init(uint8_t uart_chl, uint32_t uart_bound);
extern bool uart_send_byte(uint8_t uart_chl, uint8_t send_byte);
extern bool uart_send_serial_bytes(uint8_t uart_chl, uint8_t *pbdata, uint16_t length);
extern uint8_t uart_recv_byte(uint8_t uart_chl);
extern bool uart_recv_serial_bytes(uint8_t uart_chl, uint8_t *pbdata, uint16_t length);

extern void uart_test(void);

#endif	//__DRVUART_H__

