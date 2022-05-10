/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/10/10
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "task_gps.h"
#include "drv_usart.h"
#include "TinyGPSPlus/src/TinyGPS++.h"
#include "Common/HAL/HAL.h"

/* Private defines ----------------------------------------------------------*/
#define GPS_USART USART3

/* Private Struct -----------------------------------------------------------*/

/* 二值信号量,用于同步中断和任务 */
SemaphoreHandle_t gps_received_Semaphore;

TinyGPSPlus gps;

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
//Pointer

//Array

//Const

/* Private function prototypes ----------------------------------------------*/
static void Gps_Receive_Irq_Callback(void);

/*****************************************************************************
* @brief  
* ex:
* @par
* None
* @retval
*****************************************************************************/
void task_gps(void *arg)
{
    /* 创建信号量 */
    uint16_t Gps_Receive_Len = 0;
    uint8_t *Gps_Receive_Buf = (uint8_t *)pvPortMalloc(1030);
    gps_received_Semaphore = xSemaphoreCreateBinary();

    Drv_Usart_RxIrq_Connect(GPS_USART, Gps_Receive_Irq_Callback);
    Drv_Usart_Init(GPS_USART, 38400, 1024, 1024, 6, 0);

    while (1)
    {
        /* 等待数据的到来 */
        xSemaphoreTake(gps_received_Semaphore, portMAX_DELAY);
        Gps_Receive_Len = Drv_Usart_Read(GPS_USART, Gps_Receive_Buf, 1030);
        for (uint16_t i = 0; i < Gps_Receive_Len; i++)
        {
            gps.encode(Gps_Receive_Buf[i]);
        }
    }
}

/*****************************************************************************
* @brief  串口接收数据回调
* ex:
* @par
* None
* @retval
*****************************************************************************/
static void Gps_Receive_Irq_Callback(void)
{
  static portBASE_TYPE xHigherPriorityTaskWoken;
  
  xSemaphoreGiveFromISR(gps_received_Semaphore, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

bool HAL::GPS_GetInfo(GPS_Info_t* info)
{
    memset(info, 0, sizeof(GPS_Info_t));

    info->isVaild = gps.location.isValid();
    info->longitude = gps.location.lng();
    info->latitude = gps.location.lat();
    info->altitude = gps.altitude.meters();
    info->speed = gps.speed.kmph();
    info->compass = gps.course.deg();

    info->clock.year = gps.date.year();
    info->clock.month = gps.date.month();
    info->clock.day = gps.date.day();
    info->clock.hour = gps.time.hour();
    info->clock.minute = gps.time.minute();
    info->clock.second = gps.time.second();
    info->satellites = gps.satellites.value();

    return info->isVaild;
}

bool HAL::GPS_LocationIsValid()
{
    return gps.location.isValid();
}

double HAL::GPS_GetDistanceOffset(GPS_Info_t* info,  double preLong, double preLat)
{
    return gps.distanceBetween(info->latitude, info->longitude, preLat, preLong);
}
