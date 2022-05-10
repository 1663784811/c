/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/06/28
  *****************************************************************************
  */
#ifndef __DRV_LTDC_H
#define __DRV_LTDC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "drv_sdram.h"

/* Public defines -----------------------------------------------------------*/
#define LCD_WIDTH   1024  //LCD面板的宽度,固定参数,不随显示方向改变
#define LCD_HEIGHT  600   //LCD面板的高度,固定参数,不随显示方向改变

//层1起始地址
#define LCD429_FRAME_BUFFER  Bank5_SDRAM_ADDR

//画笔颜色
#define WHITE    0xFFFF
#define BLACK    0x0000
#define BLUE     0x001F
#define BRED     0XF81F
#define GRED     0XFFE0
#define GBLUE    0X07FF
#define RED      0xF800
#define MAGENTA  0xF81F
#define GREEN    0x07E0
#define CYAN     0x7FFF
#define YELLOW   0xFFE0
#define BROWN    0XBC40 //棕色
#define BRRED    0XFC07 //棕红色
#define GRAY     0X8430 //灰色

/* Public Struct ------------------------------------------------------------*/

//LTDC重要参数集
typedef struct _ltdc_dev {
  uint8_t  Dir;     //1,横屏;0,竖屏
  uint8_t  Pixsize; //每个像素所占字节
  uint16_t Width;   //面板宽度,单位:像素
  uint16_t Height;  //面板高度,单位:像素
  uint16_t HSync_W; //水平同步宽度
  uint16_t VSync_W; //垂直同步宽度
  uint16_t HBP;     //水平后廊
  uint16_t VBP;     //垂直后廊
  uint16_t HFP;     //水平前廊
  uint16_t VFP;     //垂直前廊
} ltdc_t;

/* Public enum --------------------------------------------------------------*/

/* Public Variable ----------------------------------------------------------*/
//Pointer

//Array

//Const

/* Pubilc function prototypes -----------------------------------------------*/
void Drv_LTDC_Init(void);

void Drv_LTDC_Draw_Point(uint16_t x, uint16_t y, uint16_t color);
uint16_t Drv_LTDC_Read_Point(uint16_t x, uint16_t y);
void Drv_LTDC_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
void Drv_LTDC_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);
void Drv_LTDC_Clear(uint16_t color);

void Drv_LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);
void Drv_LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p);

void Drv_LTDC_Display_Dir_Set(uint8_t dir);
void Drv_LTDC_PointColor_Set(uint16_t color);
ltdc_t Drv_LTDC_Get(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __DRV_LTDC_H */
