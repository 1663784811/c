#include "bsp_motor.h"
#include "bsp_SysTick.h"

static void MotorController(void);


/**
步进电机驱动部分
	数组0: AP
	数组1: AM
	数组2: BP
	数组3: BM
	
	4个位置为一个点
	
**/
uint8_t motorTable[8][4] =
{
  {1, 0, 0, 0},
  {1, 0, 1, 0},
  {0, 0, 1, 0},
  {0, 1, 1, 0},
  {0, 1, 0, 0},
  {0, 1, 0, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};


uint8_t motorPos = 0;




/*
  初始化二相四线电机
*/
void Motor_Config(void){
		
	GPIO_InitTypeDef GPIO_InitStructure;
	// 打开APBx 总线时钟
	MOTOR_GPIO_APBxClkCmd(MOTOR_GPIO_CLK , ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// 引脚1
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_AP;
	GPIO_Init(PORT_MOTOR_GPIOx, &GPIO_InitStructure);
	// 引脚2
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_AM;
	GPIO_Init(PORT_MOTOR_GPIOx, &GPIO_InitStructure);
	// 引脚3
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_BP;
	GPIO_Init(PORT_MOTOR_GPIOx, &GPIO_InitStructure);
	// 引脚4
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_BM;
	GPIO_Init(PORT_MOTOR_GPIOx, &GPIO_InitStructure);

}

// 前进一步
void MotorGoOne(void){
	// 控制电机行走
	MotorController();
	++motorPos;
  if (motorPos == 8)
  {
    motorPos = 0;
  }
	MotorStartTime(MotorStop);
}

// 走纸
void MotorGo(uint32_t steps, uint16_t wait){
  while (steps>0)
  {
		steps--;
    MotorController();
    ++motorPos;
    if (motorPos == 8)
    {
      motorPos = 0;
    }
		Delay_us(wait);
  }
	MotorStop();
}



void MotorStop(void){
	// AP线
	GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AP);
	// AM线
	GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AM);
	// BP线
	GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BP);
	// BM线
	GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BM);
}

static void MotorController(void){
	// AP线
	if(motorTable[motorPos][0]){
		GPIO_SetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AP);
	}else{
		GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AP);
	}
	// AM线
	if(motorTable[motorPos][1]){
		GPIO_SetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AM);
	}else{
		GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AM);
	}
	// BP线
	if(motorTable[motorPos][2]){
		GPIO_SetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BP);
	}else{
		GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BP);
	}
	// BM线
	if(motorTable[motorPos][3]){
		GPIO_SetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BM);
	}else{
		GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BM);
	}
}

