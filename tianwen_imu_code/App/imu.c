#include "imu.h"


void imu_init(void)//初始化函数，应在线程函数中初始化
{
	IIC_Write_1Byte(0x69,0x7F,0x02);//初始化
	IIC_Write_1Byte(0x69,0x32,0x10);//配置iic接口
	IIC_Write_1Byte(0x69,0x18,0x01);//
	IIC_Write_1Byte(0x69,0x16,0x04);//配置中断引脚
	IIC_Write_1Byte(0x69,0x1B,0x38);//设置加速度范围±4g
	IIC_Write_1Byte(0x69,0x1C,0x38);//设置角速率范围±500deg
	IIC_Write_1Byte(0x69,0x10,0x0F);//开启陀螺仪加速度计处于低噪音模式
}
static int read_dreg(uint8_t slave ,uint8_t reg, uint32_t len, uint8_t *buf)//读取函数
{
	if (IIC_Read_nByte(slave,reg,len,buf)!=0)
		return INV_IMU_ERROR_TRANSPORT;

	return INV_IMU_OK;
}
int inv_imu_get_register_data(icm45686_sensor_data *data)//解包大端通信
{
	int status = INV_IMU_OK;
	uint8_t buf[sizeof(icm45686_sensor_data)];
	status |= read_dreg(icm45686_slave_adress, icm45686_reg_adress, sizeof(icm45686_sensor_data),buf);
	data->accel_data[0]=(int16_t)buf[0]|(buf[1]<<8);
	data->accel_data[1]=(int16_t)buf[2]|(buf[3]<<8);
	data->accel_data[2]=(int16_t)buf[4]|(buf[5]<<8);
	data->gyro_data [0]=(int16_t)buf[6]|(buf[7]<<8);
	data->gyro_data [1]=(int16_t)buf[8]|(buf[9]<<8);
	data->gyro_data [2]=(int16_t)buf[10]|(buf[11]<<8);
	data->temp_data    =(int16_t)buf[12]|(buf[13]<<8);

	
	return status;
}
int bsp_IcmGetRawData(float accel_mg[3], float gyro_dps[3], float *temp_degc)//数值处理函数
{
	int rc = 0;
	icm45686_sensor_data d;
	
	rc |= inv_imu_get_register_data(&d);
	//SI_CHECK_RC(rc);
	
	accel_mg[0] = (float)(d.accel_data[0])/8192.0f*9.8f;
	accel_mg[1] = (float)(d.accel_data[1])/8192.0f*9.8f;
	accel_mg[2] = (float)(d.accel_data[2])/8192.0f*9.8f;
	gyro_dps[0] = (float)(d.gyro_data[0])/65.0f;
	gyro_dps[1] = (float)(d.gyro_data[1])/65.0f;
	gyro_dps[2] = (float)(d.gyro_data[2])/65.0f;
	*temp_degc  = (float)(25+((d.temp_data ))/128.0f);
	//计算方法：65535/2/目标最大值，温度传感器标定公式为官方手册提供
	return 0;
}