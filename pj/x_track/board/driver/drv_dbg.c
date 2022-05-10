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
#include "drv_dbg.h"
#include "drv_usart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Private defines ----------------------------------------------------------*/

/* Private Struct -----------------------------------------------------------*/

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
static USART_TypeDef *dbg_usart = NULL;

//Pointer

//Array

//Const

/* Private function prototypes ----------------------------------------------*/


/*****************************************************************************
* @brief  调试串口初始化
* ex:
* @par
* None
* @retval
*****************************************************************************/
void dbg_init(USART_TypeDef *USARTx)
{
  dbg_usart = USARTx;
  Drv_Usart_Init(USARTx, 115200, 1024, 1024, 7, 0);
}

/*****************************************************************************
* @brief  调试信息输出
* ex:
* @par
* None
* @retval
*****************************************************************************/
void dbg_printf(const char* fmt, ...)
{
  va_list ap;
  char *buf = (char *)pvPortMalloc(256);
  
  if (buf == NULL || dbg_usart == NULL)
  {
    return;
  }
  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  va_end(ap);
  Drv_Usart_Write(dbg_usart, (const char *)buf, strlen(buf));
  
  vPortFree(buf);
}
