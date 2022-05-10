/**
  *****************************************************************************
  * @file 
  * @brief 
  * @author  guojiawei
  * @version
  * @date    2021/07/16
  *****************************************************************************
  */
/* Includes -----------------------------------------------------------------*/
#include "drv_usart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Private defines ----------------------------------------------------------*/

/* Private Struct -----------------------------------------------------------*/
usart_manage_t usart_manage[6] = {  /* ����1������6���������� */
  {NULL, NULL, DMA2_Stream7, DMA_FLAG_TCIF7, DMA_Channel_4, NULL, 32, 0, 0, 0, NULL, DMA2_Stream2, DMA_FLAG_TCIF2, DMA_Channel_4, NULL, NULL, 32, 0, 0},
  {NULL, NULL, DMA1_Stream6, DMA_FLAG_TCIF6, DMA_Channel_4, NULL, 32, 0, 0, 0, NULL, DMA1_Stream5, DMA_FLAG_TCIF5, DMA_Channel_4, NULL, NULL, 32, 0, 0},
  {NULL, NULL, DMA1_Stream3, DMA_FLAG_TCIF3, DMA_Channel_4, NULL, 32, 0, 0, 0, NULL, DMA1_Stream1, DMA_FLAG_TCIF1, DMA_Channel_4, NULL, NULL, 32, 0, 0},
  {NULL, NULL, DMA1_Stream4, DMA_FLAG_TCIF4, DMA_Channel_4, NULL, 32, 0, 0, 0, NULL, DMA1_Stream2, DMA_FLAG_TCIF2, DMA_Channel_4, NULL, NULL, 32, 0, 0},
  {NULL, NULL, DMA1_Stream7, DMA_FLAG_TCIF7, DMA_Channel_4, NULL, 32, 0, 0, 0, NULL, DMA1_Stream0, DMA_FLAG_TCIF0, DMA_Channel_4, NULL, NULL, 32, 0, 0},
  {NULL, NULL, DMA2_Stream6, DMA_FLAG_TCIF6, DMA_Channel_5, NULL, 32, 0, 0, 0, NULL, DMA2_Stream1, DMA_FLAG_TCIF1, DMA_Channel_5, NULL, NULL, 32, 0, 0}
};

/* Private enum -------------------------------------------------------------*/

/* Private Variable ---------------------------------------------------------*/
//Pointer

//Array

//Const

/* Private function prototypes ----------------------------------------------*/


/*****************************************************************************
* @brief  ��ȡid
* ex:
* @par
* None
* @retval
*****************************************************************************/
static int getUsartID(USART_TypeDef *USARTx)
{
	if      (USARTx == USART1) return 0;
	else if (USARTx == USART2) return 1;
	else if (USARTx == USART3) return 2;
	else if (USARTx == UART4)  return 3;
	else if (USARTx == UART5)  return 4;
	else if (USARTx == USART6) return 5;
	else return -1;
}

