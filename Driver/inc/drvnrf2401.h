#ifndef __DRVNRF2401_H__
#define __DRVNRF2401_H__

#define NRF_CS_IO     		(GPIO_Pin_4)
#define PORT_NRF_GPIO		GPIOC
#define RCC_PCLK_NRF_GPIO	RCC_APB2Periph_GPIOC
#define NRF_CS_0()			(GPIO_ResetBits(PORT_NRF_GPIO, NRF_CS_IO))		
#define NRF_CS_1() 			(GPIO_SetBits(PORT_NRF_GPIO, NRF_CS_IO))


extern void nrf24l01_gpio_init(void);

#endif	//__DRVNRF2401_H__

