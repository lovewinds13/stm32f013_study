#ifndef __DRVEXFLASH__H__
#define __DRVEXFLASH__H__

//#include "Typedef.h"


#define  FLASH_WRITE_ENABLE_CMD 		0x06
#define  FLASH_WRITE_DISABLE_CMD		0x04
#define  FLASH_READ_SR_CMD				0x05
#define  FLASH_WRITE_SR_CMD				0x01
#define  FLASH_READ_DATA				0x03
#define  FLASH_FASTREAD_DATA			0x0b
#define  FLASH_WRITE_PAGE				0x02
#define  FLASH_ERASE_PAGE      			0x81
#define  FLASH_ERASE_SECTOR       		0x20
#define	 FLASH_ERASE_BLOCK				0xd8
#define	 FLASH_ERASE_CHIP				0xc7
#define  FLASH_POWER_DOWN				0xb9
#define  FLASH_RELEASE_POWER_DOWN       0xab
#define  FLASH_READ_DEVICE_ID      		0x90
#define  FLASH_READ_JEDEC_ID      		0x9f

#define 	FLASH_SIZE	 (1*1024*1024)	// 1M字节
#define		PAGE_SIZE			8192	// 256 bytes
#define 	SECTOR_SIZE		512	 // 4-Kbyte
#define		BLOCK_SIZE		32	// 64-Kbyte	

#define PAGE_LEN		255	 //一页256字节


//定义FLASH相关属性定义
struct Flash_Attr  {
	uint16_t flash_id;
	uint16_t page_size;
	uint16_t sector_size;
	uint8_t block_size;
};	

extern uint32_t Flash_ReadJEDECID(void);
extern void Flash_WriteEnable(void);
extern void Flash_WriteDisable(void);
extern uint8_t Flash_ReadSR(void);
extern void Flash_WriteSR(uint8_t _ucByte);
extern void Flash_WaitNobusy(void);
extern void Flash_ReadSomeBytes(uint8_t *ucpBuffer, uint32_t _ulReadAddr, uint16_t _usNByte);
extern void Flash_FastReadByte(uint8_t *ucpBuffer, uint32_t _ulReadAddr, uint16_t _usNByte);
extern void Flash_WritePage(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte);
extern void Flash_WriteNoCheck(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte);
extern void Flash_WriteSomeBytes(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte);
extern void Flash_ErasePage(uint32_t _ulPageAddr);
extern void Flash_EraseSector(uint32_t _ulSectorAddr);
extern void Flash_EraseBlock(uint32_t _ulBlockAddr);
extern void Flash_EraseChip(void);
extern void Flash_PowerDown(void);
extern void Flash_WakeUp(void);
extern uint16_t Flash_ReadDeviceID(void);
extern uint32_t Flash_ReadJEDECID(void);


#endif	//__DRVEXFLASH__H__
