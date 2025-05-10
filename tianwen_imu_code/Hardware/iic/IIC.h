#ifndef _IIC_H
#define _IIC_H


#include "main.h"

//端口移植
#define 	RCU_SDA 		RCU_GPIOB
#define 	PORT_SDA 		GPIOB
#define 	GPIO_SDA 		GPIO_PIN_9

#define 	RCU_SCL 		RCU_GPIOB
#define 	PORT_SCL	 	GPIOB
#define 	GPIO_SCL 		GPIO_PIN_8

//设置SDA输出模式
#define SDA_OUT()	gpio_mode_set(PORT_SDA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_SDA)

//设置SDA输入模式
#define SDA_IN()	gpio_mode_set(PORT_SDA,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_SDA)

//获取SDA引脚的电平变化
#define SDA_GET()	gpio_input_bit_get(PORT_SDA,GPIO_SDA) 

//SDA与SCL输出
#define SDA(x)         	gpio_bit_write(PORT_SDA,GPIO_SDA, (x?SET:RESET) )
#define SCL(x)          	gpio_bit_write(PORT_SCL,GPIO_SCL, (x?SET:RESET)  )

void iic_gpio_config(void);
float SHT20_receive_data(unsigned char num);
void iic_hardware_config(void);
float iic_hardware_get_data(unsigned int num);
uint8_t IIC_Read_1Byte(uint8_t SlaveAddress,uint8_t REG_Address);
uint8_t IIC_Write_1Byte(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
uint8_t IIC_Read_Byte(unsigned char ack);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Stop(void);
void IIC_Start(void);
uint8_t IIC_Read_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint16_t len, uint8_t *buf);
uint8_t IIC_Write_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint16_t len, const uint8_t *buf);
#endif