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

/* 背光 */
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
* @brief  初始化
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
* @brief  开灯
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
* @brief  关灯
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
* @brief  自定义设置电平
* ex:
* @par
* val:0,关灯;1,开灯
* @retval
*****************************************************************************/
void Dev_Lcd_Led_Set(int val)
{
  val ? Dev_Lcd_Led_On() : Dev_Lcd_Led_Off();
}

