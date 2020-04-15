#ifndef __DRVNRF2401_H__
#define __DRVNRF2401_H__

#define RCC_PCLK_NRF_GPIO_CS	RCC_APB2Periph_GPIOC
#define RCC_PCLK_NRF_GPIO_CE	RCC_APB2Periph_GPIOA
#define RCC_PCLK_NRF_GPIO_IRQ	RCC_APB2Periph_GPIOA

#define NRF_CS_IO     		(GPIO_Pin_4)	//CS:PC4
#define PORT_NRF_CS			GPIOC
#define NRF_CS_0()			(GPIO_ResetBits(PORT_NRF_CS, NRF_CS_IO))		
#define NRF_CS_1() 			(GPIO_SetBits(PORT_NRF_CS, NRF_CS_IO))

#define NRF_CE_IO     		(GPIO_Pin_4)	//CE:PA4
#define PORT_NRF_CE			GPIOA
#define NRF_CE_0()			(GPIO_ResetBits(PORT_NRF_CE, NRF_CE_IO))		
#define NRF_CE_1() 			(GPIO_SetBits(PORT_NRF_CE, NRF_CE_IO))

#define NRF_IRQ_IO     			(GPIO_Pin_1)	//CE:PA1
#define PORT_NRF_IRQ			GPIOA
#define NRF_IRQ_0()				(GPIO_ResetBits(PORT_NRF_IRQ, NRF_IRQ_IO))		
#define NRF_IRQ_1() 			(GPIO_SetBits(PORT_NRF_IRQ, NRF_IRQ_IO))
#define GET_NRF_IRQ_STATUS		(GPIO_ReadInputDataBit(PORT_NRF_IRQ, NRF_IRQ))

extern void nrf24l01_gpio_init(void);

#endif	//__DRVNRF2401_H__

