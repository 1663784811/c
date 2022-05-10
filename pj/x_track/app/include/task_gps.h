/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/10/10
  *****************************************************************************
  */
#ifndef __TASK_GPS_H
#define __TASK_GPS_H

#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Public defines -----------------------------------------------------------*/

/* Public Struct ------------------------------------------------------------*/

/* Public enum --------------------------------------------------------------*/

/* Public Variable ----------------------------------------------------------*/
//Pointer

//Array

//Const

/* Pubilc function prototypes -----------------------------------------------*/
void task_gps(void *arg);

#endif /* __TASK_GPS_H */