/*****************************************************************************
* @brief  ��ʼ��
* ex:
* @par
* USARTx   :��ѡ�Ĵ���
* baud     :������
* txBufSize:���ͻ����С
* rxBufSize:���ջ����С
* PrePri   :���ж����ȼ�
* SubPri   :���ж����ȼ�
* @retval
*****************************************************************************/
int Drv_Usart_Init(USART_TypeDef *USARTx, uint32_t baud,
                      uint16_t txBufSize, uint16_t rxBufSize,
                      uint8_t PrePri, uint8_t SubPri)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  DMA_InitTypeDef   DMA_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;

  if (USARTx != USART1 && USARTx != USART2 && USARTx != USART3 && USARTx != UART4 && USARTx != UART5 && USARTx != USART6) {
    return -1;
  }

  /* ��ȡ��ǰ�Ĵ��ڹ������� */
  usart_manage_t *usart = &usart_manage[getUsartID(USARTx)];

  /* ����Ƿ��ʼ������ */
  if ((usart->txBuf != NULL) && (usart->rxBuf[0] != NULL) && (usart->rxBuf[1] != NULL)) {
    return 0;
  }

  usart->txBufSize = txBufSize;
  usart->rxBufSize = rxBufSize;

  /************************************************************************/
  /* ���뷢�ͺͽ��ջ�����                                                 */
  /************************************************************************/
  usart->txBuf = (uint8_t *)pvPortMalloc(usart->txBufSize);
  if (usart->txBuf == NULL) {
    goto _fail_malloc_txbuf;
  }
  usart->rxBuf[0] = (uint8_t *)pvPortMalloc(usart->rxBufSize);
  if (usart->rxBuf[0] == NULL) {
    goto _fail_malloc_rxbuf0;
  }
  usart->rxBuf[1] = (uint8_t *)pvPortMalloc(usart->rxBufSize);
  if (usart->rxBuf[1] == NULL) {
    goto _fail_malloc_rxbuf1;
  }

  /************************************************************************/
  /* �ײ�Ӳ����ʼ��                                                       */
  /************************************************************************/
  if (USARTx == USART1) { /* ����1 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* ע���ⲽ����֮ǰҪ�ȿ�ʱ�� */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePri;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SubPri;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  else if (USARTx == USART2) { /* ����2 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePri;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SubPri;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_Init(&NVIC_InitStructure);
//    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
//    NVIC_Init(&NVIC_InitStructure);
  }
  else if (USARTx == USART3) { /* ����3 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePri;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SubPri;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  else if (USARTx == UART4) { /* ����4 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePri;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SubPri;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
    NVIC_Init(&NVIC_InitStructure);
  }
  else if (USARTx == UART5) { /* ����5 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2,  GPIO_AF_UART5);
    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePri;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SubPri;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_Init(&NVIC_InitStructure);
//    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
//    NVIC_Init(&NVIC_InitStructure);
  }
  else if (USARTx == USART6) { /* ����6 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePri;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = SubPri;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_Init(&NVIC_InitStructure);
//    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
//    NVIC_Init(&NVIC_InitStructure);
  }

  //����
  DMA_DeInit(usart->txStream);
  DMA_StructInit(&DMA_InitStructure);                                    //����Ĭ��DMA��������
  DMA_InitStructure.DMA_Channel            = usart->txChannel;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USARTx->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)usart->txBuf;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral; //��������
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;       //�ڴ��ַ����
  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;        //���ȵȼ�Ϊ��
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
  DMA_Init(usart->txStream, &DMA_InitStructure);
  DMA_Cmd(usart->txStream, DISABLE);                //�ȹر�DMA���ڷ���

  USART_StructInit(&USART_InitStructure);           //����Ĭ�ϵĴ�����������
  USART_InitStructure.USART_BaudRate = baud;        //����������
  USART_Init(USARTx, &USART_InitStructure);         //��ʼ������

  USARTx->SR = 0;                                   //״̬�Ĵ�����0
  DMA_ITConfig(usart->txStream, DMA_IT_TC, ENABLE); //������������ж�
  USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);    //����DMA����
  USART_Cmd(USARTx, ENABLE);                        //��������

  //����
  DMA_DeInit(usart->rxStream);
  DMA_StructInit(&DMA_InitStructure);                                    //����Ĭ��DMA��������
  DMA_InitStructure.DMA_Channel            = usart->rxChannel;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USARTx->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)usart->rxBuf[0];
  DMA_InitStructure.DMA_BufferSize         = usart->rxBufSize;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory; //��������
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;       //�ڴ��ַ����
  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;        //���ȵȼ�Ϊ��
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
  DMA_Init(usart->rxStream, &DMA_InitStructure);
  DMA_Cmd(usart->rxStream, ENABLE);

  USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE); //�رս����ж�
  USART_ITConfig(USARTx, USART_IT_TC,   ENABLE);  //������������ж�
  USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);  //���������ж�
  USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);  //����DMA����

  return 0;

  _fail_malloc_rxbuf1:
  vPortFree(usart->rxBuf[0]); /* �Ѿ�����ɹ���һ��Ҫ�ͷŵ� */
  _fail_malloc_rxbuf0:
  vPortFree(usart->txBuf);    /* �Ѿ�����ɹ���һ��Ҫ�ͷŵ� */
  _fail_malloc_txbuf:
  return -1;
}

/*****************************************************************************
* @brief  ���ý����жϻص�����
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_Usart_RxIrq_Connect(USART_TypeDef *USARTx, void (*rx_irq)(void))
{
  usart_manage_t *usart = &usart_manage[getUsartID(USARTx)];
  
  if (rx_irq != NULL)
  {
    usart->rx_irq_callback = rx_irq;
  }
}

/*****************************************************************************
* @brief  ���÷����жϻص�����(ʹ��DMA���͵�ʱ�򲢲���ÿ�ֽڶ��ж�)
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_Usart_TxIrq_Connect(USART_TypeDef *USARTx, void (*tx_irq)(void))
{
  usart_manage_t *usart = &usart_manage[getUsartID(USARTx)];
  
  if (tx_irq != NULL)
  {
    usart->tx_irq_callback = tx_irq;
  }
}

/*****************************************************************************
* @brief  ����DMA��������жϻص�����
* ex:
* @par
* None
* @retval
*****************************************************************************/
void Drv_Usart_TxDmaIrq_Connect(USART_TypeDef *USARTx, void (*tx_dma_irq)(void))
{
  usart_manage_t *usart = &usart_manage[getUsartID(USARTx)];
  
  if (tx_dma_irq != NULL)
  {
    usart->tx_dma_irq_callback = tx_dma_irq;
  }
}

/*****************************************************************************
* @brief  д��
* ex:
* @par
* None
* @retval
*****************************************************************************/
int Drv_Usart_Write(USART_TypeDef* USARTx, const void* src, uint32_t size)
{
  usart_manage_t *usart = &usart_manage[getUsartID(USARTx)];
  uint32_t emptyBufLen = 0; /* ���õĿռ� */
  uint32_t len_1 = 0, len_2 = 0;

  if (usart->txBufHead >= usart->txBufTail) {
    emptyBufLen = usart->txBufSize - (usart->txBufHead - usart->txBufTail) - 1;
    
    if (emptyBufLen == 0) {
      return 0;
    }
    
    /* ���ܳ������λ�����õĿռ� */
    if (size > emptyBufLen) {
      size = emptyBufLen;
    }
    
    /* �������������Ƿ���Ҫ�ֶλ��� */
    if (size >= usart->txBufSize - usart->txBufHead) {
      len_1 = usart->txBufSize - usart->txBufHead;
      len_2 = size - len_1;
      memcpy(usart->txBuf + usart->txBufHead, (uint8_t *)src, len_1);
      memcpy(usart->txBuf, (uint8_t *)src + len_1, len_2);
      usart->txBufHead = 0 + len_2;
    }
    else {
      memcpy(usart->txBuf + usart->txBufHead, src, size);
      usart->txBufHead += size;
    }
  }
  else {
    emptyBufLen = usart->txBufTail - usart->txBufHead - 1;
    
    /* ���ܳ������λ�����õĿռ� */
    if (size > emptyBufLen) {
      size = emptyBufLen;
    }
    
    memcpy(usart->txBuf + usart->txBufHead, src, size);
    usart->txBufHead += size;
  }

  /* ����Ƿ���Ҫ���÷��� */
  if ((usart->txStream->NDTR == 0) || (DMA_GetCmdStatus(usart->txStream) == DISABLE)) {
    DMA_Cmd(usart->txStream, DISABLE);                //ֹͣ����,�����µķ�������
    DMA_ClearFlag(usart->txStream, usart->tx_tcif);   //���������ɱ�־λ
    usart->txStream->M0AR = (uint32_t)usart->txBuf + usart->txBufTail;
    usart->txStream->NDTR = len_1 > 0 ? len_1 : size; //��Ҫ���͵ĳ���
    usart->txBufNDTR = usart->txStream->NDTR;         //��¼���ο����÷��͵����ݳ���
    DMA_Cmd(usart->txStream, ENABLE);                 //��ʼ����
  }

  return size;
}

/*****************************************************************************
* @brief  ��ȡ
* ex:
* @par
* None
* @retval
*****************************************************************************/
int Drv_Usart_Read(USART_TypeDef* USARTx, void* src, uint16_t bufsize)
{
  usart_manage_t *usart = &usart_manage[getUsartID(USARTx)];
  uint8_t  obj = usart->rxObj;
  uint16_t len = usart->rxLen;
  
  if (len > bufsize) len = bufsize; /* ��ֹԽ�� */

  if (len > 0) {
    DMA_Cmd(usart->rxStream, DISABLE);
    DMA_ClearFlag(usart->txStream, usart->rx_tcif);
    usart->rxObj = 1 - obj;
    usart->rxLen = 0;
    usart->rxStream->M0AR = (uint32_t)usart->rxBuf[1 - obj];
    usart->rxStream->NDTR = usart->rxBufSize;
    DMA_Cmd(usart->rxStream, ENABLE);
    
    memcpy((uint8_t *)src, usart->rxBuf[obj], len);
  }

  return len;
}

/*****************************************************************************
* @brief  ����1�ж�
* ex:
* @par
* None
* @retval
*****************************************************************************/
void USART1_IRQHandler(void)
{
  usart_manage_t *usart = &usart_manage[getUsartID(USART1)];
  
  if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
  {
    uint16_t temp;
    /* �����жϵ�����Ƚ��ر�,����д�Ĵ���,���Ƕ��Ĵ��� */
    temp = USART1->SR;
    temp = USART1->DR;
    temp = usart->rxBufSize - usart->rxStream->NDTR;
    if (temp < usart->rxBufSize) {
        usart->rxLen = temp;
    }
    else {
        usart->rxLen = usart->rxBufSize;
    }
    
    /* �����жϻص� */
    if (usart->rx_irq_callback != NULL)
    {
      usart->rx_irq_callback();
    }
  }
  
  if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
  {
    USART_ClearITPendingBit(USART1, USART_IT_TC);
    
    /* �����жϻص� */
    if (usart->tx_irq_callback != NULL)
    {
      usart->tx_irq_callback();
    }
  }
}

/*****************************************************************************
* @brief  ����1�ķ���DMA�ж�
* ex:
* @par
* None
* @retval
*****************************************************************************/
void DMA2_Stream7_IRQHandler(void)
{
  usart_manage_t *usart = &usart_manage[getUsartID(USART1)];

  if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET) {
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
    
    usart->txBufTail += usart->txBufNDTR;
    
    /* β������,���λ�����Ҫ��ͷ���� */
    if (usart->txBufTail >= usart->txBufSize) {
      usart->txBufTail = 0;
    }
    
    DMA_Cmd(usart->txStream, DISABLE);
    usart->txStream->M0AR = (uint32_t)usart->txBuf + usart->txBufTail;
    
    /* β׷��ͷ�����,˵�����ݷ������,����Ҫ�ٿ���DMA */
    if (usart->txBufHead == usart->txBufTail) {
      
      /* ��������жϻص� */
      if (usart->tx_dma_irq_callback != NULL)
      {
        usart->tx_dma_irq_callback();
      }
    }
    else if (usart->txBufHead > usart->txBufTail) {
      usart->txStream->NDTR = usart->txBufHead - usart->txBufTail;
      usart->txBufNDTR = usart->txStream->NDTR;
      DMA_Cmd(usart->txStream, ENABLE);
    }
    else if (usart->txBufHead < usart->txBufTail) {
      usart->txStream->NDTR = usart->txBufSize - usart->txBufTail;
      usart->txBufNDTR = usart->txStream->NDTR;
      DMA_Cmd(usart->txStream, ENABLE);
    }
  }
}

