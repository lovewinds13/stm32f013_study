#ifndef __APP_AT24CXX_H__
#define __APP_AT24CXX_H__

#define TYPE_AT24C01    127     //Bits
#define TYPE_AT24C02    255
#define TYPE_AT24C04    511
#define TYPE_AT24C08    1023
#define TYPE_AT24C16    2047 

#define AT24CXX_ADDR     0xA0

extern void ee_at24cxx_init(void);
extern bool ee_at24cxx_write_byte(uint16_t write_addr, uint8_t write_byte);
extern bool ee_at24cxx_read_byte(uint16_t read_addr);
extern void ee_at24cxx_write_some_bytes(uint16_t write_addr, uint8_t *pbdata, uint16_t write_length);
extern uint16_t ee_at24cxx_read_some_bytes(uint16_t read_addr, uint8_t *pbdata, uint16_t read_length);
extern void ee_at24cxx_test(void);

#endif	//__APP_AT24CXX_H__
