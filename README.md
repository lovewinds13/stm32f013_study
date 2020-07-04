# 微信公众号：TECHTIMES

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200704103640469.jpg)

My csdn blog：https://blog.csdn.net/wwt18811707971

# My_Embedded_Code

（1）MCU：STM32F103RC

（2）包含底层通信和对应的应用测试代码



# 修改记录：

（1）2020.03.11 上传工程代码

（2）2020.03.14 软硬件 SPI 通信均测试验证成功

（3）2020.03.22 软件 I2C 通信完成验证，并实现温湿度传感器通过 I2C 采集数据；


# 文件记录：

## App：

（1）app_flash.c：spi flash test code（erase；read；write）

（2）app_adc：adc get voltage test（stm32 the input voltage is 0——3.3V，notice input maybe lead to adc module break）

（3）app_htu21d.c：Sensor temperature and humidity gets data by i2c，see the data form usart by "printf"

（4）logic_grammer.c：c library function（memcpy，memset，memcmp，strcpy，strlen，strcmp，sprintf）

（5）app_at24cxx.c：eeprom operate by i2c

（6）c_language_regular.c：some c language regular test

（7）variable.c：gloal variable



## Driver：
（1）drvexflash.c：Spi flash operate （W25X，W25Q ，and so on）

（2）drvsfi2c.c：Soft simulate i2c logic communication

（3）drvsfspi.c：Soft simulate spi logic communication（mode0 and mode3）

（4）hardware_spi.c：Hard spi driver

（4）drvadc.c ：ADC dirver (single channel)

（5）drvuart.c：Uart driver 









