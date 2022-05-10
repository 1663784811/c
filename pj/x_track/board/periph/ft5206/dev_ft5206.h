/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/08/01
  *****************************************************************************
  */
#ifndef __DEV_FT5206_H
#define __DEV_FT5206_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

/* Public defines -----------------------------------------------------------*/

//FT5206 部分寄存器定义 
#define FT_DEVIDE_MODE        0x00  //FT5206模式控制寄存器
#define FT_REG_NUM_FINGER     0x02  //触摸状态寄存器

#define FT_TP1_REG            0X03  //第一个触摸点数据地址
#define FT_TP2_REG            0X09  //第二个触摸点数据地址
#define FT_TP3_REG            0X0F  //第三个触摸点数据地址
#define FT_TP4_REG            0X15  //第四个触摸点数据地址
#define FT_TP5_REG            0X1B  //第五个触摸点数据地址

#define FT_ID_G_LIB_VERSION   0xA1  //版本
#define FT_ID_G_MODE          0xA4  //FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP       0x80  //触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE  0x88  //激活状态周期设置寄存器

/* Public Struct ------------------------------------------------------------*/

/* Public enum --------------------------------------------------------------*/

/* Public Variable ----------------------------------------------------------*/
//Pointer

//Array

//Const

/* Pubilc function prototypes -----------------------------------------------*/
uint8_t Dev_FT5206_Init(void);
uint8_t Dev_FT5206_Is_Touch(void);
void Dev_FT5206_Get_XY(uint16_t *x, uint16_t *y);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __DEV_FT5206_H */
