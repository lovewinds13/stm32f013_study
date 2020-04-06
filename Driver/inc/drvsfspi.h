#ifndef __DRVSFSPI_H__
#define __DRVSFSPI_H__


#ifdef SOFT_SPI_COMM

#define RCC_PCLK_SPI1_IO			RCC_APB2Periph_GPIOA
#define RCC_PCLK_SPI1_HD			RCC_APB2Periph_SPI1
#define PORT_SPI1_IO				GPIOA

#define FLASH_CS     		(GPIO_Pin_2)
#define FLASH_CS_LOW		(GPIO_ResetBits(PORT_SPI1_IO, FLASH_CS))		
#define FLASH_CS_HIGH 		(GPIO_SetBits(PORT_SPI1_IO, FLASH_CS))

#else

//#define GPIO_FLASH_CS     		(GPIO9)
//#define FLASH_CS_0()              (IOMSetGPIOVal(GPIO_FLASH_CS, 0))
//#define FLASH_CS_1()              (IOMSetGPIOVal(GPIO_FLASH_CS, 1))

#endif

#define SPI_CLK      (GPIO_Pin_5)
#define SPI_CLK_LOW 		(GPIO_ResetBits(PORT_SPI1_IO, SPI_CLK))
#define SPI_CLK_HIGH 		(GPIO_SetBits(PORT_SPI1_IO, SPI_CLK))

#define SPI_MOSI  	  (GPIO_Pin_7)
#define SPI_MOSI_LOW 		(GPIO_ResetBits(PORT_SPI1_IO, SPI_MOSI))
#define SPI_MOSI_HIGH		(GPIO_SetBits(PORT_SPI1_IO, SPI_MOSI))

#define SPI_MISO  	   (GPIO_Pin_6)
#define SPI_MISO_LOW 		(GPIO_ResetBits(PORT_SPI1_IO, SPI_MISO))
#define SPI_MISO_HIGH		(GPIO_SetBits(PORT_SPI1_IO, SPI_MISO))
#define SPI_MISO_READ		GPIO_ReadInputDataBit(PORT_SPI1_IO, SPI_MISO)


extern void Spi_GpioInit(void);
extern void Spi_WriteByte(uint8_t _ucByte);
extern uint8_t Spi_ReadByte(void);

#endif	//__DRVSFSPI_H__
