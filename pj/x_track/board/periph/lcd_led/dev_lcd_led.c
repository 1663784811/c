/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/31
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "dev_lcd_led.h"

/* Private defines ----------------------------------------------------------*/

/* ���� */
#define DEV_LCD_LED_RCC       RCC_AHB1Periph_GPIOB
#define DEV_LCD_LED_PORT      GPIOB
#define DEV_LCD_LED_PIN       GPIO_Pin_5

/* Private Struct -----------------------------------------------------------*/

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
//Pointer

//Array

//Const

/* Private function prototypes ----------------------------------------------*/


/*****************************************************************************
* @brief  ��ʼ��
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Dev_Lcd_Led_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(DEV_LCD_LED_RCC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin   = DEV_LCD_LED_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(DEV_LCD_LED_PORT, &GPIO_InitStructure);
  
  Dev_Lcd_Led_Off();
}

/*****************************************************************************
* @brief  ����
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Dev_Lcd_Led_On(void)
{
  GPIO_SetBits(DEV_LCD_LED_PORT, DEV_LCD_LED_PIN);
}

/*****************************************************************************
* @brief  �ص�
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Dev_Lcd_Led_Off(void)
{
  GPIO_ResetBits(DEV_LCD_LED_PORT, DEV_LCD_LED_PIN);
}

/*****************************************************************************
* @brief  �Զ������õ�ƽ
* ex:
* @par
* val:0,�ص�;1,����
* @retval
*****************************************************************************/
void Dev_Lcd_Led_Set(int val)
{
  val ? Dev_Lcd_Led_On() : Dev_Lcd_Led_Off();
}

