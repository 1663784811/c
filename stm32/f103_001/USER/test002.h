//
// Created by why on 2022/5/6.
//

#ifndef S_TEST002_H
#define S_TEST002_H

#include "stm32f10x.h"
#include <stdio.h>

#define printf_USARTx		USART1	//重定向printf的串口
#define scanf_USARTx		USART1	//重定向scanf等的串口


void test002(void);

void USART_Config(u8 UsartN,int baudrate,u8 Ppro,u8 Spro);//串口外设和GPIO初始化及相关参数设定
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);//串口发送一个字节
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);//串口发送8位的数组
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);//串口发送字符串
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);//串口发送一个16位数



#endif //S_TEST002_H
