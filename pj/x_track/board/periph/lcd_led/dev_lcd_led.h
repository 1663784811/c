/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/31
  *****************************************************************************
  */
#ifndef __DEV_LCD_LED_H
#define __DEV_LCD_LED_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

/* Public defines -----------------------------------------------------------*/

/* Public Struct ------------------------------------------------------------*/

/* Public enum --------------------------------------------------------------*/

/* Public Variable ----------------------------------------------------------*/
//Pointer

//Array

//Const

/* Pubilc function prototypes -----------------------------------------------*/
void Dev_Lcd_Led_Init(void);
void Dev_Lcd_Led_On(void);
void Dev_Lcd_Led_Off(void);
void Dev_Lcd_Led_Set(int val);

#ifdef __cplusplus
}
#endif

#endif /* __DEV_LCD_LED_H */
