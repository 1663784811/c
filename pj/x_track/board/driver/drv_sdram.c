/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/31
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "drv_sdram.h"
#include "drv_dbg.h"
#include "board.h"

/* Private defines ----------------------------------------------------------*/

/* SDRAM超时 */
#define SDRAM_TIMEOUT  ((uint32_t)0x0000FFFF)

/* FMC SDRAM Mode definition register defines */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_1              ((uint16_t)0x0010)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

/* Private Struct -----------------------------------------------------------*/

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
//Pointer

//Array

//Const

/* Private function prototypes ----------------------------------------------*/


/*****************************************************************************
* @brief  执行SDRAM初始化序列
* ex:
* @par
* None
* @retval
*****************************************************************************/
static void SDRAM_InitSequence(void)
{
  FMC_SDRAMCommandTypeDef  FMC_SDRAMCommandStructure;
  uint32_t tmpr    = 0;
  uint32_t timeout = SDRAM_TIMEOUT;

  /* Configure a clock configuration enable command */
  FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_CLK_Enabled; //命令
  FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_Command_Target_bank1;     //目标SDRAM存储区域
  FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 1;                            //自刷新次数
  FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;                            //要写入模式寄存器的值
  /* Wait until the SDRAM controller is ready */
  while((FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
  {
    timeout--;
  }
  /* Send the command */
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

  /* Insert 1 ms delay */
  delay_ms(1);

  /* Configure a PALL (precharge all) command */
  FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_PALL;
  FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_Command_Target_bank1;
  FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 1;
  FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
  /* Wait until the SDRAM controller is ready */
  timeout = SDRAM_TIMEOUT;
  while((FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
  {
    timeout--;
  }
  /* Send the command */
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

  /* Configure a Auto-Refresh command */
  FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_AutoRefresh;
  FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_Command_Target_bank1;
  FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 8;
  FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
  /* Wait until the SDRAM controller is ready */
  timeout = SDRAM_TIMEOUT;
  while((FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
  {
    timeout--;
  }
  /* Send the command */
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

  /* Program the external memory mode register */
  tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          | //设置突发长度(可以是1/2/4/8)
                   SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   | //设置突发类型(可以是连续/交错)
                   SDRAM_MODEREG_CAS_LATENCY_3           | //设置CAS值:3(可以是2/3)
                   SDRAM_MODEREG_OPERATING_MODE_STANDARD | //设置操作模式:0,标准模式
                   SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;   //设置突发写模式:1,单点访问

  /* Configure a load Mode register command*/
  FMC_SDRAMCommandStructure.FMC_CommandMode            = FMC_Command_Mode_LoadMode;
  FMC_SDRAMCommandStructure.FMC_CommandTarget          = FMC_Command_Target_bank1;
  FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber      = 1;
  FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = tmpr;

  /* Wait until the SDRAM controller is ready */
  timeout = SDRAM_TIMEOUT;
  while((FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
  {
    timeout--;
  }
  /* Send the command */
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

  FMC_SetRefreshCount(683);
  /* Wait until the SDRAM controller is ready */
  timeout = SDRAM_TIMEOUT;
  while((FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
  {
    timeout--;
  }
}

/*****************************************************************************
* @brief  sdram初始化
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_SDRAM_Init(void)
{
  GPIO_InitTypeDef            GPIO_InitStructure;
  FMC_SDRAMInitTypeDef        FMC_SDRAMInitStructure;
  FMC_SDRAMTimingInitTypeDef  FMC_SDRAMTimingInitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);
  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource0,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource2,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3,  GPIO_AF_FMC);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FMC);

  GPIO_PinAFConfig(GPIOF, GPIO_PinSource0,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource1,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource2,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource3,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource4,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource5,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource11, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FMC);

  GPIO_PinAFConfig(GPIOG, GPIO_PinSource0,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource1,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource2,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource4,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource5,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource8,  GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource15, GPIO_AF_FMC);
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_11|GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_11|GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay    = 2; //加载模式寄存器到激活时间的延迟为2个时钟周期
  FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 8; //退出自刷新延迟为8个时钟周期
  FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime      = 6; //自刷新时间为6个时钟周期
  FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay        = 6; //行循环延迟为6个时钟周期
  FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime    = 4; //恢复延迟为2个时钟周期(这里在cubemx上设置不了2)
  FMC_SDRAMTimingInitStructure.FMC_RPDelay              = 2; //行预充电延迟为2个时钟周期
  FMC_SDRAMTimingInitStructure.FMC_RCDDelay             = 2; //行到列延迟为2个时钟周期

  /* FMC SDRAM control configuration */
  FMC_SDRAMInitStructure.FMC_Bank               = FMC_Bank1_SDRAM;
  FMC_SDRAMInitStructure.FMC_ColumnBitsNumber   = FMC_ColumnBits_Number_9b;
  FMC_SDRAMInitStructure.FMC_RowBitsNumber      = FMC_RowBits_Number_13b;
  FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth  = FMC_SDMemory_Width_16b;
  FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4;
  FMC_SDRAMInitStructure.FMC_CASLatency         = FMC_CAS_Latency_3;
  FMC_SDRAMInitStructure.FMC_WriteProtection    = FMC_Write_Protection_Disable;
  FMC_SDRAMInitStructure.FMC_SDClockPeriod      = FMC_SDClock_Period_2;
  FMC_SDRAMInitStructure.FMC_ReadBurst          = FMC_Read_Burst_Enable;
  FMC_SDRAMInitStructure.FMC_ReadPipeDelay      = FMC_ReadPipe_Delay_1;
  FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct  = &FMC_SDRAMTimingInitStructure;

  /* FMC SDRAM bank initialization */
  FMC_SDRAMInit(&FMC_SDRAMInitStructure);

  /* FMC SDRAM device initialization sequence */
  SDRAM_InitSequence();
}

/*****************************************************************************
* @brief  SDRAM内存测试
* ex:
* @par
* None
* @retval
*****************************************************************************/
void fsmc_sdram_test(void)
{
  uint32_t size = 0;

  //每隔32K字节,写入一个数据,总共写入1024个数据,刚好是32M字节
  for (uint32_t i = 0, temp = 0; i < 32*1024*1024; i += 32*1024, temp++) {
    *(__IO uint32_t *)(Bank5_SDRAM_ADDR + i) = temp;
  }

  //依次读出之前写入的数据,进行校验
  for (uint32_t i = 0, temp = 0; i < 32*1024*1024; i += 32*1024, temp++) {
    if (temp == *(__IO uint32_t *)(Bank5_SDRAM_ADDR + i)) {
      size = (temp + 1) * 32;
    } else {
      break;
    }
  }

  dbg_printf("SDRAM Capacity:%dKB\r\n\r\n", size); //打印SDRAM容量
}
