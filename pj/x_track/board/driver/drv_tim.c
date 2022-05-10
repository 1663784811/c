/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/17
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "drv_tim.h"

/* Private defines ----------------------------------------------------------*/

/* Private Struct -----------------------------------------------------------*/

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
//Pointer
void (*tim5_irq)(void) = NULL;

//Array

//Const

/* Private function prototypes ----------------------------------------------*/

/*****************************************************************************
* @brief  ��ʼ��
* ex:
* @par
* psc   :��Ƶ��
* arr   :װ��ֵ
* PrePri:�ж������ȼ�
* SubPri:�жϴ����ȼ�
* irq   :�жϻص�����
* @retval
*****************************************************************************/
void Drv_Tim5_Init(uint16_t psc, uint16_t arr, uint8_t PrePri, uint8_t SubPri, void (*irq)(void))
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;
  
  if (irq == NULL)
  {
    return;
  }
  tim5_irq = irq;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = psc;
  TIM_TimeBaseStructure.TIM_Period    = arr;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //�����ʼ�������и������жϵ�����
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM5, ENABLE);
  
  NVIC_InitStructure.NVIC_IRQChannel                   = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePri;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SubPri;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*****************************************************************************
* @brief  �жϷ�����
* ex:
* @par
* None
* @retval
*****************************************************************************/
void TIM5_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    if (tim5_irq != NULL)
    {
      tim5_irq();
    }
  }
}
