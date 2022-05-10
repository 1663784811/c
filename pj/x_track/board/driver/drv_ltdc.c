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
#include "drv_ltdc.h"
#include "board.h"
#include "font.h"

/* Private defines ----------------------------------------------------------*/

/* Private Struct -----------------------------------------------------------*/
ltdc_t ltdc;

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/

//LCD�Ļ�����ɫ�ͱ���ɫ
static uint16_t POINT_COLOR = 0X0000; //������ɫ
static uint16_t BACK_COLOR  = 0XFFFF; //����ɫ

//Pointer

//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����
uint16_t *ltdc_framebuf[2];

//Array

//����������ֱ���ʱ,LCD�����֡���������С
uint16_t ltdc_lcd_framebuf_1[1024][600] __attribute__((at(LCD429_FRAME_BUFFER)));
//uint16_t ltdc_lcd_framebuf_2[1024][600] __attribute__((at(LCD429_FRAME_BUFFER + BUFFER_OFFSET)));

//Const

/* Private function prototypes ----------------------------------------------*/


/*****************************************************************************
* @brief  
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_LTDC_Init(void)
{
  GPIO_InitTypeDef       GPIO_InitStructure;
  LTDC_InitTypeDef       LTDC_InitStruct;
  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;

  RCC_PLLSAIConfig(320, 4, 2);
  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div4);
  RCC_PLLSAICmd(ENABLE);

  delay_ms(1);  /* ����Ҫ */

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

  GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);

  GPIO_PinAFConfig(GPIOG, GPIO_PinSource6,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource7,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);

  GPIO_PinAFConfig(GPIOH, GPIO_PinSource9,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource12, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource13, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource15, GPIO_AF_LTDC);

  GPIO_PinAFConfig(GPIOI, GPIO_PinSource0,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource1,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource2,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource4,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource5,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource6,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource7,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource9,  GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_LTDC);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOH, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOI, &GPIO_InitStructure);

  ltdc.Pixsize = 2;
  ltdc.Width   = 1024;   //�����,��λ:����
  ltdc.Height  = 600;    //���߶�,��λ:����
  ltdc.HSync_W = 20;     //ˮƽͬ�����
  ltdc.VSync_W = 3;      //��ֱͬ�����
  ltdc.HBP     = 140;    //ˮƽ����
  ltdc.VBP     = 20;     //��ֱ����
  ltdc.HFP     = 160;    //ˮƽǰ��
  ltdc.VFP     = 12;     //��ֱǰ��

  /* �����źż��� */
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;   /* HSYNC �͵�ƽ��Ч */
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;   /* VSYNC �͵�ƽ��Ч */
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;   /* DE �͵�ƽ��Ч */
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

  /* ����LTDC��ͬ��ʱ�� */
  LTDC_InitStruct.LTDC_HorizontalSync     = ltdc.HSync_W - 1;
  LTDC_InitStruct.LTDC_VerticalSync       = ltdc.VSync_W - 1;
  LTDC_InitStruct.LTDC_AccumulatedHBP     = ltdc.HSync_W + ltdc.HBP - 1;
  LTDC_InitStruct.LTDC_AccumulatedVBP     = ltdc.VSync_W + ltdc.VBP - 1;
  LTDC_InitStruct.LTDC_AccumulatedActiveW = ltdc.HSync_W + ltdc.HBP + ltdc.Width - 1;
  LTDC_InitStruct.LTDC_AccumulatedActiveH = ltdc.VSync_W + ltdc.VBP + ltdc.Height - 1;
  LTDC_InitStruct.LTDC_TotalWidth         = ltdc.HSync_W + ltdc.HBP + ltdc.Width  + ltdc.HFP - 1;
  LTDC_InitStruct.LTDC_TotalHeigh         = ltdc.VSync_W + ltdc.VBP + ltdc.Height + ltdc.VFP - 1;

  /* ����ɫ */
  LTDC_InitStruct.LTDC_BackgroundRedValue   = 0;
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
  LTDC_InitStruct.LTDC_BackgroundBlueValue  = 0;

  LTDC_Init(&LTDC_InitStruct);

  //LTDC������
  LTDC_Layer_InitStruct.LTDC_HorizontalStart   = ltdc.HSync_W + ltdc.HBP; //ˮƽ��ʼλ��
  LTDC_Layer_InitStruct.LTDC_HorizontalStop    = ltdc.HSync_W + ltdc.HBP + ltdc.Width - 1;
  LTDC_Layer_InitStruct.LTDC_VerticalStart     = ltdc.VSync_W + ltdc.VBP; //��ֱ��ʼλ��
  LTDC_Layer_InitStruct.LTDC_VerticalStop      = ltdc.VSync_W + ltdc.VBP + ltdc.Height - 1;

  LTDC_Layer_InitStruct.LTDC_PixelFormat       = LTDC_Pixelformat_RGB565; //���ظ�ʽ
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha     = 255;                     //�㶨alpha

  /* Default Color configuration (configure A,R,G,B component values) */
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue  = 0;  //Ĭ����ɫ
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;  //Ĭ����ɫ
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed   = 0;  //Ĭ�Ϻ�ɫ
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;  //Ĭ��alpha

  /* Configure blending factors */
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1  = LTDC_BlendingFactor1_PAxCA; //���ϵ��
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2  = LTDC_BlendingFactor2_PAxCA; //���ϵ��

  /* Start Address configuration : the LCD Frame buffer is defined on SDRAM */
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress    = LCD429_FRAME_BUFFER;   //��ɫ֡��������ַ

  LTDC_Layer_InitStruct.LTDC_CFBLineLength     = ((ltdc.Width * 2) + 3);//��ɫ֡�������г�
  LTDC_Layer_InitStruct.LTDC_CFBPitch          =  (ltdc.Width * 2);     //��ɫ֡���������
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber     =   ltdc.Height;         //����

  //���ò�1
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

  //	/* Configure blending factors */
  //	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1  = LTDC_BlendingFactor1_PAxCA;  //���ϵ��
  //	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2  = LTDC_BlendingFactor1_PAxCA;  //���ϵ��
  //	
  //	/* Start Address configuration : the LCD Frame buffer is defined on SDRAM */
  //	LTDC_Layer_InitStruct.LTDC_CFBStartAdress    = LCD429_FRAME_BUFFER + BUFFER_OFFSET;
  //	
  //	//���ò�2
  //	LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);

  LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  //	LTDC_LayerCmd(LTDC_Layer2, ENABLE);

  LTDC_ReloadConfig(LTDC_IMReload);//Ӱ�ӼĴ�����������

  LTDC_Cmd(ENABLE);

  ltdc_framebuf[0] = (uint16_t *)&ltdc_lcd_framebuf_1;

  //����Ϊ����
  Drv_LTDC_Display_Dir_Set(1);

  //����
  Drv_LTDC_Clear(WHITE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////


/*****************************************************************************
* @brief  ���㺯��
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_LTDC_Draw_Point(uint16_t x, uint16_t y, uint16_t color)
{
  if (ltdc.Dir) {
    *(uint16_t *)((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (LCD_WIDTH * y + x)) = color;                    //����
  } else {
    *(uint16_t *)((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (LCD_WIDTH * (LCD_HEIGHT - x - 1) + y)) = color; //����
  }
}

/*****************************************************************************
* @brief  ���㺯��
* ex:
* @par
* None
* @retval
*****************************************************************************/
uint16_t Drv_LTDC_Read_Point(uint16_t x, uint16_t y)
{
  if (ltdc.Dir) {
    return *(uint16_t *)((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (LCD_WIDTH * y + x));                    //����
  } else {
    return *(uint16_t *)((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (LCD_WIDTH * (LCD_HEIGHT - x - 1) + y)); //����
  }
}

/*****************************************************************************
* @brief  LTDC������
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_LTDC_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
//  //����
//  if(ltdc.Dir) {
//    for (uint16_t y = sy; y < ey + 1; y++) {
//      for (uint16_t x = sx; x< ex + 1; x++) {
//        *(uint16_t *)((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (LCD_WIDTH * y + x)) = *color;
//        color++;
//      }
//    }
//  } else {
//    for(uint16_t y = sy; y < ey + 1; y++) {
//      for (uint16_t x = sx; x < ex + 1; x++) {
//        *(uint16_t *)((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (LCD_WIDTH * (LCD_HEIGHT - x - 1) + y)) = *color;
//        color++;
//      }
//    }
//  }
  
  uint32_t psx, psy, pex, pey; //��LCD���Ϊ��׼������ϵ,����������仯���仯
  uint32_t timeout=0; 
  uint16_t offline;
  uint32_t addr;
  
  //����ϵת��
  if(ltdc.Dir)  //����
  {
    psx = sx; psy = sy;
    pex = ex; pey = ey;
  }
  else     //����
  {
    psx = sy; psy = ltdc.Height - ex - 1;
    pex = ey; pey = ltdc.Height - sx - 1;
  }
  offline = ltdc.Width - (pex - psx + 1);
  addr = ((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (ltdc.Width * psy + psx));
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); //ʹ��DM2Dʱ��
  
  DMA2D->CR    &= ~(DMA2D_CR_START);  //��ֹͣDMA2D
  DMA2D->CR     = DMA2D_R2M;          //�Ĵ������洢��ģʽ
  DMA2D->OPFCCR = DMA2D_RGB565;       //������ɫ��ʽ
  DMA2D->OOR    = offline;            //������ƫ�� 

  DMA2D->OMAR   = addr;               //����洢����ַ
  DMA2D->NLR    = (pey - psy + 1) | ((pex - psx + 1) << 16); //�趨�����Ĵ���
  DMA2D->OCOLR  = color;              //�趨�����ɫ�Ĵ��� 
  DMA2D->CR    |= DMA2D_CR_START;     //����DMA2D
  
  while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)  //�ȴ��������
  {
    timeout++;
    if (timeout > 0X1FFFFF) break;    //��ʱ�˳�
  } 
  DMA2D->IFCR|=DMA2D_FLAG_TC;         //���������ɱ�־ 
}

/*****************************************************************************
* @brief  LTDC�����ɫ����
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_LTDC_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
  uint32_t psx, psy, pex, pey;
  uint32_t timeout = 0;
  uint16_t offline;
  uint32_t addr;
  
  /* ����ת�� */
  if (ltdc.Dir)
  {
    psx = sx; psy = sy;
    pex = ex; pey = ey;
  }
  else
  {
    psx = sy; psy = ltdc.Height - ex - 1;
    pex = ey; pey = ltdc.Height - sx - 1;
  }
  offline = ltdc.Width - (pex - psx + 1);
  
  addr = ((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (ltdc.Width * psy + psx));
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);
  
  DMA2D->CR     &= ~(DMA2D_CR_START); //��ֹͣDMA2D
  DMA2D->CR      = DMA2D_M2M;         //�洢�����洢��ģʽ
  DMA2D->FGPFCCR = DMA2D_RGB565;      //������ɫ��ʽ
  DMA2D->FGOR    = 0;                 //ǰ������ƫ��Ϊ0
  DMA2D->OOR     = offline;           //������ƫ��
  
  DMA2D->FGMAR   = (uint32_t)color;   //Դ��ַ
  DMA2D->OMAR    = addr;              //����洢����ַ
  DMA2D->NLR     = (pey - psy + 1) | ((pex - psx + 1) << 16); //�趨�����Ĵ��� 
  DMA2D->CR     |= DMA2D_CR_START;    //����DMA2D
  
  while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)
  {
    timeout++;
    if (timeout > 0X1FFFFF) break;    //��ʱ�˳�
  }
  DMA2D->IFCR |= DMA2D_FLAG_TC;       //���������ɱ�־
}

/*****************************************************************************
* @brief  LCD����
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_LTDC_Clear(uint16_t color)
{
  Drv_LTDC_Fill(0, 0, ltdc.Width - 1, ltdc.Height - 1, color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************
* @brief  ��ָ��λ����ʾ1���ַ�
* ex:
* @par
* num�� Ҫ��ʾ���ַ�
* size�������С 12/16/24
* mode��1�����ӷ�ʽ��0���ǵ��ӷ�ʽ
* @retval
*****************************************************************************/
void Drv_LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode)
{
  uint8_t temp;
  uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //��ͬ����һ���ַ��ĵ�����ռ���ֽ���
  uint16_t y0 = y; //������ʼy����

  num -= ' ';//�õ�ƫ�ƺ��ֵ

  for (uint8_t t = 0; t < csize; t++) {
    switch (size) {
      case 12: temp = asc2_1206[num][t]; break; //1206����
      case 16: temp = asc2_1608[num][t]; break; //1608����
      case 24: temp = asc2_2412[num][t]; break; //2412����
      case 32: temp = asc2_3216[num][t]; break; //3216����
      default: return;
    }
    
    for (uint8_t t1 = 0; t1 < 8; t1++) {
      if(temp & 0x80) {
        Drv_LTDC_Draw_Point(x, y, POINT_COLOR);
      } else if (mode == 0) {
        Drv_LTDC_Draw_Point(x, y, BACK_COLOR);
      }
      
      temp <<= 1;
      y++;
      if(y >= 600) return;    //������Χ
      if ((y-y0) == size) {
        y = y0;
        x++;
        if(x >= 1024) return; //������Χ
        break;  //�����y�ᵽ��,����1��
      }
    }
  }
}

