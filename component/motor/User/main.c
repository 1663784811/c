#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_usart.h"
#include "bsp_motor.h"



void Delay(uint32_t count);


int main(void){

	
	SysTick_Init();
	// LED_GPIO_Config();
	//EXTI_Key_Config();
	/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	// 初始化电机
	Motor_Config();
	
	while(1){
		Usart_SendString( DEBUG_USARTx,"这是一个串口中断接收回显实验\n");
		// LED_G(OFF);
		// Delay(0xFFFFFF);
		// Usart_SendString( DEBUG_USARTx,"这是一个串口中断接收回显实验\n");
	  // LED_G(ON);
		
		
		//Delay_s(1);
		

		MotorGo(1000, 3000);
		
		Delay_s(1);
	}
}

