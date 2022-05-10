/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/31
  *****************************************************************************
  */
#ifndef __DRV_DBG_H
#define __DRV_DBG_H

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
void dbg_init(USART_TypeDef *USARTx);
void dbg_printf(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_DBG_H */
