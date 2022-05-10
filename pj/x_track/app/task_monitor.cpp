/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/17
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "task_monitor.h"
#include "drv_tim.h"
#include "drv_dbg.h"

/* Private defines ----------------------------------------------------------*/

/* Private Struct -----------------------------------------------------------*/

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
volatile uint32_t FreeRTOSRunTimeTicks = 0;

//Pointer

//Array

//Const

/* Private function prototypes ----------------------------------------------*/

/*****************************************************************************
* @brief  ��ʱ��5�жϻص�
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_Tim5_IRQ_CallBack(void)
{
  FreeRTOSRunTimeTicks++;
}

/*****************************************************************************
* @brief  ��ʼ��
* ex:
* @par
* None
* @retval
*****************************************************************************/
void TaskGetRunTimeStats_Init(void)
{
  FreeRTOSRunTimeTicks = 0;
  Drv_Tim5_Init(180 / 2 - 1, 50 - 1, 6, 0, Drv_Tim5_IRQ_CallBack); /* 50us�ж�һ�� */
}

/*****************************************************************************
* @brief  �������,�鿴�������������,���ڵ���
* ex:
* @par
* None
* @retval
*****************************************************************************/
void task_monitor(void *arg)
{
  char monBuf[1024] = {0};
  
  while (1)
  {
    vTaskDelay(2000);
    
    vTaskList(monBuf);
    dbg_printf("Name    	State	Pri	Stack	Num\n");
    dbg_printf("*******************************************\n");
    dbg_printf("%s\n", monBuf);
    
    vTaskGetRunTimeStats(monBuf);
    dbg_printf("Name    	Time		TimPercent\n");
    dbg_printf("*******************************************\n");
    dbg_printf("%s\n", monBuf);
  }
}
