#ifndef __C_LANGUAGE_REGULAR_H__
#define __C_LANGUAGE_REGULAR_H__
	
#define FUNC_TEST1(x)	\
	printf("1 - arg is %d \r\n", x);	\
	printf("test ok! \r\n");
	
#define FUNC_TEST2(x)	do {	\
	printf("2 - arg is %d \r\n", x);	\
	printf("test ok! \r\n");	\
}while(0);


typedef struct 
{
	uint8_t data_buff[5];
//	uint8_t data_flag;
	uint32_t data_length;	//结构体的变量按照最大的长度对齐(此处即sizeof(uin32_t) = 4 byte)
	uint8_t data_flag;
}st_data_test;

typedef struct
{
	uint8_t type;
	uint16_t length;
	uint8_t value;
}st_tlv;


#if 1
#pragma pack(1)
typedef struct 
{
	uint8_t data_buff[5];
	uint32_t data_length;
	uint8_t data_flag;
}st_data_test1;
#pragma pack()
#endif 

__packed struct send_msg
{
	uint8_t head;
	uint32_t length;
	uint16_t crc;
	uint8_t flag;
};

typedef struct
{
	uint32_t parm1;
	uint32_t parm2;
	uint16_t length;
	uint8_t flag;
	uint8_t data_pad;	//填充一字节,保证4字节对齐
}s_msg, *p_msg;

extern void macro_define_test(void);
extern void pack_test(void);
extern void stack_test(void);
extern void heap_test(void);


#endif	//__C_LANGUAGE_REGULAR_H__
