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
	/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	// 初始化电机
	Motor_Config();

  U8g2Init();
	
	
	while(1){
		
		
	}
}


