#include <stdio.h>
#include "stm32f10x.h"


/**
 * =============================    宏定义
 */


// 变量
USART_InitTypeDef USART_InitStructure;

/**
 * =============================   函数定义
 */

void usartOut(void);

/**
 * =============================    主启动类
 */
int main(void) {

    usartOut();

    while (1) {
        printf("\n\r %s", "s");
    }
    return 0;
}


/**
 * 串口输出
 */
void usartOut(void) {
    // 波特率
    USART_InitStructure.USART_BaudRate = 115200;
    // 字长度8位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    //
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    //
    USART_InitStructure.USART_Parity = USART_Parity_No;
    //
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    // 模式
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;



    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
    /* Enable UART clock */
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitTypeDef usart1_tx;
    usart1_tx.GPIO_Mode = GPIO_Mode_AF_PP;
    usart1_tx.GPIO_Pin = GPIO_Pin_6;
    usart1_tx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &usart1_tx);

    /* Configure USART Rx as input floating */
    GPIO_InitTypeDef usart1_rx;
    usart1_rx.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    usart1_rx.GPIO_Pin = GPIO_Pin_7;
    usart1_rx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &usart1_rx);

    /* USART configuration */
    USART_Init(USART1, &USART_InitStructure);

    /* Enable USART */
    USART_Cmd(USART1, ENABLE);


}



