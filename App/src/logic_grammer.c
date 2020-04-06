#ifndef __PRJ_STM32F10X_LOGIC_GRAMMER_C__
#define __PRJ_STM32F10X_LOGIC_GRAMMER_C__

#include "Typedef.h"
#include "logic_grammer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


//--------------------------------------------------------------------------------------------------------
//	函 数 名: sprintf_test
//	功能说明: sprintf功能测试
//	形    参: 无
//	返 回 值: 无
//	日    期: 2020-03-21
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void sprintf_test(void)
{
	char buf[100] = {0};
	double data = 3.141593;
	uint16_t len = 0;
	uint8_t test_buff[5] = {55, 22, 66, 77, 99};
	
	len = sprintf((char *)buf, "%f", data);
	printf("----------------------------------------------------------------- \r\n");
	printf("buf is %s \r\n", buf);
	printf("sprintf return len is %d \r\n", len);
	
	len = 0;
	
	for (uint16_t i = 0; i < sizeof(test_buff); i++)
	{
		len += sprintf((char *)buf + i * 2, "%d", test_buff[i]);	//数组数据转换为字符串
	}
	printf("test_buff val is %s \r\n", buf);
	printf("sprintf return len is %d \r\n", len);
	printf("----------------------------------------------------------------- \r\n");
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: My_Memcpy
//	功能说明: memcpy 原码实现
//	形    参: 	dest：目的地址
//				src：源地址
//				count：数据长度
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void *My_Memcpy(void *dest, const void *src, unsigned int count)
{
	char *pdest = (char *)dest;
	char *psrc = (char *)src;

	if (pdest == NULL || psrc == NULL)
	{
		return NULL;
	}

	if (pdest == psrc)
	{
		return 0;
	}

	if (pdest > psrc)	//目的地址和源地址存在重叠的情况
	{
		while (count--)
		{
			*(pdest + count) = *(psrc + count);
		}
	}
	else
	{
		while (count--)
		{
			*pdest++ = *psrc++;
		}
	}
	return pdest;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: My_Memset
//	功能说明: memset 原码实现
//	形    参: 	dest：目的地址
//				src：源地址
//				len：数据长度
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void *My_Memset(void *dest, int n, unsigned int len)
{
	if (dest == NULL)
		return NULL;
	char *pdest = (char *)dest;
	while (len--)
	{
		*pdest++ = n;
	}

	return dest;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: My_Memcmp
//	功能说明: memcmp 原码实现
//	形    参: 	dest：目的地址
//				src：源地址
//				count：数据长度
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
int My_Memcmp(void *str1, void *str2, int count)
{
	char *ptr1 = (char *)str1;
	char *ptr2 = (char *)str2;

	if (!count)
	{
		return 0;
	}

	while (count--)
	{
		if (*ptr1 == *ptr2)
		{
			ptr1 = ptr1 + 1;
			ptr2 = ptr2 + 1;
		}
		else
		{
			return *ptr1 - *ptr2;
		}
	}
	return *ptr1 - *ptr2;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: My_Strlen
//	功能说明: strlen 原码实现
//	形    参: 	src：数据目的地址
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
unsigned int My_Strlen(const char *str)
{
	int str_len = 0;

	while (*str++ != '\0')
	{
		str_len++;
	}
	
	return str_len;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: My_Strcpy
//	功能说明: strcpy 原码实现
//	形    参: 	src：数据目的地址
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
char *My_Strcpy(char *dest, const char *src)
{
	char *tmp = (char *)dest;
	
	if ((dest == NULL) || (src == NULL))
	{
		return 0;
	}
	
	while (*src != '\0')
	{
		*tmp++ = *src++;
	}
	
	*tmp = '\0';	//字符串结束增加'\0'
	
	return tmp;
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: My_Strcmp
//	功能说明: strcmp 原码实现
//	形    参: 	src：数据目的地址
//	返 回 值: 无
//	日    期: 2020-03-22
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
int My_Strcmp(const char *str1, const char *str2)
{
	if (str1 == NULL || str2 == NULL)
	{
		return NULL;
	}
	
	while (*str1 == *str2)
	{
		str1++;
		str2++;
		
		if (*str1 == '\0' || *str2 == '\0')
		{
			break;
		}
	}
	
	return *str1 - *str2;
}

void strcmp_test(void)
{
	int ret = 0;
//	char test_buf1[6] = "ABCBE";
//	char test_buf2[3] = "AB";
	char test_buf1[6] = {1, 2, 3, 4, 5, 6};
	char test_buf2[6] = {1, 2, 3, 7, 5, 6};
	
	ret = My_Strcmp(test_buf1, test_buf2);
	printf("ret val is %d \r\n", ret);
}
	
void strcpy_test(void)
{
	char test_buf1[6] = "ABCDE";
	char test_buf2[100] = {0};
	
	My_Strcpy(test_buf2, test_buf1);
	printf("strcpy test %s \r\n", test_buf2);
}

void memcmp_test(void)
{
	char str1[] = "abcd";
	char str2[] = "abcd";
	int ret = 0;

	ret = My_Memcmp(str1, str2, 4);
	printf("ret val is %d \r\n", ret);

	memcpy(str1, "abce", 4);
	ret = My_Memcmp(str1, str2, 4);
	printf("ret val is %d \r\n", ret);

	memcpy(str1, "abcc", 4);
	ret = My_Memcmp(str1, str2, 4);
	printf("ret val is %d \r\n", ret);
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: cpu_mem_endian
//	功能说明: 大小端测试
//	形    参: 无
//	返 回 值: 无
//	日    期: 2019-03-22
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
void cpu_mem_endian(void)
{
	union un_test 
	{
		uint8_t x;
		uint16_t y;
	}un_endian_test;
	
	un_endian_test.y = 0x1234;
	
	if (un_endian_test.x == 0x34)
	{
		printf("cpu is little endian \r\n");
	}
	if (un_endian_test.x == 0x12)
	{
		printf("cpu is big endian \r\n");
	}
}

//--------------------------------------------------------------------------------------------------------
//	函 数 名: Crc_B16
//	功能说明: CRC运算
//	形    参: 无
//	返 回 值: 无
//	日    期: 2019-03-28
//  备    注: 无
//	作    者: by 霁风AI
//--------------------------------------------------------------------------------------------------------
uint16_t Crc_B16(uint8_t *Indata, uint16_t len, uint16_t CrcTmp)
{
	uint16_t y = 0;
	uint16_t yy = 0;
	uint8_t x = 0;
	uint32_t i = 0;
	uint32_t j = 0;

     y = ~CrcTmp & 0xffff;
	
	for(i = 0; i < len; i++)
	{
		x = Indata[i];
		for (j = 0; j < 8; j++)
		{
			if (((y^x) & 1) !=0) 
			{
				yy= 0x8408;
			}
			else 
			{
				yy=0;
			}
		x >>= 1;
		y >>= 1;
		y ^= yy;
		}
	}
	
	return (~y & 0xffff);
}

#endif	//__PRJ_STM32F10X_LOGIC_GRAMMER_C__
