#include "board.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lvgl/src/hal/lv_hal_tick.h"

#define CALIBRATION_TICKS 500000UL
uint32_t _ticks_per_us = 8;

volatile uint32_t uwTick = 0;

void xPortSysTickHandler(void);

/*
 * ϵͳʱ���趨������ʱ�䣺1ms
 */
static void set_board_systick(void)
{
  SysTick->LOAD = SYSTICK_1MS_TICKS - 1;        //SysTick->LOAD����ֵ
  NVIC_SetPriority(SysTick_IRQn, 0x10);         //�ж����ȼ�
  SysTick->VAL = 0;                             //SysTick->VAL��ǰֵ
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
*	�� �� ��: softdelay_calibrate
*	����˵��: ΢�뼶��ʱ��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************
*/
static void softdelay_calibrate(void)
{
  //��ʱһ���Ƚϴ�������������ʱ��
  uint64_t startUs = getus();
  _delay_loop(CALIBRATION_TICKS);
  uint64_t usedTime = getus() - startUs;

  //usedTime�ǲ���Ϊ0�ģ���һ��ֻ���Է���һ
  if(!usedTime) usedTime = 1;

  //����ÿus��Ҫ����
  _ticks_per_us = CALIBRATION_TICKS/usedTime;

  //_ticks_per_usҲ�ǲ���Ϊ0�ģ���һ��Ҳֻ���Է���һ
  if(!_ticks_per_us) _ticks_per_us = 1;
}

/*
*********************************************************************************
*	�� �� ��: IncTick
*	����˵��: SysTick�δ�ʱ��ÿ1ms�ж�һ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************
*/
void IncTick(void)
{
  uwTick++;

  lv_tick_inc(1);

  /*ϵͳ�Ѿ�����*/
  if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    xPortSysTickHandler();
  }
}

/*
*********************************************************************************
*	�� �� ��: board_init
*	����˵��: MCU��ײ��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************
*/
void board_init(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //�жϷ���4��ֻ����ռ���ȼ�

  set_board_systick();
  softdelay_calibrate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * ����΢����
 */
uint64_t getus(void)
{
  //***********************************
  //SysTickΪ24λ����ʱ��ʱ��
  //SysTick->LOAD����ֵ
  //SysTick->VAL��ǰֵ
  //***********************************

  //������ʱ�� = uwTick * 1000 + us
  //us = (SysTick->LOAD - SysTick->VAL) * 1000 / (SysTick->LOAD + 1)
  return ((uint64_t)uwTick * 1000 + ((SYSTICK_1MS_TICKS - 1 - SysTick->VAL) * SYSTICK_uS_PER_TICK));
}

/*
 * ���غ�����
 */
uint32_t getms(void)
{
  return uwTick;
}

/*
*********************************************************************************
*	�� �� ��: delay_us
*	����˵��: us��ʱ
*	��    �Σ�us,Ҫ��ʱ��us��
*	�� �� ֵ: ��
*********************************************************************************
*/
void delay_us(uint32_t us)
{
  _delay_loop(us * _ticks_per_us);
}

/*
*********************************************************************************
*	�� �� ��: delay_ms
*	����˵��: ms����ʱ
*	��    �Σ�ms,Ҫ��ʱ��ms��
*	�� �� ֵ: ��
*********************************************************************************
*/
void delay_ms(uint16_t ms)
{
  uint32_t targettime = getms() + ms;
  while(getms() < targettime);
}
