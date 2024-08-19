#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"


typedef void (*MotorFun)(void);

// ��ʼ��ϵͳʱ��
void SysTick_Init(void);

// ��ʱus
void Delay_us(__IO u32 nsTime);

// ��ʱms
void Delay_ms(__IO u32 msTime);

// ��ʱs
void Delay_s(__IO u32 sTime);


void MotorStartTime(MotorFun motorFun);

#endif
