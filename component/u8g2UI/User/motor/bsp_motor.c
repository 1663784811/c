#include "bsp_motor.h"
#include "bsp_SysTick.h"

static void MotorController(void);


/**
���������������
	����0: AP
	����1: AM
	����2: BP
	����3: BM
	
	4��λ��Ϊһ����
	
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
  ��ʼ���������ߵ��
*/
void Motor_Config(void){
		
	GPIO_InitTypeDef GPIO_InitStructure;
	// ��APBx ����ʱ��
	MOTOR_GPIO_APBxClkCmd(MOTOR_GPIO_CLK , ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// ����1
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_AP;
	GPIO_Init(PORT_MOTOR_GPIOx, &GPIO_InitStructure);
	// ����2
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_AM;
	GPIO_Init(PORT_MOTOR_GPIOx, &GPIO_InitStructure);
	// ����3
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_BP;
	GPIO_Init(PORT_MOTOR_GPIOx, &GPIO_InitStructure);
	// ����4
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_BM;
	GPIO_Init(PORT_MOTOR_GPIOx, &GPIO_InitStructure);

}

// ǰ��һ��
void MotorGoOne(void){
	// ���Ƶ������
	MotorController();
	++motorPos;
  if (motorPos == 8)
  {
    motorPos = 0;
  }
	MotorStartTime(MotorStop);
}

// ��ֽ
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
	// AP��
	GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AP);
	// AM��
	GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AM);
	// BP��
	GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BP);
	// BM��
	GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BM);
}

static void MotorController(void){
	// AP��
	if(motorTable[motorPos][0]){
		GPIO_SetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AP);
	}else{
		GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AP);
	}
	// AM��
	if(motorTable[motorPos][1]){
		GPIO_SetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AM);
	}else{
		GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_AM);
	}
	// BP��
	if(motorTable[motorPos][2]){
		GPIO_SetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BP);
	}else{
		GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BP);
	}
	// BM��
	if(motorTable[motorPos][3]){
		GPIO_SetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BM);
	}else{
		GPIO_ResetBits(PORT_MOTOR_GPIOx, PIN_MOTOR_BM);
	}
}

