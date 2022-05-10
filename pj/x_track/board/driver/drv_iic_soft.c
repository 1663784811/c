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
  iic_io_t  scl;  /* SCL�˿� */
  iic_io_t  sda;  /* SDA�˿� */
} iic_cfg_t;

/* IIC�������� */
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
* @brief  ��SDA���ó�����
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
* @brief  ��SDA���ó����
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
* @brief  ����SCL�������ƽ
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
* @brief  ����SDA�������ƽ
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
* @brief  ��ȡSDA�������ƽ
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
/*                                       �����ķָ���                                       */
/********************************************************************************************/


/*****************************************************************************
* @brief  ��ʼ��
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
  
  /* ����ʱ�ߵ�ƽ */
  Drv_IIC_Soft_SDA_IO(drv, 1);
  Drv_IIC_Soft_SCL_IO(drv, 1);
}

/*****************************************************************************
* @brief  ����IIC��ʼ�ź�
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_IIC_Soft_Start(int drv)
{
  Drv_IIC_Soft_SDA_Out(drv);    //SDA�����
  Drv_IIC_Soft_SDA_IO(drv, 1);
  Drv_IIC_Soft_SCL_IO(drv, 1);
  delay_us(1);
  Drv_IIC_Soft_SDA_IO(drv, 0);  //START: when CLK is high, DATA change form high to low
  delay_us(1);
  Drv_IIC_Soft_SCL_IO(drv, 0);  //ǯסI2C���ߣ�׼�����ͻ�������� 
}

/*****************************************************************************
* @brief  ����IICֹͣ�ź�
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_IIC_Soft_Stop(int drv)
{
  Drv_IIC_Soft_SDA_Out(drv);    //SDA�����
  Drv_IIC_Soft_SCL_IO(drv, 0);
  Drv_IIC_Soft_SDA_IO(drv, 0);  //STOP:when CLK is high DATA change form low to high
  delay_us(1);
  Drv_IIC_Soft_SCL_IO(drv, 1);
  Drv_IIC_Soft_SDA_IO(drv, 1);  //����I2C���߽����ź�
  delay_us(1);
}

/*****************************************************************************
* @brief  �ȴ�Ӧ���źŵ���
* ex:
* @par
* None
* @retval
* 1,����Ӧ��ʧ��
* 0,����Ӧ��ɹ�
*****************************************************************************/
uint8_t Drv_IIC_Soft_Wait_Ack(int drv)
{
  uint8_t ucErrTime=0;

  Drv_IIC_Soft_SDA_In(drv);      //SDA����Ϊ����
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
* @brief  ����ACKӦ��
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
* @brief  ������ACKӦ��
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
* @brief  IIC����һ���ֽ�
* ex:
* @par
* None
* @retval ���شӻ�����Ӧ��
* 1,��Ӧ��
* 0,��Ӧ��
*****************************************************************************/
void Drv_IIC_Soft_Send_Byte(int drv, uint8_t txd)
{
  Drv_IIC_Soft_SDA_Out(drv);
  Drv_IIC_Soft_SCL_IO(drv, 0);        //����ʱ�ӿ�ʼ���ݴ���
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
* @brief  ��1���ֽ�
* ex:
* @par
* ack=1,����ACK; ack=0,����nACK
* @retval
*****************************************************************************/
uint8_t Drv_IIC_Soft_Read_Byte(int drv, uint8_t ack)
{
  uint8_t recv = 0;
  
  Drv_IIC_Soft_SDA_In(drv);      //SDA����Ϊ����
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
    Drv_IIC_Soft_NAck(drv);      //����nACK
  }
  else
  {
    Drv_IIC_Soft_Ack(drv);       //����ACK
  }

  return recv;
}
