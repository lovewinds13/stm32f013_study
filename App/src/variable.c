#ifndef __PRJ_STM32F10X_VARIABLE_C__
#define __PRJ_STM32F10X_VARIABLE_C__

#include "typedef.h"
//#include "variable.h"

uint16_t g_SpiId = 0;
__align(4) uint8_t g_DataTmpBuffer[0x1000] = {0};
volatile uint16_t g_SpiTxRxLen = 0;

#endif	//__PRJ_STM32F10X_VARIABLE_C__
