/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/06/28
  *****************************************************************************
  */
#ifndef __TASK_LVGL_H
#define __TASK_LVGL_H

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
void task_lvgl(void *arg);

#endif /* __TASK_LVGL_H */
