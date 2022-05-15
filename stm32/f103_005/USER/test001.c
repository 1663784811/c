//
// Created by why on 2022/5/6.
//
#include "stm32f10x.h"
#include "test001.h"

/**
 * =============   常量变量定义
 */
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
/**
 * ==============  函数定义
 */
void test001(void);
void config(void);
void run(void);

void test001(void){
    config();
    run();
}

void config(void){
    // 第一步: 配置时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 第二步：配置GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void run(void){
    GPIO_SetBits(GPIOA,GPIO_Pin_9);
    GPIO_SetBits(GPIOA,GPIO_Pin_10);
}
