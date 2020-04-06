#ifndef __DRVSFI2C_H__
#define __DRVSFI2C_H__

//#ifdef SOFT_I2C_COMM

#define RCC_PCLK_I2C_GPIO			RCC_APB2Periph_GPIOC

#define PORT_I2C_SCL			GPIOC
#define PORT_I2C_SDA			GPIOC

#define I2C_SCL_IO					GPIO_Pin_12
#define I2C_SDA_IO					GPIO_Pin_11

#if 1
	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
//#define I2C_SCL_LOW				GPIO_ResetBits(PORT_I2C_SCL, I2C_SCL_IO)	
//#define I2C_SCL_HIGH			GPIO_SetBits(PORT_I2C_SCL, I2C_SCL_IO)
//#define I2C_SDA_LOW				GPIO_ResetBits(PORT_I2C_SDA, I2C_SDA_IO)
//#define I2C_SDA_HIGH			GPIO_SetBits(PORT_I2C_SDA, I2C_SDA_IO)

#define I2C_SCL_0()				GPIO_ResetBits(PORT_I2C_SCL, I2C_SCL_IO)	
#define I2C_SCL_1()				GPIO_SetBits(PORT_I2C_SCL, I2C_SCL_IO)
#define I2C_SDA_0()				GPIO_ResetBits(PORT_I2C_SDA, I2C_SDA_IO)
#define I2C_SDA_1()				GPIO_SetBits(PORT_I2C_SDA, I2C_SDA_IO)

#define I2C_SDA_READ  GPIO_ReadInputDataBit(PORT_I2C_SDA, I2C_SDA_IO)
	
#else
	/* 选择直接寄存器操作实现IO读写 */
#define I2C_SCL_LOW				(PORT_I2C_SCL->BRR  = I2C_SCL_IO)
#define I2C_SCL_HIGH			(PORT_I2C_SCL->BSRR = I2C_SCL_IO )
#define I2C_SDA_LOW				(PORT_I2C_SDA->BRR  = I2C_SDA_IO)
#define I2C_SDA_HIGH			(PORT_I2C_SDA->BSRR = I2C_SDA_IO)

#define I2C_SDA_READ			(PORT_I2C_SDA->IDR & I2C_SDA_IO)//读取输入电平状态,判断IO的电平

#endif

#define I2C_ACK					0				//应答
#define I2C_NACK				1				//非应答

#define I2C_OK					0
#define I2C_ERR					1


typedef struct 
{
	uint16_t uiI2cSpeed;
}St_I2cInfo;

extern St_I2cInfo StI2cInfo;


extern void I2c_GpioInit(void);
extern void I2c_CommSpeedSet(uint16_t _usSpeed);
extern void I2c_Init(void);
extern void I2c_Delay(uint16_t _usTime);
extern void I2c_Start(void);
extern void I2c_Stop(void);
extern void I2c_SendOneByte(uint8_t _ucData);
extern uint8_t I2c_RecvOneByte(uint8_t _ucAck);
extern uint8_t I2c_WaitAck(uint16_t _usErrTime);
extern void I2c_GetAck(void);
extern void I2c_GetNack(void);

//#endif

#endif	//__DRVSFI2C_H__

