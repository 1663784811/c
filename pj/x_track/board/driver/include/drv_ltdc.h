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
#define LCD_WIDTH   1024  //LCD���Ŀ��,�̶�����,������ʾ����ı�
#define LCD_HEIGHT  600   //LCD���ĸ߶�,�̶�����,������ʾ����ı�

//��1��ʼ��ַ
#define LCD429_FRAME_BUFFER  Bank5_SDRAM_ADDR

//������ɫ
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
#define BROWN    0XBC40 //��ɫ
#define BRRED    0XFC07 //�غ�ɫ
#define GRAY     0X8430 //��ɫ

/* Public Struct ------------------------------------------------------------*/

//LTDC��Ҫ������
typedef struct _ltdc_dev {
  uint8_t  Dir;     //1,����;0,����
  uint8_t  Pixsize; //ÿ��������ռ�ֽ�
  uint16_t Width;   //�����,��λ:����
  uint16_t Height;  //���߶�,��λ:����
  uint16_t HSync_W; //ˮƽͬ�����
  uint16_t VSync_W; //��ֱͬ�����
  uint16_t HBP;     //ˮƽ����
  uint16_t VBP;     //��ֱ����
  uint16_t HFP;     //ˮƽǰ��
  uint16_t VFP;     //��ֱǰ��
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
