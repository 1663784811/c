#include "bsp_SysTick.h"


// 保存时钟次数
static __IO u32 TimingDelay;
// 防止烧电机( 1秒不走一步则关闭电机 )
static __IO u32 MotorTime;
// 停止电机函数指针
static __IO MotorFun MotorStopFn;

// 初始化系统时钟
void SysTick_Init(void){
	
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	SysTick_Config(SystemCoreClock / 1000000);
}

// 延时us
void Delay_us(__IO u32 nsTime)
{
	TimingDelay = nsTime;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	while(TimingDelay != 0);
}

// 延时ms
void Delay_ms(__IO u32 msTime)
{
	while (msTime > 0)
	{
		Delay_us(1000);
		msTime--;
	}
}

// 延时s
void Delay_s(__IO u32 sTime){
	while (sTime > 0)
	{
		Delay_ms(1000);
		sTime--;
	}
}

/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay > 0)
	{
		TimingDelay--;
	}
	if(MotorTime>0){
		MotorTime--;
		// TODO 会拖慢系统时钟
		if(MotorTime <= 0){
			MotorStopFn();
		}
	}
	if(TimingDelay<=0 && MotorTime <= 0){
		SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
	}
}

// ============================================================   电机 防烧处理
// 电机开始启动计时
void MotorStartTime(MotorFun motorFun){
	MotorTime = 1 * 1000 * 1000;
	MotorStopFn = motorFun;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
}

