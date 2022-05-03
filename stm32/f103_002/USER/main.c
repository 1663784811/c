#include "stm32f10x.h"
#include "stm32_eval.h"
#include <stdio.h>


void usartOut(void);


int main(void){


    usartOut();








}


/**
 * 串口输出
 */
void usartOut(void){

    // 波特率
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    STM_EVAL_COMInit(COM1, &USART_InitStructure);




}


