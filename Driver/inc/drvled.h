#ifndef _DRVLED_H_
#define _DRVLED_H_ 

//#include "Typedef.h"
//#include "stm32f10x.h"


#define LED0 PAout(8)	// PA8
#define LED1 PDout(2)	// PD2	

#define LED0_CON_IO		GPIO_Pin_8
#define LED1_CON_IO		GPIO_Pin_2
#define LED0_GPIO_PORT	GPIOA
#define LED1_GPIO_PORT	GPIOD

#define LED_GPIO_CLK_ALLENABLE  (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD)

extern void Bsp_LedInit(void);//≥ı ºªØ
extern void Bsp_LedOn(uint8_t _no);
extern void Bsp_LedOff(uint8_t _no);
extern void Bsp_LedToggle(uint8_t _no);
extern void Bsp_LedTest(uint16_t _uiTime);

#endif	//_DRVLED_H_
