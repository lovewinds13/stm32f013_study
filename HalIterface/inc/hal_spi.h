#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

extern uint8_t hal_spi_send_bytes(uint8_t mode, uint8_t *pbdata, uint16_t send_length);
extern uint8_t hal_spi_recv_bytes(uint8_t mode, uint8_t *pbdata, uint16_t send_length);

#endif	//__HAL_SPI_H__
