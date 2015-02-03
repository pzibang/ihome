
#include "dmx512.h"
//----- ���������� ���������� --------------------------------------------------
DMA_InitTypeDef  DMA_InitStructureTX_DMX512;
struct dmx512_t dmx, dmx_old;   // dmx - ������ ������� �������� ��������� DMX
                                // dmx_old - ������ ���������� �������� ��������� DMX
//extern xSemaphoreHandle xUSARTSemaphore;
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* ������� ������������� ����������������� ���������� USART2 - ������������ ��� DMX512
* � ������ DMA �� TX, � ������ ������ �� ������������ ������ ���������� �� DMX512 ���������, 
* ������ ��������. 
*/
void init_dma_uart2() {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
       
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(USART_DMX512_DMA_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(USART_DMX512_CLK, ENABLE);
      
    GPIO_InitStructure.GPIO_Pin = USART_DMX512_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
    GPIO_Init(USART_DMX512_TX_PORT, &GPIO_InitStructure); 
    
    GPIO_PinAFConfig(USART_DMX512_TX_PORT, USART_DMX512_TX_SOURCE, USART_DMX512_TX_AF);    
   
/*
* ������� ������ 250000 - ���������� ����������
* ���������� 8 ���, 1 ��� -(low)��������� ���
* 2 ��� (hight) - �������� ����    
*/    
    USART_InitStructure.USART_BaudRate = 250000;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_2;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(USART_DMX512, &USART_InitStructure);

// �������� USART bynthatqc    
    USART_Cmd(USART_DMX512, ENABLE);
    
// ���������� DMA ��� ���������� USART2    
    DMA_DeInit(USART_DMX512_TX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_DMX512_TX_DMA_STREAM) != DISABLE)  //���������� ���� ����� DeInit
    {
    }
    
    DMA_InitStructureTX_DMX512.DMA_Channel = USART_DMX512_TX_DMA_CHANNEL;
    DMA_InitStructureTX_DMX512.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Transmit
    DMA_InitStructureTX_DMX512.DMA_Memory0BaseAddr = (uint32_t) dmx.data;  //����� ���������� ������ �� ����� ������
    DMA_InitStructureTX_DMX512.DMA_BufferSize = DMX512_devices;  // ���������� ������������ ������
    DMA_InitStructureTX_DMX512.DMA_PeripheralBaseAddr = (uint32_t)&USART_DMX512->DR;
    DMA_InitStructureTX_DMX512.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructureTX_DMX512.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructureTX_DMX512.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructureTX_DMX512.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructureTX_DMX512.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructureTX_DMX512.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructureTX_DMX512.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructureTX_DMX512.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructureTX_DMX512.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructureTX_DMX512.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;       
}

/**-----------------------------------------------------------------------------
* @brief �������� ���������� DMX512 � ������ ������ ���������� ������� break 
* �������� �� 100��� �������� MaB ����� 44���. ��. ������������ 
*-------------------------------------------------------------------------------
*/
void DMX512_Break() {
	USART_InitTypeDef USART_InitStructure;        
//-- ������ ������� �� ��������� hight � ��������� low �������� �� 100 - 200���
//-- �� �� ����� 1�, ��� ����� ������ ��������� USART        
	USART_InitStructure.USART_BaudRate = 20000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART_DMX512, &USART_InitStructure);

	USART_ClearFlag(USART_DMX512, USART_FLAG_TC); //������� ���� �������� �� - transfer comlete      
	USART_SendData(USART_DMX512, 0xC0);  //�������� break - ������� ���������� ������� � ����� ������� �������

  //@todo: ���������� ������� ����� ������ ���������
	while (USART_GetFlagStatus(USART_DMX512, USART_FLAG_TC) == RESET) {  //���������� ���� �� �������� ��� ������ �� �����
          
              #ifdef OW_GIVE_TICK_RTOS
		taskYIELD();
              #endif
	}
// ---- ���������� �������������� ��������� USART
      USART_InitStructure.USART_BaudRate = 250000;
      USART_InitStructure.USART_WordLength = USART_WordLength_8b;
      USART_InitStructure.USART_StopBits = USART_StopBits_2;
      USART_InitStructure.USART_Parity = USART_Parity_No;
      USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
      USART_InitStructure.USART_Mode = USART_Mode_Tx;
      USART_Init(USART_DMX512, &USART_InitStructure);  
}

