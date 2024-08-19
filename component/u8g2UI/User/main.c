#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_usart.h"
#include "bsp_motor.h"

#include "u8g2.h"
#include "oled.h"



void Delay(uint32_t count);
void draw(u8g2_t *u8g2);








int main(void){

	
	SysTick_Init();
	// LED_GPIO_Config();
	//EXTI_Key_Config();
	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  USART_Config();
	// ��ʼ�����
	Motor_Config();

  U8g2Init();
	
	
	while(1){
		
		
	}
}


