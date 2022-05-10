/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/06/28
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "dev_key.h"

/* Private defines ----------------------------------------------------------*/
#define DEV_KEY_0_RCC     RCC_AHB1Periph_GPIOH
#define DEV_KEY_0_PORT    GPIOH
#define DEV_KEY_0_PIN     GPIO_Pin_3

#define DEV_KEY_1_RCC     RCC_AHB1Periph_GPIOH
#define DEV_KEY_1_PORT    GPIOH
#define DEV_KEY_1_PIN     GPIO_Pin_2

#define DEV_KEY_2_RCC     RCC_AHB1Periph_GPIOC
#define DEV_KEY_2_PORT    GPIOC
#define DEV_KEY_2_PIN     GPIO_Pin_13

#define DEV_KEY_UP_RCC    RCC_AHB1Periph_GPIOA
#define DEV_KEY_UP_PORT   GPIOA
#define DEV_KEY_UP_PIN    GPIO_Pin_0

#define CONFIG_KEY_NUM    4

/* Private Struct -----------------------------------------------------------*/

typedef struct {
  GPIO_TypeDef *port;
  uint16_t      pin;
} key_gpio_t;

//管脚定义
const key_gpio_t wireKeyGpio[CONFIG_KEY_NUM] = {
  { DEV_KEY_0_PORT,  DEV_KEY_0_PIN  },
  { DEV_KEY_1_PORT,  DEV_KEY_1_PIN  },
  { DEV_KEY_2_PORT,  DEV_KEY_2_PIN  },
  { DEV_KEY_UP_PORT, DEV_KEY_UP_PIN }
};

wireKeyBit_t wireKeyBit[CONFIG_KEY_NUM];

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
//Pointer

//Array

//Const

/* Private function prototypes ----------------------------------------------*/

/*****************************************************************************
* @brief  按键初始化
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Dev_Key_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(DEV_KEY_0_RCC,  ENABLE);
  RCC_AHB1PeriphClockCmd(DEV_KEY_1_RCC,  ENABLE);
  RCC_AHB1PeriphClockCmd(DEV_KEY_2_RCC,  ENABLE);
  RCC_AHB1PeriphClockCmd(DEV_KEY_UP_RCC, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin   = DEV_KEY_0_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(DEV_KEY_0_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = DEV_KEY_1_PIN;
  GPIO_Init(DEV_KEY_1_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = DEV_KEY_2_PIN;
  GPIO_Init(DEV_KEY_2_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = DEV_KEY_UP_PIN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(DEV_KEY_UP_PORT, &GPIO_InitStructure);
}

/*****************************************************************************
* @brief  读单个按键的值
* ex:
* @par
* None
* @retval
*****************************************************************************/
bool Dev_Read_Key(uint8_t n)
{
  if (n == 3)
  {
    return (GPIO_ReadInputDataBit(wireKeyGpio[n].port, wireKeyGpio[n].pin) ? true : false);
  }
  return (GPIO_ReadInputDataBit(wireKeyGpio[n].port, wireKeyGpio[n].pin) ? false : true);
}

/*****************************************************************************
* @brief  按键扫描
* ex:
* @par
* None
* @retval
*****************************************************************************/
uint16_t Dev_Key_Scan(void)
{
  static uint8_t  cnt = 0, clickCnt_temp[CONFIG_KEY_NUM];
  static uint16_t key = 0, key_temp[3]; //环形缓存

  key_temp[cnt] = 0;

  //读IO口数据,并通过移位操作汇总到2字节数据上
  for (uint8_t i = 0; i < CONFIG_KEY_NUM; i++) {
    key_temp[cnt] |= (uint16_t)Dev_Read_Key(i) << i; //记录被按下的按键
  }
  cnt = (cnt + 1) % 3;
  
  //如果3次扫描的结果相同,则认为扫描结果有效
  if ((key_temp[0] == key_temp[1]) && (key_temp[1] == key_temp[2])) {
    key = key_temp[0];
  }

  //按键计时和点击次数统计,可根据功能需求选择长按时间数据还是多击次数数据
  for (uint8_t i = 0; i < CONFIG_KEY_NUM; i++) {
    wireKeyBit[i].dowmTim = (wireKeyBit[i].dowmTim + PERIOD_MS_KEY) * (( key >> i) & 0x0001); //按键按下计时
    wireKeyBit[i].upTim   = (wireKeyBit[i].upTim   + PERIOD_MS_KEY) * ((~key >> i) & 0x0001); //按键抬起计时
    
    if ((wireKeyBit[i].upTim >= 1000) || (wireKeyBit[i].dowmTim >= 250)) { //间隔过长,则不符合短按和连击的规律,清0
      clickCnt_temp[i] = 0;
      wireKeyBit[i].clickCnt = 0;
    } else if (wireKeyBit[i].dowmTim == 20) {    //按键短时间内连续点击次数统计,暂存
      clickCnt_temp[i]++;
    } else if (wireKeyBit[i].upTim == 250) {     //按键抬起时间达到,连击次数生效
      wireKeyBit[i].clickCnt = clickCnt_temp[i];
      clickCnt_temp[i] = 0;
    }
  }
  
  return key;
}

/*****************************************************************************
* @brief  获取按键数据
* ex:
* @par
* None
* @retval
*****************************************************************************/
wireKeyBit_t Dev_Key_Data_Get(uint8_t id)
{
  return wireKeyBit[id];
}
