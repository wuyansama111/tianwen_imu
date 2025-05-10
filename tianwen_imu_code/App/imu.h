#ifndef _IMU_H
#define _IMU_H

#include "main.h"
#include "imu.h"
#define INV_IMU_OK                   0 /**< Success */
#define INV_IMU_ERROR                -1 /**< Unspecified error */
#define INV_IMU_ERROR_TRANSPORT      -3 /**< Error occurred at transport level */
#define INV_IMU_ERROR_TIMEOUT        -4 /**< Action did not complete in the expected time window */
#define INV_IMU_ERROR_BAD_ARG        -11 /**< Invalid argument provided */
#define INV_IMU_ERROR_EDMP_ODR       -126 /**< EDMP ODR decimator reconfiguration is needed */
#define INV_IMU_ERROR_EDMP_BUF_EMPTY -127 /**< EDMP buffer is empty */
#define icm45686_slave_adress        0x69
#define icm45686_reg_adress          0x00
typedef struct {
	int16_t accel_data[3];
	int16_t gyro_data[3];
	int16_t temp_data;
} icm45686_sensor_data;
void imu_init(void);
int bsp_IcmGetRawData(float accel_mg[3], float gyro_dps[3], float *temp_degc);
int inv_imu_get_register_data(icm45686_sensor_data *data);
static int read_dreg(uint8_t slave ,uint8_t reg, uint32_t len, uint8_t *buf);
#endif