/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/31
  *****************************************************************************
  */
#ifndef __DRV_SDRAM_H
#define __DRV_SDRAM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

/* Public defines -----------------------------------------------------------*/
#define Bank5_SDRAM_ADDR  ((uint32_t)(0XC0000000)) //SDRAM��ʼ��ַ

/* Public Struct ------------------------------------------------------------*/

/* Public enum --------------------------------------------------------------*/

/* Public Variable ----------------------------------------------------------*/
//Pointer

//Array

//Const

/* Pubilc function prototypes -----------------------------------------------*/
void Drv_SDRAM_Init(void);
void fsmc_sdram_test(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_SDRAM_H */
