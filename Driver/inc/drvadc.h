#ifndef __DRVADC_H__
#define __DRVADC_H__

//#include "stm32f10x_adc.h"

//GPIO 部分
#define RCC_PCLK_ADC_GPIO			RCC_APB2Periph_GPIOA
#define PORT_ADC_IN					GPIOA
#define ADC_IN_PIN					GPIO_Pin_1

//ADC 部分
#define RCC_PCLK_ADC_CHL			RCC_APB2Periph_ADC1
#define ADC_CHL						ADC1

//函数声明部分
extern void  Adc_Init(void);
extern uint16_t Get_AdcAverage(uint8_t _ucChl, uint8_t _ucTimes);



#endif	//__DRVADC_H__

