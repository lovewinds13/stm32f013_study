#ifndef __HARDWARE_SPI__H__
#define __HARDWARE_SPI__H__

//#include "stm32f10x.h"

//#define SPI1_IRQ_ENABLE
#define SPIM1_GPIO_PORT	    GPIOA

#define SPIM1_CLK_IO	(GPIO_Pin_5)
#define SPIM1_MISO_IO	(GPIO_Pin_6)
#define SPIM1_MOSI_IO	(GPIO_Pin_7)

#define FLASH_CS_IO     		(GPIO_Pin_2)
#define FLASH_CS_0()			(GPIO_ResetBits(SPIM1_GPIO_PORT, FLASH_CS_IO))		
#define FLASH_CS_1() 			(GPIO_SetBits(SPIM1_GPIO_PORT, FLASH_CS_IO))

#define RCC_PCLK_SPIM1_GPIO     RCC_APB2Periph_GPIOA
#define RCC_PCLK_SPIM1_HD       RCC_APB2Periph_SPI1



extern void spi_master_init(uint8_t spi_chl);			 //初始化SPI口
extern uint8_t spi_master_send_recv_byte(uint8_t spi_chl, uint8_t spi_byte);
extern void spi_master_nvic_set(void);

extern void spi_master_speed_set(uint8_t spi_chl, uint8_t spi_speed); //设置SPI速度   
extern uint8_t spi_master_send_byte(uint8_t spi_chl, uint8_t send_byte);//SPI总线读写一个字节
extern uint8_t spi_master_recv_byte(uint8_t spi_chl);

extern void spi_master_send_some_bytes(uint8_t spi_chl, uint8_t *pbdata, uint16_t send_length);
extern void spi_master_recv_some_bytes(uint8_t spi_chl, uint8_t *pbdata, uint16_t recv_length);


extern uint32_t spi_flash_read_id(void);

//u8 SPI1_ReadWriteByte(u8 TxData);
uint8_t SPI_WriteReadByte(uint8_t TxData);
uint16_t SFLASH_ReadID(void);
uint16_t SPI_Flash_ReadID(void);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);

#endif	//__HARDWARE_SPI__H__

