//
// Created by why on 2022/5/6.
//
#include "test002.h"
#include "stm32f10x.h"
#include "delay.h"
#include "debug_x.h"
#include "com_x.h"

#include "stdio.h"
#include "string.h"


void test002(void);

void test002(void){

	delay_init();
	com_x_usart_Init();
	debug_x_usart_Init();
	delay_ms(1000);
	printf("\n\r--------------mobilebase---------------------\n ");
	while (1)
	{
		printf("\n\r--------------555kkk---------------------\n ");
		data_pack();
		delay_ms(1000);
	}
}