//------------------------------------------------------------------------------
/**-----------------------------------------------------------------------------
* �������� ������ � ��������� USART �� DMA
*
*
*/
void DMX512_SendData(void){  
  
    DMA_DeInit(USART_DMX512_TX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_DMX512_TX_DMA_STREAM) != DISABLE)   //������� ��������� ��������� DeInit        
    {      
    }  
          
    DMA_Init(USART_DMX512_TX_DMA_STREAM, &DMA_InitStructureTX_DMX512);

    USART_ClearFlag(USART_DMX512, USART_FLAG_RXNE | USART_FLAG_TC | USART_FLAG_TXE);    //����������� ����� ���������� ��� �����
    USART_DMACmd(USART_DMX512, USART_DMAReq_Tx, ENABLE);                        // ��������� USART � DMA
    DMA_ClearFlag(USART_DMX512_TX_DMA_STREAM, USART_DMX512_TX_DMA_FLAG_TCIF);  //����������� ����� ���������� ��� �����      
    DMA_Cmd(USART_DMX512_TX_DMA_STREAM, ENABLE);                               // �������� ����� DMA
    
/*
* @todo:���� ����� �������� �������, ����� ��������� �� ��������
*/    
    while((DMA_GetFlagStatus(USART_DMX512_TX_DMA_STREAM, USART_DMX512_TX_DMA_FLAG_TCIF) == RESET));           
    
    while (USART_GetFlagStatus(USART_DMX512, USART_FLAG_TC) == RESET) //���������� ���� �������� ��� ����� �� UART
      {
      }    
    
    DMA_ClearFlag(USART_DMX512_TX_DMA_STREAM, USART_DMX512_TX_DMA_FLAG_HTIF | USART_DMX512_TX_DMA_FLAG_TCIF);    //������� ����� ���    
    DMA_Cmd(USART_DMX512_TX_DMA_STREAM, DISABLE);        
    USART_DMACmd(USART_DMX512, USART_DMAReq_Tx, DISABLE);  // ����� ������ ��� UART
}
//------------------------------------------------------------------------------

//------- Task Send DMX512 Data ------------------------------------------------
/**-----------------------------------------------------------------------------
* ������ � ������� ����������� ��������� ���������� ������ � web ���������� ���
* �������� DMX512
* � ���� ������� ����� ������ ���������� ������� dmx_data � ��������� �������
* DMX512_Break(); DMX512_SendData();   
*/
void vSendDMX512 (void * dev){
//-----�������� ���������� -------------------  
//   uint8_t direction;  // ������������� ��� ����������� �������� � ���������� DMX
//--------------------------------------------   
   init_dma_uart2();  // ������������� USART2 � DMA   
   dmx.data[0] = 0;
   
   while(1){      
     if (dmx.flag_send == 1){
       if (dmx.smooth_step == 0){
        DMX512_Break();
        DMX512_SendData();                        
        dmx.flag_send = 0;
       }
       else {
         
        if (dmx.direction == 1) 
          DMX_smoot_add();               
        else
        DMX_smoot_minus ();
       }
       dmx.flag_send = 0;
       dmx_old = dmx;  // ��������� �������� ��������� DMX
       
//      msg_dbg_tsk(("DMX512: %d \n\r", dmx.data[1]));      
     }  
     vTaskDelay(1000);     
   }    
}
//------------------------------------------------------------------------------
/*
* ������� ���������� �������
*
*/
void DMX_smoot_add (void){  
  uint8_t val;
  val = dmx.data[dmx.adr];
      while (dmx_old.data[dmx.adr] < val){
// -- �� ���� ������� ����� 255                
        if (dmx_old.data[dmx.adr] + dmx.smooth_step <= 255){
          dmx_old.data[dmx.adr] = dmx_old.data[dmx.adr] + dmx.smooth_step;
        }
        else{
          dmx_old.data[dmx.adr] = 255;
        }        
// -- �� ���� ������� ����� �������� ��������                
        if (dmx_old.data[dmx.adr] > val){
          dmx.data[dmx.adr] = val;
          dmx_old.data[dmx.adr] = val;
        }
        else{
        dmx.data[dmx.adr] = dmx_old.data[dmx.adr];
        }
        DMX512_Break();
        DMX512_SendData();                    
      vTaskDelay(dmx.smooth_time);        
  }  
}

//------------------------------------------------------------------------------
/*
*  ������� ���������� �������
*
*/
void DMX_smoot_minus (void){
  uint8_t val;
  val = dmx.data[dmx.adr];
      while (dmx_old.data[dmx.adr] > val){
// -- �� ���� ������� ����� 0        
        if (dmx_old.data[dmx.adr] - dmx.smooth_step >= 0){
          dmx_old.data[dmx.adr] = dmx_old.data[dmx.adr] - dmx.smooth_step;
        }
        else{
          dmx_old.data[dmx.adr] = 0;
        }
// -- �� ���� ������� ����� �������� ��������        
        if (dmx_old.data[dmx.adr] < val){
          dmx.data[dmx.adr] = val;
          dmx_old.data[dmx.adr] = val;
        }
        else{
        dmx.data[dmx.adr] = dmx_old.data[dmx.adr];
        }
        DMX512_Break();
        DMX512_SendData();                    
        vTaskDelay(dmx.smooth_time); 
   }
}  
