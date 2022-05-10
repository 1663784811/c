/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/30
  *****************************************************************************
  */
#ifndef __DEV_KEY_H
#define __DEV_KEY_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include <stdbool.h>

/* Public defines -----------------------------------------------------------*/
#define PERIOD_MS_KEY     5  // ms

/* Public Struct ------------------------------------------------------------*/
typedef struct {
  uint32_t upTim;    //����δ���³���ʱ��
  uint32_t dowmTim;  //�������³���ʱ��
  uint8_t  clickCnt; //�����̰�����
} wireKeyBit_t;

/* Public enum --------------------------------------------------------------*/

/* Public Variable ----------------------------------------------------------*/
//Pointer

//Array

//Const

/* Pubilc function prototypes -----------------------------------------------*/
void Dev_Key_Init(void);
uint16_t Dev_Key_Scan(void);
wireKeyBit_t Dev_Key_Data_Get(uint8_t id);

#ifdef __cplusplus
}
#endif

#endif
