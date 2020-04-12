#line 1 "..\\App\\src\\variable.c"



#line 1 "..\\User\\inc\\typedef.h"



typedef unsigned char bool;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long ULONG;

typedef signed char  	int8_t;
typedef signed short 	int16_t;
typedef signed int 		int32_t;
typedef signed long 	LONG;

	










	







#line 5 "..\\App\\src\\variable.c"


uint16_t g_SpiId = 0;
__align(4) uint8_t g_DataTmpBuffer[0x1000] = {0};
volatile uint16_t g_SpiTxRxLen = 0;

