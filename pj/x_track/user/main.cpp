/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "drv_dbg.h"
#include "drv_sdram.h"
#include "drv_sdio_sd.h"
#include "dev_ft5206.h"

#include "task_monitor.h"
#include "task_led.h"
#include "task_key.h"
#include "task_lvgl.h"
#include "task_gps.h"

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    /**
     * MCU最底层初始化
     */
  board_init();

  /**
   * 调试串口初始化
   */
  dbg_init(USART1);

  /**
   * SD卡初始化
   */
  drv_sdio_sd_init();

  /**
   * sdram初始化
   */
  Drv_SDRAM_Init();

  /**
   * 进入临界段
   */
  taskENTER_CRITICAL();

  // ===============================================================   任务

  // 监测任务,查看其它任务的数据,用于调试
  xTaskCreate(task_monitor,  "MONITOR", 600, NULL, 7, NULL);
  // 开源轻量级显示框架
  xTaskCreate(task_lvgl,     "LVGL",   1850, NULL, 6, NULL);
  //
  xTaskCreate(task_key,      "KEY",     100, NULL, 4, NULL);
  // led
  xTaskCreate(task_led,      "LED",     100, NULL, 3, NULL);
  // GPS
  xTaskCreate(task_gps,      "GPS",     500, NULL, 3, NULL);

  // ==============================================================

  /**
   * 退出临界段
   */
  taskEXIT_CRITICAL();

  /**
   * 启动实时内核处理
   */
  vTaskStartScheduler();
  
  /* Infinite loop */
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
