/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/30
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "dev_led.h"

/* Private defines ----------------------------------------------------------*/
#define DEV_LED0_RCC    RCC_AHB1Periph_GPIOB
#define DEV_LED0_PORT   GPIOB
#define DEV_LED0_PIN    GPIO_Pin_1

#define DEV_LED1_RCC    RCC_AHB1Periph_GPIOB
#define DEV_LED1_PORT   GPIOB
#define DEV_LED1_PIN    GPIO_Pin_0

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
void Dev_Led_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(DEV_LED0_RCC, ENABLE);
  RCC_AHB1PeriphClockCmd(DEV_LED1_RCC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin   = DEV_LED0_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(DEV_LED0_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = DEV_LED1_PIN;
  GPIO_Init(DEV_LED1_PORT, &GPIO_InitStructure);
  
  Dev_Led_Off(DEV_LED0);
  Dev_Led_Off(DEV_LED1);
}

/*****************************************************************************
* @brief  ����
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Dev_Led_On(int dev)
{
  switch (dev)
  {
    case DEV_LED0:
      GPIO_ResetBits(DEV_LED0_PORT, DEV_LED0_PIN);
      break;
    
    case DEV_LED1:
      GPIO_ResetBits(DEV_LED1_PORT, DEV_LED1_PIN);
      break;
    
    default:
      break;
  }
}

/*****************************************************************************
* @brief  �ص�
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Dev_Led_Off(int dev)
{
  switch (dev)
  {
    case DEV_LED0:
      GPIO_SetBits(DEV_LED0_PORT, DEV_LED0_PIN);
      break;
    
    case DEV_LED1:
      GPIO_SetBits(DEV_LED1_PORT, DEV_LED1_PIN);
      break;
    
    default:
      break;
  }
}

/*****************************************************************************
* @brief  �Զ������õ�ƽ
* ex:
* @par
* val:0,�ص�;1,����
* @retval
*****************************************************************************/
void Dev_Led_Set(int dev, int val)
{
  val ? Dev_Led_On(dev) : Dev_Led_Off(dev);
}
