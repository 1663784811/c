#include "board.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lvgl/src/hal/lv_hal_tick.h"

#define CALIBRATION_TICKS 500000UL
uint32_t _ticks_per_us = 8;

volatile uint32_t uwTick = 0;

void xPortSysTickHandler(void);

/*
 * 系统时钟设定函数，时间：1ms
 */
static void set_board_systick(void)
{
  SysTick->LOAD = SYSTICK_1MS_TICKS - 1;        //SysTick->LOAD重载值
  NVIC_SetPriority(SysTick_IRQn, 0x10);         //中断优先级
  SysTick->VAL = 0;                             //SysTick->VAL当前值
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;
}

static void _delay_loop(volatile uint32_t count)
{
  while(count--);
}

/*
*********************************************************************************
*	函 数 名: softdelay_calibrate
*	功能说明: 微秒级延时矫正函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************
*/
static void softdelay_calibrate(void)
{
  //计时一个比较大的数，看所需的时间
  uint64_t startUs = getus();
  _delay_loop(CALIBRATION_TICKS);
  uint64_t usedTime = getus() - startUs;

  //usedTime是不会为0的，这一步只是以防万一
  if(!usedTime) usedTime = 1;

  //计算每us需要的数
  _ticks_per_us = CALIBRATION_TICKS/usedTime;

  //_ticks_per_us也是不会为0的，这一步也只是以防万一
  if(!_ticks_per_us) _ticks_per_us = 1;
}

/*
*********************************************************************************
*	函 数 名: IncTick
*	功能说明: SysTick滴答定时器每1ms中断一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************
*/
void IncTick(void)
{
  uwTick++;

  lv_tick_inc(1);

  /*系统已经运行*/
  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    xPortSysTickHandler();
  }
}

/*
*********************************************************************************
*	函 数 名: board_init
*	功能说明: MCU最底层初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************
*/
void board_init(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //中断分组4，只有抢占优先级

  set_board_systick();
  softdelay_calibrate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * 返回微秒数
 */
uint64_t getus(void)
{
  //***********************************
  //SysTick为24位倒计时定时器
  //SysTick->LOAD重载值
  //SysTick->VAL当前值
  //***********************************

  //总运行时间 = uwTick * 1000 + us
  //us = (SysTick->LOAD - SysTick->VAL) * 1000 / (SysTick->LOAD + 1)
  return ((uint64_t)uwTick * 1000 + ((SYSTICK_1MS_TICKS - 1 - SysTick->VAL) * SYSTICK_uS_PER_TICK));
}

/*
 * 返回毫秒数
 */
uint32_t getms(void)
{
  return uwTick;
}

/*
*********************************************************************************
*	函 数 名: delay_us
*	功能说明: us延时
*	形    参：us,要延时的us数
*	返 回 值: 无
*********************************************************************************
*/
void delay_us(uint32_t us)
{
  _delay_loop(us * _ticks_per_us);
}

/*
*********************************************************************************
*	函 数 名: delay_ms
*	功能说明: ms级延时
*	形    参：ms,要延时的ms数
*	返 回 值: 无
*********************************************************************************
*/
void delay_ms(uint16_t ms)
{
  uint32_t targettime = getms() + ms;
  while(getms() < targettime);
}
