#ifndef __HCSR04_H__
#define __HCSR04_H__

#include "sys.h" 

extern u32 g_cap_distance;

extern void timer4_cap_init(u16 timer_arr, u16 timer_psc);
extern void hcsr04_read_distance(void);

#endif
