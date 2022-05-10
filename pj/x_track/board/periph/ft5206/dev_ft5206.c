/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/08/01
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "dev_ft5206.h"
#include "drv_iic_soft.h"
#include "board.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Private defines ----------------------------------------------------------*/
#define DEV_FT5206_TPEN_RCC    RCC_AHB1Periph_GPIOH
#define DEV_FT5206_TPEN_PORT   GPIOH
#define DEV_FT5206_TPEN_PIN    GPIO_Pin_7

#define DEV_FT5206_TCS_RCC     RCC_AHB1Periph_GPIOI
#define DEV_FT5206_TCS_PORT    GPIOI
#define DEV_FT5206_TCS_PIN     GPIO_Pin_8

#define FT_CMD_WR     0X70  //д����
#define FT_CMD_RD     0X71  //������

/* Private Struct -----------------------------------------------------------*/

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
//Pointer

//Array

//Const

/* Private function prototypes ----------------------------------------------*/
static void Dev_FT5206_RST(uint8_t val);
static uint8_t Dev_FT5206_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len);
static void Dev_FT5206_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len);

/*****************************************************************************
* @brief  ��ʼ��FT5206������
* ex:
* @par
* None
* @retval ����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��
*****************************************************************************/
uint8_t Dev_FT5206_Init(void)
{
  uint8_t temp[2];
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(DEV_FT5206_TPEN_RCC, ENABLE);
  RCC_AHB1PeriphClockCmd(DEV_FT5206_TCS_RCC,  ENABLE);
  
  GPIO_InitStructure.GPIO_Pin   = DEV_FT5206_TPEN_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(DEV_FT5206_TPEN_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin   = DEV_FT5206_TCS_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(DEV_FT5206_TCS_PORT, &GPIO_InitStructure);
  
  Drv_IIC_Soft_Init(0);
  
  Dev_FT5206_RST(0);
//  vTaskDelay(20);
  delay_ms(20);
  Dev_FT5206_RST(1);
  
//  vTaskDelay(50);
  delay_ms(50);
  temp[0] = 0;
  Dev_FT5206_WR_Reg(FT_DEVIDE_MODE,       temp, 1);  //������������ģʽ
  Dev_FT5206_WR_Reg(FT_ID_G_MODE,         temp, 1);  //��ѯģʽ
  temp[0] = 22;
  Dev_FT5206_WR_Reg(FT_ID_G_THGROUP,      temp, 1);  //���ô�����Чֵ
  temp[0] = 12;
  Dev_FT5206_WR_Reg(FT_ID_G_PERIODACTIVE, temp, 1);  //�������ڣ�����С��12�����14
  
  Dev_FT5206_RD_Reg(FT_ID_G_LIB_VERSION, &temp[0], 2);
  
  //�汾:0X3003/0X0001/0X0002
  if((temp[0] == 0X30 && temp[1] == 0X03) || temp[1] == 0X01 || temp[1] == 0X02)
  {
    return 0;
  }
  return 1;
}

/*****************************************************************************
* @brief  
* ex:
* @par
* None
* @retval
*****************************************************************************/
static void Dev_FT5206_RST(uint8_t val)
{
  GPIO_WriteBit(DEV_FT5206_TCS_PORT, DEV_FT5206_TCS_PIN, (BitAction)val);
}

/*****************************************************************************
* @brief  ��FT5206д��һ������
* ex:
* @par
* reg:��ʼ�Ĵ�����ַ
* buf:���ݻ�������
* len:д���ݳ���
* @retval ����ֵ:0,�ɹ�;1,ʧ��.
*****************************************************************************/
static uint8_t Dev_FT5206_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
  uint8_t ret = 0;
  
  Drv_IIC_Soft_Start(0);
  Drv_IIC_Soft_Send_Byte(0, FT_CMD_WR);  //����д����
  Drv_IIC_Soft_Wait_Ack(0);
  Drv_IIC_Soft_Send_Byte(0, reg&0XFF);   //���͵�8λ��ַ
  Drv_IIC_Soft_Wait_Ack(0);  
  for (uint8_t i = 0; i < len; i++)
  {
    Drv_IIC_Soft_Send_Byte(0, buf[i]);   //������
    ret = Drv_IIC_Soft_Wait_Ack(0);
    if(ret)break;  
  }
  Drv_IIC_Soft_Stop(0);  //����һ��ֹͣ����
  
  return ret;
}

/*****************************************************************************
* @brief  
* ex:
* @par
* None
* @retval
*****************************************************************************/
static void Dev_FT5206_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
  Drv_IIC_Soft_Start(0);
  Drv_IIC_Soft_Send_Byte(0, FT_CMD_WR);  //����д����
  Drv_IIC_Soft_Wait_Ack(0);
  Drv_IIC_Soft_Send_Byte(0, reg&0XFF);   //���͵�8λ��ַ
  Drv_IIC_Soft_Wait_Ack(0);
  
  Drv_IIC_Soft_Start(0);
  Drv_IIC_Soft_Send_Byte(0, FT_CMD_RD);  //���Ͷ�����
  Drv_IIC_Soft_Wait_Ack(0);
  for(uint8_t i = 0; i < len; i++)
  {
    buf[i] = Drv_IIC_Soft_Read_Byte(0, i == (len - 1) ? 0 : 1); //������
  }
  Drv_IIC_Soft_Stop(0); //����һ��ֹͣ����
}

/*****************************************************************************
* @brief  ��ѯ�Ƿ��д���
* ex:
* @par
* None
* @retval 1,�д���;0,û�д���
*****************************************************************************/
uint8_t Dev_FT5206_Is_Touch(void)
{
  uint8_t val = 0;
  
  Dev_FT5206_RD_Reg(FT_REG_NUM_FINGER, &val, 1);  //��ȡ�������״̬
  
  if ((val & 0XF) && ((val & 0XF) < 6))
  {
    return 1;
  }
  return 0;
}

/*****************************************************************************
* @brief  ��ȡ����λ��
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Dev_FT5206_Get_XY(uint16_t *x, uint16_t *y)
{
  uint8_t buf[4];
  
  Dev_FT5206_RD_Reg(FT_TP1_REG, buf, 4);
  
  *y = ((uint16_t)(buf[0] & 0X0F) << 8) + buf[1];
  *x = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
}
