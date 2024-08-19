#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"


typedef void (*MotorFun)(void);

// 初始化系统时钟
void SysTick_Init(void);

// 延时us
void Delay_us(__IO u32 nsTime);

// 延时ms
void Delay_ms(__IO u32 msTime);

// 延时s
void Delay_s(__IO u32 sTime);


void MotorStartTime(MotorFun motorFun);

#endif
