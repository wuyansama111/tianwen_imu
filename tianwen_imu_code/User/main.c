/*!
    \file    main.c
    \brief   led spark with systick

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/
//
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "bsp_led.h"
#include "sys.h"
#include "bsp_usart.h"
#include "IIC.h"
#include "FreeRTOS.h"
#include "task.h"
#include "imu.h"


void LED_init(void);
void CrateTask(void *pvParameters);
void task_a(void *pvParameters);
void task_b(void *pvParameters);
void uart_task(void *pvParameters);
void iic_task(void *pvParameters);
void icm45686_task(void *pvParameters);
TaskHandle_t StartTask_Handler;
TaskHandle_t uart_Handler;
TaskHandle_t iic_Handler;
TaskHandle_t icm45686_Handler;
/*!
    \brief    main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{

	uint16_t uicount = 0;
	float fcount = 0.0;
	
    systick_config();
	led_gpio_config();  
	usart_gpio_config(9600U);
	iic_gpio_config();
	xTaskCreate(CrateTask ,"CrateTask" , 128, NULL, 1, &StartTask_Handler);
	xTaskCreate(uart_task ,"uart_task" , 128, NULL, 1, &uart_Handler);
	xTaskCreate(icm45686_task ,"icm45686_task" , 2048, NULL, 1, &icm45686_Handler);
	//xTaskCreate(iic_task ,"iic_task" , 2048, NULL, 1, &iic_Handler);
	vTaskStartScheduler();          
    while(1) 
		{
			
    }
}

void iic_task(void *pvParameters)//iic线程函数,主要调试用
{
	uint8_t slaveadress = 0x69;
	uint8_t regadress = 0x00;
	uint8_t wham = 0x11;
	char d[2048];
	imu_init();
	int flag1=1;
	float accel_mg[3];float gyro_dps[3];float temp;

	while(1)
	{
		bsp_IcmGetRawData(accel_mg,gyro_dps,&temp);
		sprintf(d,"icm_data:%f,%f,%f,%f,%f,%f,%f\n",accel_mg[0],accel_mg [1],accel_mg [2],gyro_dps [0],gyro_dps [1],gyro_dps [2],temp );
		usart_send_string((uint8_t *)d);
	
	}
}
void icm45686_task(void *pvParameters)//传感器线程函数
{	
	char d[256];
	float accel_mg[3];float gyro_dps[3];float temp;
	imu_init();
	while(1)
	{
		bsp_IcmGetRawData(accel_mg,gyro_dps,&temp);
		sprintf(d,"icm_data:%f,%f,%f,%f,%f,%f,%f\n",accel_mg[0],accel_mg [1],accel_mg [2],gyro_dps [0],gyro_dps [1],gyro_dps [2],temp );
		//firewater格式数据   "<any>:ch0,ch1,ch2,...,chN\n"
		usart_send_string((uint8_t *)d);
	}
}
void uart_task(void *pvParameters)//串口线程，调试用
{
 
  uint8_t text[] = "shore\r\n";
  
  while(1)
  {
    //usart_send_string(text);

  }


}

void CrateTask(void *pvParameters){
	
	taskENTER_CRITICAL();
	
	xTaskCreate(task_a , "task_a" ,50 ,NULL , 2 ,0);
	
	xTaskCreate(task_b , "task_b" ,50 ,NULL , 2 ,0);
	
	vTaskDelete(StartTask_Handler);
	 
	taskEXIT_CRITICAL(); 
	
}

void task_a(void *pvParameters)
{  

	while(1){
		
    PDout(7) = 1;
		vTaskDelay(200);
		PDout(7) = 1;
		vTaskDelay(200);
	}
}   

void task_b(void *pvParameters)
{
	while(1){
		PAout(5) = 1;
		vTaskDelay(200);
		PAout(5) = 1;
		vTaskDelay(200);
		
	}
}
