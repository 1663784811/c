#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

/******************************************/
//RCCʱ��
#define CPU_FREQ       168000000L

#define SYSTICK_1MS_TICKS    ( CPU_FREQ / 1000 )          //1ms���񵴴���
#define SYSTICK_uS_PER_TICK  (1000L / SYSTICK_1MS_TICKS )

///////////////////////////////////////////////////////////////////

void IncTick(void);
void board_init(void);
uint64_t getus(void);
uint32_t getms(void);
void delay_us(uint32_t us);
void delay_ms(uint16_t ms);

#ifdef __cplusplus
}
#endif

#endif
