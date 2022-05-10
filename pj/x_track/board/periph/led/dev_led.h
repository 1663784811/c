/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/30
  *****************************************************************************
  */
#ifndef __DEV_LED_H
#define __DEV_LED_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

/* Public defines -----------------------------------------------------------*/
#define DEV_LED0    0
#define DEV_LED1    1

/* Public Struct ------------------------------------------------------------*/

/* Public enum --------------------------------------------------------------*/

/* Public Variable ----------------------------------------------------------*/
//Pointer

//Array

//Const

/* Pubilc function prototypes -----------------------------------------------*/
void Dev_Led_Init(void);
void Dev_Led_On(int dev);
void Dev_Led_Off(int dev);
void Dev_Led_Set(int dev, int val);

#ifdef __cplusplus
}
#endif

#endif /* __DEV_LED_H */