/*****************************************************************************
* @brief  串口3中断
* ex:
* @par
* None
* @retval
*****************************************************************************/
void USART3_IRQHandler(void)
{
  usart_manage_t *usart = &usart_manage[getUsartID(USART3)];
  
  if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
  {
    uint16_t temp;
    /* �����жϵ�����Ƚ��ر�,����д�Ĵ���,���Ƕ��Ĵ��� */
    temp = USART3->SR;
    temp = USART3->DR;
    temp = usart->rxBufSize - usart->rxStream->NDTR;
    if (temp < usart->rxBufSize) {
        usart->rxLen = temp;
    }
    else {
        usart->rxLen = usart->rxBufSize;
    }
    
    /* �����жϻص� */
    if (usart->rx_irq_callback != NULL)
    {
      usart->rx_irq_callback();
    }
  }
  
  if (USART_GetITStatus(USART3, USART_IT_TC) != RESET)
  {
    USART_ClearITPendingBit(USART3, USART_IT_TC);
    
    /* �����жϻص� */
    if (usart->tx_irq_callback != NULL)
    {
      usart->tx_irq_callback();
    }
  }
}

/*****************************************************************************
* @brief  串口3DMA发送中断
* ex:
* @par
* None
* @retval
*****************************************************************************/
void DMA1_Stream3_IRQHandler(void)
{
  usart_manage_t *usart = &usart_manage[getUsartID(USART3)];

  if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) != RESET) {
    DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
    
    usart->txBufTail += usart->txBufNDTR;
    
    /* β������,���λ�����Ҫ��ͷ���� */
    if (usart->txBufTail >= usart->txBufSize) {
      usart->txBufTail = 0;
    }
    
    DMA_Cmd(usart->txStream, DISABLE);
    usart->txStream->M0AR = (uint32_t)usart->txBuf + usart->txBufTail;
    
    /* β׷��ͷ�����,˵�����ݷ������,����Ҫ�ٿ���DMA */
    if (usart->txBufHead == usart->txBufTail) {
      
      /* ��������жϻص� */
      if (usart->tx_dma_irq_callback != NULL)
      {
        usart->tx_dma_irq_callback();
      }
    }
    else if (usart->txBufHead > usart->txBufTail) {
      usart->txStream->NDTR = usart->txBufHead - usart->txBufTail;
      usart->txBufNDTR = usart->txStream->NDTR;
      DMA_Cmd(usart->txStream, ENABLE);
    }
    else if (usart->txBufHead < usart->txBufTail) {
      usart->txStream->NDTR = usart->txBufSize - usart->txBufTail;
      usart->txBufNDTR = usart->txStream->NDTR;
      DMA_Cmd(usart->txStream, ENABLE);
    }
  }
}

