
/*********************************************
STM32F103 ����ͨ�� ģ���ƶ�����
��ȡcmd_vel��Ϣ ����odom��Ϣ

Debug Port:
					PA9   USART1_TX    
					PA10  USART1_RX
Communication Port:
					PB10	USART3_TX
					PB11	USART3_RX
@StevenShi
*********************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "debug_x.h"
#include "com_x.h"

#include "stdio.h"
#include "string.h"

/**********************************************************************************************************************/
//int main(void)
//{
//
//
//	delay_init();
//
//	com_x_usart_Init();
//
//	debug_x_usart_Init();
//
//	delay_ms(1000);
//
//	printf("\n\r--------------mobilebase����---------------------\n ");
//
//	while (1)
//	{
//
//		printf("\n\r--------------���ݷ���---------------------\n ");
//
//		data_pack();
//
//		delay_ms(1000);
//
//	}
//}
