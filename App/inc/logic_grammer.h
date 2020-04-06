#ifndef __LOGIC_GRAMMER_H__
#define __LOGIC_GRAMMER_H__

extern unsigned int My_Strlen(const char *str);
extern char *My_Strcpy(char *dest, const char *src);
extern int My_Memcmp(void *str1, void *str2, int count);
extern void *My_Memset(void *dest, int n, unsigned int len);
extern void *My_Memcpy(void *dest, const void *src, unsigned int count);
extern int My_Strcmp(const char *str1, const char *str2);
extern void cpu_mem_endian(void);
extern uint16_t Crc_B16(uint8_t *Indata, uint16_t len, uint16_t wCrcTmp);

extern void sprintf_test(void);
extern void strcpy_test(void);
extern void memcmp_test(void);
extern void strcmp_test(void);
#endif

