/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/06/28
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "drv_iic_soft.h"
#include "board.h"

/* Private defines ----------------------------------------------------------*/

/* Private Struct -----------------------------------------------------------*/
typedef struct
{
  uint32_t      rcc;
  GPIO_TypeDef *port;
  uint16_t      pin;
} iic_io_t;

typedef struct
{
  iic_io_t  scl;  /* SCL端口 */
  iic_io_t  sda;  /* SDA端口 */
} iic_cfg_t;

/* IIC配置数据 */
const iic_cfg_t iic_cfg[1] =
{
  /* IIC1 */
  {
    {RCC_AHB1Periph_GPIOH, GPIOH, GPIO_Pin_6},
    {RCC_AHB1Periph_GPIOI, GPIOI, GPIO_Pin_3},
  }
};

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
//Pointer

//Array

//Const

/* Private function prototypes ----------------------------------------------*/


/*****************************************************************************
* @brief  将SDA设置成输入
* ex:
* @par
* None
* @retval
*****************************************************************************/
static void Drv_IIC_Soft_SDA_In(int drv)
{
  iic_cfg[0].sda.port->MODER &= ~(3 << (3 * 2));
  iic_cfg[0].sda.port->MODER |=   0 <<  3 * 2;
}

/*****************************************************************************
* @brief  将SDA设置成输出
* ex:
* @par
* None
* @retval
*****************************************************************************/
static void Drv_IIC_Soft_SDA_Out(int drv)
{
  iic_cfg[0].sda.port->MODER &= ~(3 << (3 * 2));
  iic_cfg[0].sda.port->MODER |=   1 <<  3 * 2;
}

/*****************************************************************************
* @brief  设置SCL的输出电平
* ex:
* @par
* None
* @retval
*****************************************************************************/
static void Drv_IIC_Soft_SCL_IO(int drv, uint8_t x)
{
  GPIO_WriteBit(iic_cfg[drv].scl.port, iic_cfg[drv].scl.pin, (BitAction)x);
}

/*****************************************************************************
* @brief  设置SDA的输出电平
* ex:
* @par
* None
* @retval
*****************************************************************************/
static void Drv_IIC_Soft_SDA_IO(int drv, uint8_t x)
{
  GPIO_WriteBit(iic_cfg[drv].sda.port, iic_cfg[drv].sda.pin, (BitAction)x);
}

/*****************************************************************************
* @brief  读取SDA的输入电平
* ex:
* @par
* None
* @retval
*****************************************************************************/
static uint8_t Drv_IIC_Soft_SDA_Read(int drv)
{
  return GPIO_ReadInputDataBit(iic_cfg[drv].sda.port, iic_cfg[drv].sda.pin);
}


/********************************************************************************************/
/*                                       华丽的分割线                                       */
/********************************************************************************************/


/*****************************************************************************
* @brief  初始化
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_IIC_Soft_Init(int drv)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(iic_cfg[drv].scl.rcc, ENABLE);
  RCC_APB2PeriphClockCmd(iic_cfg[drv].sda.rcc, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin   = iic_cfg[drv].scl.pin;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(iic_cfg[drv].scl.port, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = iic_cfg[drv].sda.pin;
  GPIO_Init(iic_cfg[drv].sda.port, &GPIO_InitStructure);
  
  /* 空闲时高电平 */
  Drv_IIC_Soft_SDA_IO(drv, 1);
  Drv_IIC_Soft_SCL_IO(drv, 1);
}

