#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

typedef unsigned char bool;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long ULONG;

typedef signed char  	int8_t;
typedef signed short 	int16_t;
typedef signed int 		int32_t;
typedef signed long 	LONG;
#define __IO volatile 
	
#define FALSE 0
#define TRUE  1

#ifndef true
#define true	1
#endif

#ifndef false
#define false	0
#endif
	
#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE
#define printk	printf
#endif 



#endif
