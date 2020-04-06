#ifndef __APP_HTU21D_H__
#define __APP_HTU21D_H__

#define HTU_ADDR		0x80
#define HTU_ADDR_WR (HTU_ADDR & 0xFE)
#define HTU_ADDR_RD (HTU_ADDR | 0x01)

//非主机模式
#define HTU_TEMP    0xF3
#define HTU_HUMI    0XF5

#define HTU_SOFTWARE_RESET  0xFe

extern void Htu_Init(void);
extern float Htu_Measure(uint8_t _ucOrder);
extern void Htu_Display(void);
extern bool htu_write_some_bytes(uint8_t *pbdata, uint16_t write_length);
extern bool htu_read_some_bytes(uint8_t *pbdata, uint16_t read_length);

#endif	//__APP_HTU21D_H__
