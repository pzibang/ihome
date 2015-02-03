/*
 * dmx512.h
 *
 *  Version 1.0.0
 */
//------------------------------------------------------------------------------
#ifndef DMX512_H_
#define DMX512_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "stm32f2xx.h"
#include "croutine.h"
#include "stm32f2xx_dma.h"
#include "main.h"

//---- ������������ USART2 ��� DMX512 ------------------------------------------

#define USART_DMX512                          USART2  
#define USART_DMX512_CLK                      RCC_APB1Periph_USART2 
#define USART_DMX512_AF                       GPIO_AF_USART2

#define USART2_CLK_INIT                       RCC_APB1PeriphClockCmd
/*
*  USART2 �������� �� ���� D  
* TX - 5
* RX - 6   
*/
#define USART_DMX512_TX_PIN                   GPIO_Pin_5                
#define USART_DMX512_TX_GPIO_CLK              RCC_AHB1Periph_GPIOD
#define USART_DMX512_TX_SOURCE                GPIO_PinSource5
#define USART_DMX512_TX_PORT                  GPIOD      
#define USART_DMX512_TX_AF                    GPIO_AF_USART2                 
 //-----------------------------------------------------------------------------
//---- ������������ DMA1 ��� USART2   ------------------------------------------
#define USART_DMX512_DMA                       DMA1
#define USART_DMX512_DMA_CLK                   RCC_AHB1Periph_DMA1
   
#define USART_DMX512_TX_DMA_CHANNEL            DMA_Channel_4
#define USART_DMX512_TX_DMA_STREAM             DMA1_Stream6
   
#define USART_DMX512_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF6
#define USART_DMX512_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF6
#define USART_DMX512_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF6
#define USART_DMX512_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF6
#define USART_DMX512_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF6
#define USART_DMX512_TX_DMA_FLAG_TC            DMA_FLAG_TC6

//--- �������� �������� --------------------------------------------------------
/**
* DMX512_devices - ����� ��������� ��� ������� ��� DMX512 ����������.
* 0 - ����� ������ ������ ���� ����� 0 - �� �������� ����������.
* ��������� ��������� ���������� � ������ 1
*/
#define DMX512_devices  513

struct dmx512_t{  //� ���� ��������� ��������� ��������� ������ ��������� DMX
  uint8_t data[DMX512_devices]; // ������ ������ �������� DNX ���������
  uint8_t adr;          // ����� ������� �������� ���������� DNX
  uint8_t smooth_step;  // ��� �������� ��������� ��������
  uint8_t smooth_time;  // ����� ����� ���������� ������ ��������� ��������
  uint8_t direction;    // 1 - ����������� 0 - ���������� �������� DMX ����������
  uint8_t flag_send;    // ���� 1- ���������� �������� ������ � DNX ���������
                        // 0 - ������ ��������   
};

//-- �������� ������� ----------------------------------------------------------   
void init_dma_uart2(void);
void DMX512_SendData(void);   
void vSendDMX512 (void * dev);
void DMX512_Break();
void DMX_smoot_add (void);  
void DMX_smoot_minus (void);  


#endif /* DMX512_H_ */