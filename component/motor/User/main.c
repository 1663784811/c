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
	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  USART_Config();
	// ��ʼ�����
	Motor_Config();
	
	while(1){
		Usart_SendString( DEBUG_USARTx,"����һ�������жϽ��ջ���ʵ��\n");
		// LED_G(OFF);
		// Delay(0xFFFFFF);
		// Usart_SendString( DEBUG_USARTx,"����һ�������жϽ��ջ���ʵ��\n");
	  // LED_G(ON);
		
		
		//Delay_s(1);
		

		MotorGo(1000, 3000);
		
		Delay_s(1);
	}
}

