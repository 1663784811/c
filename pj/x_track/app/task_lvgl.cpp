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
#include "task_lvgl.h"
#include "drv_ltdc.h"

#include "lvgl/porting/lv_port_disp.h"
#include "lvgl/porting/lv_port_indev.h"
#include "lvgl/porting/lv_port_fs.h"

#include "lvgl_demos/lv_demo.h"
#include "app.h"

/* Private defines ----------------------------------------------------------*/

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
* @retval  开源轻量级显示框架
*****************************************************************************/
void task_lvgl(void *arg)
{
  lv_init();
  
  lv_port_disp_init();
  
  lv_port_indev_init();
  
  lv_port_fs_init();
  
  App_Init();
  
//  lv_demo_widgets();
//  lv_demo_keypad_encoder();
  
  while (1)
  {
    lv_task_handler();
    vTaskDelay(5);
  }
}
