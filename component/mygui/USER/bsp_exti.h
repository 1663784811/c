#ifndef __EXTI_H
#define	__EXTI_H


#include "stm32f10x.h"


// ======================================  引脚定义 Key1            上
#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_6
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource6
#define KEY1_INT_EXTI_LINE         EXTI_Line6
#define KEY1_INT_EXTI_IRQ          EXTI9_5_IRQn

// ======================================  引脚定义 Key2            下
#define KEY2_INT_GPIO_PORT         GPIOA
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_7
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource7
#define KEY2_INT_EXTI_LINE         EXTI_Line7
#define KEY2_INT_EXTI_IRQ          EXTI9_5_IRQn


// ======================================  引脚定义 Key3            左
#define KEY3_INT_GPIO_PORT         GPIOA
#define KEY3_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY3_INT_GPIO_PIN          GPIO_Pin_8
#define KEY3_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY3_INT_EXTI_PINSOURCE    GPIO_PinSource8
#define KEY3_INT_EXTI_LINE         EXTI_Line8
#define KEY3_INT_EXTI_IRQ          EXTI9_5_IRQn



// ======================================  引脚定义 Key4            右
#define KEY4_INT_GPIO_PORT         GPIOA
#define KEY4_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY4_INT_GPIO_PIN          GPIO_Pin_9
#define KEY4_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define KEY4_INT_EXTI_PINSOURCE    GPIO_PinSource9
#define KEY4_INT_EXTI_LINE         EXTI_Line9
#define KEY4_INT_EXTI_IRQ          EXTI9_5_IRQn



#define KEY_EXTI9_5_Handler            EXTI9_5_IRQHandler




// ======================================  引脚定义 Key5            确定
#define KEY5_INT_GPIO_PORT         GPIOA
#define KEY5_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY5_INT_GPIO_PIN          GPIO_Pin_10
#define KEY5_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY5_INT_EXTI_PINSOURCE    GPIO_PinSource10
#define KEY5_INT_EXTI_LINE         EXTI_Line10
#define KEY5_INT_EXTI_IRQ          EXTI15_10_IRQn



#define KEY_EXTI15_10_Handler      EXTI15_10_IRQHandler



void EXTI_Key_Config(void);


#endif /* __EXTI_H */
