#ifndef _APP_FLASH_H__
#define _APP_FLASH_H__

#include "declares.h"

#define MAX_PAGE_1MBYTE     4096
#define MAX_PAGE_2MBYTE     8192
#define MAX_PAGE_4MBYTE     16384
#define MAX_PAGE_8MBYTE		32768

typedef enum 
{
    FLASH_SIZE_1MBYTE = 0x01,
    FLASH_SIZE_2MBYTE = 0x02,
    FLASH_SIZE_4MBYTE = 0x03,
	FLASH_SIZE_8MBYTE = 0x04,
}FLASH_SIZE_E;

extern void Flash_TestDataInit(void);
extern void W25_WriteReadBytes(void);
extern void W25_EraseTest(void);
extern void W25_CircleWriteRead(uint8_t _ucFlashSize, uint32_t _ulAddr, uint8_t *ucpWBuf, uint8_t *ucpRBuf, uint16_t _usLen);

#endif //_APP_FLASH_H__
