#ifndef __PRJ_STM32F10X_C_LANGUAGE_REGULAR_C__
#define __PRJ_STM32F10X_C_LANGUAGE_REGULAR_C__

#include "typedef.h"
#include "c_language_regular.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

st_data_test g_data_test = {0};
st_tlv g_data_tlv = {0};
st_data_test1 g_data_test1 = {0};
struct send_msg g_send_msg = {0};
__align(4) uint8_t g_data_buff[0x10] = {0};
uint8_t g_data_buff1[0x10] = {0};
__align(4) uint16_t g_data_length = 0;
uint16_t g_data_length1 = 0;
//--------------------------------------------------------------------------------------------------------
//	函 数 名: macro_define_test
//	功能说明: 宏定义方式测试
//	形    参: 无
//	返 回 值: 无
//	日    期: 2019-03-28
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void macro_define_test(void)
{
	uint8_t tmp = 1;
	
	for (uint16_t i = 0; i < 3; i++)
	{
		FUNC_TEST1(6);
	}
	
	for (uint16_t i = 0; i < 3; i++)
	{
		FUNC_TEST2(6);
	}
	
	if (!tmp)
	{
		FUNC_TEST1(99);
		FUNC_TEST2(66);
	}
	else 
	{
		FUNC_TEST1(100);
		FUNC_TEST2(101);
	}
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: pack_test
//	功能说明: 对齐测试
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-04
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void pack_test(void)
{
	uint8_t data_size = 0;
	
//	data_size = sizeof(g_data_test);
//	printf("struct test size is %d \r\n", data_size);
	
//	data_size = sizeof(g_send_msg);
//	printf("struct test size is %d \r\n", data_size);
//	
//	printf("g_send_msg addr is			%p \r\n", &g_send_msg);
//	printf("g_send_msg.head addr is 		%p \r\n", &g_send_msg.head);
//	printf("g_send_msg.length addr is 		%p \r\n", &g_send_msg.length);
//	printf("g_send_msg.crc addr is 			%p \r\n", &g_send_msg.crc);
//	printf("g_send_msg.flag addr is 		%p \r\n", &g_send_msg.flag);
	
//	data_size = sizeof(g_data_tlv);
//	printf("struct test size is %d \r\n", data_size);
	printf("g_data_buff addr is			%p \r\n", &g_data_buff);
	printf("g_data_buff[1] addr is			%p \r\n", &g_data_buff[1]);
	printf("g_data_buff[2] addr is			%p \r\n", &g_data_buff[2]);
	
	printf("g_data_buff1 addr is			%p \r\n", &g_data_buff1);
	printf("g_data_buff1[1] addr is			%p \r\n", &g_data_buff1[1]);
	printf("g_data_buff1[2] addr is			%p \r\n", &g_data_buff1[2]);
	
	printf("g_data_length addr is			%p \r\n", &g_data_length);
	printf("g_data_length1 addr is			%p \r\n", &g_data_length1);
	printf("\r\n");
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: stack_test
//	功能说明: 栈测试
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-04-04
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void stack_test(void)
{
#pragma pack(push, 1)	// 1/2/4/8
struct st_test
{
	uint8_t val1;
	uint32_t val2;
};
#pragma pack(pop)

	uint8_t tmp1;
	uint16_t tmp2;
	uint32_t tmp3;
	double data[2];
	struct st_test tag_test;

	printf("tmp1    		address:   %p \r\n", &tmp1);
	printf("tmp2    		address:   %p \r\n", &tmp2);
	printf("tmp3    		address:   %p \r\n", &tmp3);
	printf("data[0] 		address:   %p \r\n", &(data[0]));
	printf("data[1] 		address:   %p \r\n", &(data[1]));
	printf("tag_test    	address:   %p \r\n", &tag_test);
	printf("tag_test.val2 	address:   %p \r\n", &(tag_test.val2));
}

void heap_test(void)
{
	char *p1 = (char *)malloc(1 * sizeof(char));
	char *p2 = (char *)malloc(2 * sizeof(char));
	char *p3 = (char *)malloc(4 * sizeof(char));
	char *p4 = (char *)malloc(8 * sizeof(char));
	
	printf("\r\n");
	printf("p1    		address:   %p \r\n", p1);
	printf("p2    		address:   %p \r\n", p2);
	printf("p3    		address:   %p \r\n", p3);
	printf("p4    		address:   %p \r\n", p4);
	
	free(p1);
	free(p2);
	free(p3);
	free(p4);
}

#endif	//__PRJ_STM32F10X_C_LANGUAGE_REGULAR_C__
