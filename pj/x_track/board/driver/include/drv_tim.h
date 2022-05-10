/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/17
  *****************************************************************************
  */
#ifndef __DRV_TIM_H
#define __DRV_TIM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "board.h"

/* Public defines -----------------------------------------------------------*/

/* Public Struct ------------------------------------------------------------*/

/* Public enum --------------------------------------------------------------*/

/* Public Variable ----------------------------------------------------------*/
//Pointer

//Array

//Const

/* Pubilc function prototypes -----------------------------------------------*/
void Drv_Tim5_Init(uint16_t psc, uint16_t arr, uint8_t PrePri, uint8_t SubPri, void (*irq)(void));

#ifdef __cplusplus
}
#endif

#endif /* __DRV_TIM_H */
