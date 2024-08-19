#include "bsp_SysTick.h"


// ����ʱ�Ӵ���
static __IO u32 TimingDelay;
// ��ֹ�յ��( 1�벻��һ����رյ�� )
static __IO u32 MotorTime;
// ֹͣ�������ָ��
static __IO MotorFun MotorStopFn;

// ��ʼ��ϵͳʱ��
void SysTick_Init(void){
	
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	SysTick_Config(SystemCoreClock / 1000000);
}

// ��ʱus
void Delay_us(__IO u32 nsTime)
{
	TimingDelay = nsTime;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	while(TimingDelay != 0);
}

// ��ʱms
void Delay_ms(__IO u32 msTime)
{
	while (msTime > 0)
	{
		Delay_us(1000);
		msTime--;
	}
}

// ��ʱs
void Delay_s(__IO u32 sTime){
	while (sTime > 0)
	{
		Delay_ms(1000);
		sTime--;
	}
}

/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay > 0)
	{
		TimingDelay--;
	}
	if(MotorTime>0){
		MotorTime--;
		// TODO ������ϵͳʱ��
		if(MotorTime <= 0){
			MotorStopFn();
		}
	}
	if(TimingDelay<=0 && MotorTime <= 0){
		SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
	}
}

// ============================================================   ��� ���մ���
// �����ʼ������ʱ
void MotorStartTime(MotorFun motorFun){
	MotorTime = 1 * 1000 * 1000;
	MotorStopFn = motorFun;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}

