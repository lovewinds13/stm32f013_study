#ifndef _DRVOLED_H_
#define _DRVOLED_H_ 

#define MAX_COLUMN	128
#define MAX_ROW	64
#define SIZE 16 	//œ‘ æ◊÷ÃÂ—°‘Ò()

#define OLED_DATA	1
#define OLED_COMM	0


#define	OLED_GPIO_CLK				RCC_APB2Periph_GPIOA
#define	OLED_PORT					GPIOA

#if 1
#define OLED_CS_PIN		GPIO_Pin_1	//CS
#define OLED_CS_LOW		GPIO_ResetBits(OLED_PORT, OLED_CS_PIN)						
#define OLED_CS_HIGH	GPIO_SetBits(OLED_PORT, OLED_CS_PIN)
#else
#define OLED_CS_PIN		GPIO_Pin_4	//CS
#define OLED_CS_LOW		GPIO_ResetBits(GPIOC, GPIO_Pin_2)						
#define OLED_CS_HIGH	GPIO_SetBits(GPIOC, GPIO_Pin_2)
#endif

#define OLED_DC_PIN		GPIO_Pin_3	//DC
#define OLED_DC_LOW		GPIO_ResetBits(OLED_PORT, OLED_DC_PIN)
#define OLED_DC_HIGH	GPIO_SetBits(OLED_PORT, OLED_DC_PIN)

#define OLED_RST_PIN	GPIO_Pin_4	//RST
#define OLED_RST_LOW	GPIO_ResetBits(OLED_PORT, OLED_RST_PIN)
#define OLED_RST_HIGH	GPIO_SetBits(OLED_PORT, OLED_RST_PIN)

#define OLED_CLK_PIN	GPIO_Pin_5
#define OLED_DATA_PIN	GPIO_Pin_7

extern void oled_init(void);
extern void oled_dis_clear(void);
extern void oled_dis_set(void);
extern void oled_dis_one_char(uint8_t x, uint8_t y, uint8_t str);	
extern void oled_dis_str(uint8_t x, uint8_t y, uint8_t *str);
extern void oled_dis_chinese(uint8_t x, uint8_t y, uint8_t no);
extern void oled_dis_picture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *bmp);
extern void oled_dis_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size_num);
extern void oled_dis_logo(void);


#endif	//_DRVOLED_H_
