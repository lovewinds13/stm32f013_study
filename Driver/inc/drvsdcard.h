#ifndef __DRVSDCARD_H__
#define __DRVSDCARD_H__

#define SD_CS_IO     		(GPIO_Pin_3)
#define PORT_SD_GPIO		GPIOA
#define RCC_PCLK_SD_GPIO	RCC_APB2Periph_GPIOA
#define SD_CS_0()			(GPIO_ResetBits(PORT_SD_GPIO, SD_CS_IO))		
#define SD_CS_1() 			(GPIO_SetBits(PORT_SD_GPIO, SD_CS_IOS))

extern void sd_gpio_init(void);

#endif	//__DRVSDCARD_H__