/*****************************************************************************
* @brief  ��ָ��λ����ʾ�ַ���
* ex:
* @par
* size�������С 12/16/24
* *p��  �ַ�����ʼ��ַ
* @retval
*****************************************************************************/
void Drv_LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p)
{
  uint8_t x0 = x;

  width  += x;
  height += y;

  //�ж��Ƿ��ǷǷ��ַ�
  while((*p <= '~') && (*p >= ' ')) {
    //�Ƿ�����һ��
    if(x >= width) {
      x = x0;
      y += size;
    }
    
    //�Ƿ񳬳���������
    if(y >= height) break;
    
    Drv_LCD_ShowChar(x, y, *p, size, 0);
    
    x += size/2;
    p++;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************
* @brief  LCD��ʾ����
* ex:
* @par
* dir:1,����;0,����
* @retval
*****************************************************************************/
void Drv_LTDC_Display_Dir_Set(uint8_t dir)
{
  ltdc.Dir = dir;

  if (dir == 0) {
    ltdc.Width  = LCD_HEIGHT;
    ltdc.Height = LCD_WIDTH;
  } else {
    ltdc.Width  = LCD_WIDTH;
    ltdc.Height = LCD_HEIGHT;
  }
}

/*****************************************************************************
* @brief  ���û�����ɫ
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_LTDC_PointColor_Set(uint16_t color)
{
  POINT_COLOR = color;
}

/*****************************************************************************
* @brief  ��ȡltdc��Ա����
* ex:
* @par
* None
* @retval
*****************************************************************************/
ltdc_t Drv_LTDC_Get(void)
{
  return ltdc;
}