/*****************************************************************************
* @brief  ����4�ж�
* ex:
* @par
* None
* @retval
*****************************************************************************/
void UART4_IRQHandler(void)
{
  usart_manage_t *usart = &usart_manage[getUsartID(UART4)];
  
  if (USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
  {
    uint16_t temp;
    /* �����жϵ�����Ƚ��ر�,����д�Ĵ���,���Ƕ��Ĵ��� */
    temp = UART4->SR;
    temp = UART4->DR;
    temp = usart->rxBufSize - usart->rxStream->NDTR;
    if (temp < usart->rxBufSize) {
        usart->rxLen = temp;
    }
    else {
        usart->rxLen = usart->rxBufSize;
    }
    
    /* �����жϻص� */
    if (usart->rx_irq_callback != NULL)
    {
      usart->rx_irq_callback();
    }
  }
  
  if (USART_GetITStatus(UART4, USART_IT_TC) != RESET)
  {
    USART_ClearITPendingBit(UART4, USART_IT_TC);
    
    /* �����жϻص� */
    if (usart->tx_irq_callback != NULL)
    {
      usart->tx_irq_callback();
    }
  }
}

/*****************************************************************************
* @brief  ����4�ķ���DMA�ж�
* ex:
* @par
* None
* @retval
*****************************************************************************/
void DMA1_Stream4_IRQHandler(void)
{
  usart_manage_t *usart = &usart_manage[getUsartID(UART4)];

  if (DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4) != RESET) {
    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
    
    usart->txBufTail += usart->txBufNDTR;
    
    /* β������,���λ�����Ҫ��ͷ���� */
    if (usart->txBufTail >= usart->txBufSize) {
      usart->txBufTail = 0;
    }
    
    DMA_Cmd(usart->txStream, DISABLE);
    usart->txStream->M0AR = (uint32_t)usart->txBuf + usart->txBufTail;
    
    /* β׷��ͷ�����,˵�����ݷ������,����Ҫ�ٿ���DMA */
    if (usart->txBufHead == usart->txBufTail) {
      
      /* ��������жϻص� */
      if (usart->tx_dma_irq_callback != NULL)
      {
        usart->tx_dma_irq_callback();
      }
    }
    else if (usart->txBufHead > usart->txBufTail) {
      usart->txStream->NDTR = usart->txBufHead - usart->txBufTail;
      usart->txBufNDTR = usart->txStream->NDTR;
      DMA_Cmd(usart->txStream, ENABLE);
    }
    else if (usart->txBufHead < usart->txBufTail) {
      usart->txStream->NDTR = usart->txBufSize - usart->txBufTail;
      usart->txBufNDTR = usart->txStream->NDTR;
      DMA_Cmd(usart->txStream, ENABLE);
    }
  }
}
