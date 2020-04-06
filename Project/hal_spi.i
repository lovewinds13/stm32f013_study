#line 1 "..\\HalIterface\\src\\hal_spi.c"



#line 1 "..\\User\\inc\\Typedef.h"



typedef unsigned char bool;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long ULONG;

typedef signed char  	int8_t;
typedef signed short 	int16_t;
typedef signed int 		int32_t;
typedef signed long 	LONG;

	










	







#line 5 "..\\HalIterface\\src\\hal_spi.c"
#line 1 "..\\User\\inc\\declares.h"






























































#line 6 "..\\HalIterface\\src\\hal_spi.c"
#line 1 "..\\App\\inc\\variable.h"





extern __align(4) uint8_t g_DataTmpBuffer[0x1000];
extern volatile uint16_t g_SpiTxRxLen;

#line 7 "..\\HalIterface\\src\\hal_spi.c"
#line 1 "..\\HalIterface\\inc\\hal_spi.h"



extern bool hal_spi_send_bytes(uint8_t mode, uint8_t *pbdata, uint16_t send_length);
extern bool hal_spi_recv_bytes(uint8_t mode, uint8_t *pbdata, uint16_t send_length);

#line 8 "..\\HalIterface\\src\\hal_spi.c"
#line 1 "..\\Driver\\inc\\hardware_spi.h"





















extern void spi_master_init(uint8_t spi_chl);			 
extern uint8_t spi_master_send_recv_byte(uint8_t spi_chl, uint8_t spi_byte);
extern void spi_master_nvic_set(void);

extern void spi_master_speed_set(uint8_t spi_chl, uint8_t spi_speed); 
extern uint8_t spi_master_send_byte(uint8_t spi_chl, uint8_t send_byte);
extern uint8_t spi_master_recv_byte(uint8_t spi_chl);

extern void spi_master_send_some_bytes(uint8_t spi_chl, uint8_t *pbdata, uint16_t send_length);
extern void spi_master_recv_some_bytes(uint8_t spi_chl, uint8_t *pbdata, uint16_t recv_length);


extern uint32_t spi_flash_read_id(void);


uint8_t SPI_WriteReadByte(uint8_t TxData);
uint16_t SFLASH_ReadID(void);
uint16_t SPI_Flash_ReadID(void);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);



#line 9 "..\\HalIterface\\src\\hal_spi.c"
#line 1 "..\\Driver\\inc\\drvsfspi.h"




#line 16 "..\\Driver\\inc\\drvsfspi.h"





















extern void Spi_GpioInit(void);
extern void Spi_WriteByte(uint8_t _ucByte);
extern uint8_t Spi_ReadByte(void);

#line 10 "..\\HalIterface\\src\\hal_spi.c"

#line 97 "..\\HalIterface\\src\\hal_spi.c"

