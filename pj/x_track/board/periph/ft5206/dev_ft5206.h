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

//FT5206 ���ּĴ������� 
#define FT_DEVIDE_MODE        0x00  //FT5206ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER     0x02  //����״̬�Ĵ���

#define FT_TP1_REG            0X03  //��һ�����������ݵ�ַ
#define FT_TP2_REG            0X09  //�ڶ������������ݵ�ַ
#define FT_TP3_REG            0X0F  //���������������ݵ�ַ
#define FT_TP4_REG            0X15  //���ĸ����������ݵ�ַ
#define FT_TP5_REG            0X1B  //��������������ݵ�ַ

#define FT_ID_G_LIB_VERSION   0xA1  //�汾
#define FT_ID_G_MODE          0xA4  //FT5206�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP       0x80  //������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE  0x88  //����״̬�������üĴ���

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
