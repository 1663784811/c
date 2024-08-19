#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

#include "stm32f10x.h"


#define  MOTOR_GPIO_CLK           			(RCC_APB2Periph_GPIOA)
#define  MOTOR_GPIO_APBxClkCmd    			RCC_APB2PeriphClockCmd
#define  PORT_MOTOR_GPIOx       				GPIOA   

#define  PIN_MOTOR_AP 									GPIO_Pin_1
#define  PIN_MOTOR_AM 									GPIO_Pin_2
#define  PIN_MOTOR_BP 									GPIO_Pin_3
#define  PIN_MOTOR_BM 									GPIO_Pin_4


// ���õ��
void Motor_Config(void);


// ���ǰ��һ��
void MotorGoOne(void);

// ��ֽ
void MotorGo(uint32_t steps, uint16_t wait);

// ֹͣ���
void MotorStop(void);

#endif
