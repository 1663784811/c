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

//LCD的画笔颜色和背景色
static uint16_t POINT_COLOR = 0X0000; //画笔颜色
static uint16_t BACK_COLOR  = 0XFFFF; //背景色

//Pointer

//LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域
uint16_t *ltdc_framebuf[2];

//Array

//定义最大屏分辨率时,LCD所需的帧缓存数组大小
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

  delay_ms(1);  /* 很重要 */

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
  ltdc.Width   = 1024;   //面板宽度,单位:像素
  ltdc.Height  = 600;    //面板高度,单位:像素
  ltdc.HSync_W = 20;     //水平同步宽度
  ltdc.VSync_W = 3;      //垂直同步宽度
  ltdc.HBP     = 140;    //水平后廊
  ltdc.VBP     = 20;     //垂直后廊
  ltdc.HFP     = 160;    //水平前廊
  ltdc.VFP     = 12;     //垂直前廊

  /* 配置信号极性 */
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;   /* HSYNC 低电平有效 */
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;   /* VSYNC 低电平有效 */
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;   /* DE 低电平有效 */
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

  /* 配置LTDC的同步时序 */
  LTDC_InitStruct.LTDC_HorizontalSync     = ltdc.HSync_W - 1;
  LTDC_InitStruct.LTDC_VerticalSync       = ltdc.VSync_W - 1;
  LTDC_InitStruct.LTDC_AccumulatedHBP     = ltdc.HSync_W + ltdc.HBP - 1;
  LTDC_InitStruct.LTDC_AccumulatedVBP     = ltdc.VSync_W + ltdc.VBP - 1;
  LTDC_InitStruct.LTDC_AccumulatedActiveW = ltdc.HSync_W + ltdc.HBP + ltdc.Width - 1;
  LTDC_InitStruct.LTDC_AccumulatedActiveH = ltdc.VSync_W + ltdc.VBP + ltdc.Height - 1;
  LTDC_InitStruct.LTDC_TotalWidth         = ltdc.HSync_W + ltdc.HBP + ltdc.Width  + ltdc.HFP - 1;
  LTDC_InitStruct.LTDC_TotalHeigh         = ltdc.VSync_W + ltdc.VBP + ltdc.Height + ltdc.VFP - 1;

  /* 背景色 */
  LTDC_InitStruct.LTDC_BackgroundRedValue   = 0;
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
  LTDC_InitStruct.LTDC_BackgroundBlueValue  = 0;

  LTDC_Init(&LTDC_InitStruct);

  //LTDC层配置
  LTDC_Layer_InitStruct.LTDC_HorizontalStart   = ltdc.HSync_W + ltdc.HBP; //水平起始位置
  LTDC_Layer_InitStruct.LTDC_HorizontalStop    = ltdc.HSync_W + ltdc.HBP + ltdc.Width - 1;
  LTDC_Layer_InitStruct.LTDC_VerticalStart     = ltdc.VSync_W + ltdc.VBP; //垂直起始位置
  LTDC_Layer_InitStruct.LTDC_VerticalStop      = ltdc.VSync_W + ltdc.VBP + ltdc.Height - 1;

  LTDC_Layer_InitStruct.LTDC_PixelFormat       = LTDC_Pixelformat_RGB565; //像素格式
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha     = 255;                     //恒定alpha

  /* Default Color configuration (configure A,R,G,B component values) */
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue  = 0;  //默认蓝色
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;  //默认绿色
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed   = 0;  //默认红色
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;  //默认alpha

  /* Configure blending factors */
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1  = LTDC_BlendingFactor1_PAxCA; //混合系数
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2  = LTDC_BlendingFactor2_PAxCA; //混合系数

  /* Start Address configuration : the LCD Frame buffer is defined on SDRAM */
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress    = LCD429_FRAME_BUFFER;   //颜色帧缓冲区地址

  LTDC_Layer_InitStruct.LTDC_CFBLineLength     = ((ltdc.Width * 2) + 3);//颜色帧缓冲区行长
  LTDC_Layer_InitStruct.LTDC_CFBPitch          =  (ltdc.Width * 2);     //颜色帧缓冲区间距
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber     =   ltdc.Height;         //行数

  //配置层1
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

  //	/* Configure blending factors */
  //	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1  = LTDC_BlendingFactor1_PAxCA;  //混合系数
  //	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2  = LTDC_BlendingFactor1_PAxCA;  //混合系数
  //	
  //	/* Start Address configuration : the LCD Frame buffer is defined on SDRAM */
  //	LTDC_Layer_InitStruct.LTDC_CFBStartAdress    = LCD429_FRAME_BUFFER + BUFFER_OFFSET;
  //	
  //	//配置层2
  //	LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);

  LTDC_LayerCmd(LTDC_Layer1, ENABLE);
  //	LTDC_LayerCmd(LTDC_Layer2, ENABLE);

  LTDC_ReloadConfig(LTDC_IMReload);//影子寄存器立即重载

  LTDC_Cmd(ENABLE);

  ltdc_framebuf[0] = (uint16_t *)&ltdc_lcd_framebuf_1;

  //设置为横屏
  Drv_LTDC_Display_Dir_Set(1);

  //清屏
  Drv_LTDC_Clear(WHITE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////


/*****************************************************************************
* @brief  画点函数
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_LTDC_Draw_Point(uint16_t x, uint16_t y, uint16_t color)
{
  if (ltdc.Dir) {
    *(uint16_t *)((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (LCD_WIDTH * y + x)) = color;                    //横屏
  } else {
    *(uint16_t *)((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (LCD_WIDTH * (LCD_HEIGHT - x - 1) + y)) = color; //竖屏
  }
}

/*****************************************************************************
* @brief  读点函数
* ex:
* @par
* None
* @retval
*****************************************************************************/
uint16_t Drv_LTDC_Read_Point(uint16_t x, uint16_t y)
{
  if (ltdc.Dir) {
    return *(uint16_t *)((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (LCD_WIDTH * y + x));                    //横屏
  } else {
    return *(uint16_t *)((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (LCD_WIDTH * (LCD_HEIGHT - x - 1) + y)); //竖屏
  }
}

/*****************************************************************************
* @brief  LTDC填充矩形
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_LTDC_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
//  //横屏
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
  
  uint32_t psx, psy, pex, pey; //以LCD面板为基准的坐标系,不随横竖屏变化而变化
  uint32_t timeout=0; 
  uint16_t offline;
  uint32_t addr;
  
  //坐标系转换
  if(ltdc.Dir)  //横屏
  {
    psx = sx; psy = sy;
    pex = ex; pey = ey;
  }
  else     //竖屏
  {
    psx = sy; psy = ltdc.Height - ex - 1;
    pex = ey; pey = ltdc.Height - sx - 1;
  }
  offline = ltdc.Width - (pex - psx + 1);
  addr = ((uint32_t)ltdc_framebuf[0] + ltdc.Pixsize * (ltdc.Width * psy + psx));
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); //使能DM2D时钟
  
  DMA2D->CR    &= ~(DMA2D_CR_START);  //先停止DMA2D
  DMA2D->CR     = DMA2D_R2M;          //寄存器到存储器模式
  DMA2D->OPFCCR = DMA2D_RGB565;       //设置颜色格式
  DMA2D->OOR    = offline;            //设置行偏移 

  DMA2D->OMAR   = addr;               //输出存储器地址
  DMA2D->NLR    = (pey - psy + 1) | ((pex - psx + 1) << 16); //设定行数寄存器
  DMA2D->OCOLR  = color;              //设定输出颜色寄存器 
  DMA2D->CR    |= DMA2D_CR_START;     //启动DMA2D
  
  while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)  //等待传输完成
  {
    timeout++;
    if (timeout > 0X1FFFFF) break;    //超时退出
  } 
  DMA2D->IFCR|=DMA2D_FLAG_TC;         //清除传输完成标志 
}

/*****************************************************************************
* @brief  LTDC填充颜色矩形
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
  
  /* 坐标转换 */
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
  
  DMA2D->CR     &= ~(DMA2D_CR_START); //先停止DMA2D
  DMA2D->CR      = DMA2D_M2M;         //存储器到存储器模式
  DMA2D->FGPFCCR = DMA2D_RGB565;      //设置颜色格式
  DMA2D->FGOR    = 0;                 //前景层行偏移为0
  DMA2D->OOR     = offline;           //设置行偏移
  
  DMA2D->FGMAR   = (uint32_t)color;   //源地址
  DMA2D->OMAR    = addr;              //输出存储器地址
  DMA2D->NLR     = (pey - psy + 1) | ((pex - psx + 1) << 16); //设定行数寄存器 
  DMA2D->CR     |= DMA2D_CR_START;    //启动DMA2D
  
  while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)
  {
    timeout++;
    if (timeout > 0X1FFFFF) break;    //超时退出
  }
  DMA2D->IFCR |= DMA2D_FLAG_TC;       //清除传输完成标志
}

/*****************************************************************************
* @brief  LCD清屏
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
* @brief  在指定位置显示1个字符
* ex:
* @par
* num： 要显示的字符
* size：字体大小 12/16/24
* mode：1，叠加方式；0，非叠加方式
* @retval
*****************************************************************************/
void Drv_LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode)
{
  uint8_t temp;
  uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //不同字体一个字符的点阵集所占的字节数
  uint16_t y0 = y; //保存起始y坐标

  num -= ' ';//得到偏移后的值

  for (uint8_t t = 0; t < csize; t++) {
    switch (size) {
      case 12: temp = asc2_1206[num][t]; break; //1206字体
      case 16: temp = asc2_1608[num][t]; break; //1608字体
      case 24: temp = asc2_2412[num][t]; break; //2412字体
      case 32: temp = asc2_3216[num][t]; break; //3216字体
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
      if(y >= 600) return;    //超出范围
      if ((y-y0) == size) {
        y = y0;
        x++;
        if(x >= 1024) return; //超出范围
        break;  //字体的y轴到底,另起1列
      }
    }
  }
}

/*****************************************************************************
* @brief  在指定位置显示字符串
* ex:
* @par
* size：字体大小 12/16/24
* *p：  字符串起始地址
* @retval
*****************************************************************************/
void Drv_LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p)
{
  uint8_t x0 = x;

  width  += x;
  height += y;

  //判断是否是非法字符
  while((*p <= '~') && (*p >= ' ')) {
    //是否另起一行
    if(x >= width) {
      x = x0;
      y += size;
    }
    
    //是否超出给定区域
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
* @brief  LCD显示方向
* ex:
* @par
* dir:1,横屏;0,竖屏
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
* @brief  设置画笔颜色
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
* @brief  获取ltdc成员数据
* ex:
* @par
* None
* @retval
*****************************************************************************/
ltdc_t Drv_LTDC_Get(void)
{
  return ltdc;
}
