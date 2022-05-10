/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/27
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "task_led.h"
#include "dev_led.h"

/* Private defines ----------------------------------------------------------*/
#define LED_EVENT_DEFAULT       0x00010001

/* Private Struct -----------------------------------------------------------*/

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
//Pointer

//Array

//Const

/* Private function prototypes ----------------------------------------------*/


/*****************************************************************************
* @brief  
* ex:
* @par
* None
* @retval
*****************************************************************************/
void task_led(void *arg)
{
  uint8_t  ledBitCnt = 0;
  uint32_t currentLedEvent = LED_EVENT_DEFAULT;
  
  Dev_Led_Init();
  
  while (1)
  {
    vTaskDelay(125);
    
    /* ִ��led��˸ */
    Dev_Led_Set(DEV_LED1, (currentLedEvent << ledBitCnt) & 0x80000000);
    ledBitCnt = (ledBitCnt + 1) % 32;
  }
}
