/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/06/28
  *****************************************************************************
  */
#ifndef __DRV_IIC_SOFT_H
#define __DRV_IIC_SOFT_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

/* Public defines -----------------------------------------------------------*/
#define DRV_IIC1   0

/* Public Struct ------------------------------------------------------------*/

/* Public enum --------------------------------------------------------------*/

/* Public Variable ----------------------------------------------------------*/
//Pointer

//Array

//Const

/* Pubilc function prototypes -----------------------------------------------*/
void    Drv_IIC_Soft_Init(int drv);
void    Drv_IIC_Soft_Start(int drv);
void    Drv_IIC_Soft_Stop(int drv);
uint8_t Drv_IIC_Soft_Wait_Ack(int drv);
void    Drv_IIC_Soft_Ack(int drv);
void    Drv_IIC_Soft_NAck(int drv);
void    Drv_IIC_Soft_Send_Byte(int drv, uint8_t txd);
uint8_t Drv_IIC_Soft_Read_Byte(int drv, uint8_t ack);

#ifdef __cplusplus
}
#endif

#endif