/*****************************************************************************
* @brief  产生IIC起始信号
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_IIC_Soft_Start(int drv)
{
  Drv_IIC_Soft_SDA_Out(drv);    //SDA线输出
  Drv_IIC_Soft_SDA_IO(drv, 1);
  Drv_IIC_Soft_SCL_IO(drv, 1);
  delay_us(1);
  Drv_IIC_Soft_SDA_IO(drv, 0);  //START: when CLK is high, DATA change form high to low
  delay_us(1);
  Drv_IIC_Soft_SCL_IO(drv, 0);  //钳住I2C总线，准备发送或接收数据 
}

/*****************************************************************************
* @brief  产生IIC停止信号
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_IIC_Soft_Stop(int drv)
{
  Drv_IIC_Soft_SDA_Out(drv);    //SDA线输出
  Drv_IIC_Soft_SCL_IO(drv, 0);
  Drv_IIC_Soft_SDA_IO(drv, 0);  //STOP:when CLK is high DATA change form low to high
  delay_us(1);
  Drv_IIC_Soft_SCL_IO(drv, 1);
  Drv_IIC_Soft_SDA_IO(drv, 1);  //发送I2C总线结束信号
  delay_us(1);
}

/*****************************************************************************
* @brief  等待应答信号到来
* ex:
* @par
* None
* @retval
* 1,接收应答失败
* 0,接收应答成功
*****************************************************************************/
uint8_t Drv_IIC_Soft_Wait_Ack(int drv)
{
  uint8_t ucErrTime=0;

  Drv_IIC_Soft_SDA_In(drv);      //SDA设置为输入
  Drv_IIC_Soft_SDA_IO(drv, 1);
  delay_us(1);
  Drv_IIC_Soft_SCL_IO(drv, 1);
  delay_us(1);
  while(Drv_IIC_Soft_SDA_Read(drv))
  {
    ucErrTime++;
    if(ucErrTime > 250)
    {
      Drv_IIC_Soft_Stop(drv);
      return 1;
    }
  }
  Drv_IIC_Soft_SCL_IO(drv, 0);
  return 0;
}

/*****************************************************************************
* @brief  产生ACK应答
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_IIC_Soft_Ack(int drv)
{
  Drv_IIC_Soft_SCL_IO(drv, 0);
  Drv_IIC_Soft_SDA_Out(drv);
  Drv_IIC_Soft_SDA_IO(drv, 0);
  delay_us(1);
  Drv_IIC_Soft_SCL_IO(drv, 1);
  delay_us(1);
  Drv_IIC_Soft_SCL_IO(drv, 0);
}

/*****************************************************************************
* @brief  不产生ACK应答
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_IIC_Soft_NAck(int drv)
{
  Drv_IIC_Soft_SCL_IO(drv, 0);
  Drv_IIC_Soft_SDA_Out(drv);
  Drv_IIC_Soft_SDA_IO(drv, 1);
  delay_us(1);
  Drv_IIC_Soft_SCL_IO(drv, 1);
  delay_us(1);
  Drv_IIC_Soft_SCL_IO(drv, 0);
}

/*****************************************************************************
* @brief  IIC发送一个字节
* ex:
* @par
* None
* @retval 返回从机有无应答
* 1,有应答
* 0,无应答
*****************************************************************************/
void Drv_IIC_Soft_Send_Byte(int drv, uint8_t txd)
{
  Drv_IIC_Soft_SDA_Out(drv);
  Drv_IIC_Soft_SCL_IO(drv, 0);        //拉低时钟开始数据传输
  for(uint8_t t = 0; t < 8; t++)
  {
    txd & 0x80 ? Drv_IIC_Soft_SDA_IO(drv, 1) : Drv_IIC_Soft_SDA_IO(drv, 0);
    txd <<= 1;
    Drv_IIC_Soft_SCL_IO(drv, 1);
    delay_us(1);
    Drv_IIC_Soft_SCL_IO(drv, 0);
    delay_us(1);
  }
}

/*****************************************************************************
* @brief  读1个字节
* ex:
* @par
* ack=1,发送ACK; ack=0,发送nACK
* @retval
*****************************************************************************/
uint8_t Drv_IIC_Soft_Read_Byte(int drv, uint8_t ack)
{
  uint8_t recv = 0;
  
  Drv_IIC_Soft_SDA_In(drv);      //SDA设置为输入
  for(uint8_t i = 0; i < 8; i++ )
  {
    Drv_IIC_Soft_SCL_IO(drv, 0);
    delay_us(1);
    Drv_IIC_Soft_SCL_IO(drv, 1);
    recv <<= 1;
    if(Drv_IIC_Soft_SDA_Read(drv))
    {
      recv++;
    }
    delay_us(1);
  }
  if (!ack)
  {
    Drv_IIC_Soft_NAck(drv);      //发送nACK
  }
  else
  {
    Drv_IIC_Soft_Ack(drv);       //发送ACK
  }

  return recv;
}
