#ifndef __DRV_USART_H
#define __DRV_USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "board.h"

////////////////////////////////////////////////////////////////

#define GPIO_USART1_TX  GPIOA,GPIO_Pin_9
#define GPIO_USART1_RX  GPIOA,GPIO_Pin_10

#define GPIO_USART2_TX  GPIOA,GPIO_Pin_2
#define GPIO_USART2_RX  GPIOA,GPIO_Pin_3

#define GPIO_USART3_TX  GPIOB,GPIO_Pin_10
#define GPIO_USART3_RX  GPIOB,GPIO_Pin_11

#define GPIO_UART4_TX   GPIOC,GPIO_Pin_10
#define GPIO_UART4_RX   GPIOC,GPIO_Pin_11

#define GPIO_UART5_TX   GPIOC,GPIO_Pin_12
#define GPIO_UART5_RX   GPIOD,GPIO_Pin_2

#define GPIO_USART6_TX  GPIOC,GPIO_Pin_6
#define GPIO_USART6_RX  GPIOC,GPIO_Pin_7

////////////////////////////////////////////////////////////////

typedef struct {
  void               (*tx_irq_callback)(void);
  void               (*tx_dma_irq_callback)(void);
  DMA_Stream_TypeDef  *txStream;
  uint32_t             tx_tcif;
  uint32_t             txChannel;
  uint8_t             *txBuf;
  uint16_t             txBufSize;
  uint16_t             txBufHead;
  uint16_t             txBufTail;
  uint16_t             txBufNDTR;

  void               (*rx_irq_callback)(void);
  DMA_Stream_TypeDef  *rxStream;
  uint32_t             rx_tcif;
  uint32_t             rxChannel;
  uint8_t             *rxBuf[2];
  uint16_t             rxBufSize;
  uint16_t             rxLen;
  uint8_t              rxObj;
} usart_manage_t;

/////////////////////////////////////////////////////////////////////

int  Drv_Usart_Init(USART_TypeDef *USARTx, uint32_t baud,
                    uint16_t txBufSize, uint16_t rxBufSize,
                    uint8_t PrePri, uint8_t SubPri);
void Drv_Usart_RxIrq_Connect(USART_TypeDef *USARTx, void (*rx_irq)(void));
void Drv_Usart_TxIrq_Connect(USART_TypeDef *USARTx, void (*tx_irq)(void));
void Drv_Usart_TxDmaIrq_Connect(USART_TypeDef *USARTx, void (*tx_dma_irq)(void));
int  Drv_Usart_Write(USART_TypeDef* USARTx, const void* src, uint32_t size);
int  Drv_Usart_Read(USART_TypeDef* USARTx, void* src, uint16_t bufsize);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
