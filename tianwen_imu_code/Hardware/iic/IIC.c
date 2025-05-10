//#include "IIC.h"
#include "systick.h"
//#include "bsp_usart.h"
//#include "stdio.h"
//#include "FreeRTOS.h"
//#include "systick.h"
//#include "main.h"
//#include "gd32f4xx.h"
#include "main.h"
void iic_gpio_config(void)//PB8==SCL PB9==SDA
{
	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_SCL);
	rcu_periph_clock_enable(RCU_SDA);
		
	/* 配置SCL为输出模式 */
	gpio_mode_set(PORT_SCL,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_SCL);
	/* 配置为开漏输出 50MHZ */
	gpio_output_options_set(PORT_SCL,GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ,GPIO_SCL);
		
	/* 配置SDA为输出模式 */
	gpio_mode_set(PORT_SDA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_SDA);
	/* 配置为开漏输出 50MHZ */
	gpio_output_options_set(PORT_SDA,GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ,GPIO_SDA);
}
//起始信号
void IIC_Start(void)
{
	SDA_OUT();
	SDA(1);
	SCL(1); 
	delay_us(4);
	SDA(0);
	delay_us(4);
	SCL(0);
	       
}
//停止信号
void IIC_Stop(void)
{
	SDA_OUT();
	SCL(0);
	SDA(0);
	delay_us(4);
	SCL(1);
	SDA(1);
	delay_us(4);
}
//发送非应答
void IIC_NAck(void)
{
	
	SCL(0);
	SDA_OUT();
	SDA(1);
	delay_us(2);
	SCL(1);
	delay_us(2);
	SCL(0);
}
//发送应答
void IIC_Ack(void)
{
	SCL(0);
	SDA_OUT();
	SDA(0);
	delay_us(2);
	SCL(1);
	delay_us(2);
	SCL(0);
}

/**********************************************************
 * 函数名称：I2C_WaitAck
 * 函数功能：等待从机应答
 * 传入参数：无
 * 函数返回：1=非应答 0=应答
 * 作者    :LC
 * 备注    ：无
**********************************************************/
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	SDA(1);delay_us(1);	   
	SCL(1);delay_us(1);	 
	while(SDA_GET())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL(0);//时钟输出0 	   
	return 0;  
} 
//发送一个字节

void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
      SDA((txd&0x80)>>7);
      txd<<=1; 	  
	  delay_us(2);   
	  SCL(1);
	  delay_us(2); 
	  SCL(0);	
	  delay_us(2);
    }	 
} 	

uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{	
      SCL(0); 
      delay_us(2);
	  SCL(1);
      receive<<=1;
      if(SDA_GET())receive++;   
		  delay_us(1); 
	}					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

uint8_t IIC_Write_1Byte(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
				 
{ 
    IIC_Start(); 
	IIC_Send_Byte((SlaveAddress<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack())	//等待应答
	{		
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(REG_Address);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答 
	IIC_Send_Byte(REG_data);//发送数据
	if(IIC_Wait_Ack())	//等待ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}
uint8_t IIC_Read_1Byte(uint8_t SlaveAddress,uint8_t REG_Address)
{
	uint8_t data;
	
    IIC_Start(); 
	IIC_Send_Byte((SlaveAddress<<1)|0);//发送器件地址+写命令	
	IIC_Wait_Ack();		//等待应答 
    IIC_Send_Byte(REG_Address);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
    IIC_Start();
	IIC_Send_Byte((SlaveAddress<<1)|1);//发送器件地址+读命令	
    IIC_Wait_Ack();		//等待应答 
	data=IIC_Read_Byte(0);//读取数据,发送nACK 
    IIC_Stop();			//产生一个停止条件 
	return data;		
}

uint8_t IIC_Write_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint16_t len, const uint8_t *buf)
{	
	IIC_Start();
	IIC_Send_Byte(SlaveAddress<<1); 
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	IIC_Send_Byte(REG_Address); 
	IIC_Wait_Ack();
	while(len--) 
	{
		IIC_Send_Byte(*buf++); 
		IIC_Wait_Ack();
	}
	IIC_Stop();
	return 0;
}


// IIC读n字节数据
uint8_t IIC_Read_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint16_t len, uint8_t *buf)
{	
	IIC_Start();
	IIC_Send_Byte(SlaveAddress<<1); 
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	IIC_Send_Byte(REG_Address); 
	IIC_Wait_Ack();
	
	IIC_Start();
	IIC_Send_Byte(SlaveAddress<<1 | 0x01); 
	IIC_Wait_Ack();
	while(len) 
	{
		if(len == 1)
		{
			*buf = IIC_Read_Byte(0);
		}
		else
		{
			*buf = IIC_Read_Byte(1);
		}
		buf++;
		len--;
	}
	IIC_Stop();
	return 0;
}
