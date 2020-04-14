#ifndef _VARIABLE_H__
#define _VARIABLE_H__

#include "typedef.h"

extern __align(4) uint8_t g_DataTmpBuffer[0x1000];
extern __align(4) volatile uint16_t g_SpiTxRxLen;


extern volatile uint16_t g_timer3_tick;


#endif //_VARIABLE_H__
