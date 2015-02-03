/*
 * onewire.c
 *
 *  Version 1.0.3
 */

#include "onewire.h"

DMA_InitTypeDef  DMA_InitStructureTX_1WIRE, DMA_InitStructureRX_1WIRE;
//----������� ���������� -------------------------------------------------------
extern xTimerHandle	xOneWireSendTimer;
extern uint8_t send_timer;
extern xSemaphoreHandle xUSARTSemaphore;
//extern xQueueHandle tcp_tempsensor_queue;
//------------------------------------------------------------------------------

//---���������� ----------------------------------------------------------------
// ����� ��� ������/�������� �� 1-wire
uint8_t ow_buf[8];

//#define OW_0	0x00
//#define OW_1	0xff
//#define OW_R_1	0xff
//------------------------------------------------------------------------------

//----- �������� ������� -------------------------------------------------------
/**-----------------------------------------------------------------------------
* @brief �������������� USART6 � DMA2 ��� ���� 1-wire
* @return void
*-----------------------------------------------------------------------------
*/
void OW_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
       
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(USART_1WIRE_DMA_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(USART_1WIRE_CLK, ENABLE);
       
    GPIO_InitStructure.GPIO_Pin = USART_1WIRE_TX_PIN | USART_1WIRE_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
// ������ ������ �������� ���������    
     GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(USART_1WIRE_TX_PORT, &GPIO_InitStructure); 
    
    GPIO_PinAFConfig(USART_1WIRE_TX_PORT, USART_1WIRE_TX_SOURCE, USART_1WIRE_TX_AF);    
    GPIO_PinAFConfig(USART_1WIRE_RX_PORT, USART_1WIRE_RX_SOURCE, USART_1WIRE_RX_AF);
//--- ������������� �������� �������� 115200 �� ������������   
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART_1WIRE, &USART_InitStructure);
    
//--- ��������� USART6    
    USART_Cmd(USART_1WIRE, ENABLE);
    
//---- ����������� DMA ��� ����������� -----------------------------------------    
    DMA_DeInit(USART_1WIRE_TX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_1WIRE_TX_DMA_STREAM) != DISABLE)  //���������� ���� ����� DeInit
    {
    }
    
    DMA_InitStructureTX_1WIRE.DMA_Channel = USART_1WIRE_TX_DMA_CHANNEL;
    DMA_InitStructureTX_1WIRE.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Transmit
    DMA_InitStructureTX_1WIRE.DMA_Memory0BaseAddr = (uint32_t) ow_buf;  // ����� ��� �������� ������ � 1-wire �����������
    DMA_InitStructureTX_1WIRE.DMA_PeripheralBaseAddr = (uint32_t) &(USART_1WIRE->DR);
    DMA_InitStructureTX_1WIRE.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructureTX_1WIRE.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructureTX_1WIRE.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructureTX_1WIRE.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructureTX_1WIRE.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructureTX_1WIRE.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructureTX_1WIRE.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructureTX_1WIRE.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructureTX_1WIRE.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructureTX_1WIRE.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        
//---- ����������� DMA ��� ��������� -------------------------------------------
    DMA_DeInit(USART_1WIRE_RX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_1WIRE_RX_DMA_STREAM) != DISABLE) //���������� ���� ����� DeInit
    {
    }    
    DMA_InitStructureRX_1WIRE.DMA_Channel = USART_1WIRE_RX_DMA_CHANNEL;
    DMA_InitStructureRX_1WIRE.DMA_DIR = DMA_DIR_PeripheralToMemory; // Receive
    DMA_InitStructureRX_1WIRE.DMA_Memory0BaseAddr = (uint32_t) ow_buf;  // ����� ��� �������� ������ � 1-wire �����������
    DMA_InitStructureRX_1WIRE.DMA_PeripheralBaseAddr = (uint32_t) &(USART_1WIRE->DR);
    DMA_InitStructureRX_1WIRE.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructureRX_1WIRE.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructureRX_1WIRE.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructureRX_1WIRE.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructureRX_1WIRE.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructureRX_1WIRE.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructureRX_1WIRE.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructureRX_1WIRE.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructureRX_1WIRE.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructureRX_1WIRE.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 }
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief ������� ����������� ���� ���� � ������, ��� �������� ����� USART
* @param ow_byte - ����, ������� ���� �������������
* @param ow_bits - ������ �� �����, �������� �� ����� 8 ����
* r@eturn void
* -----------------------------------------------------------------------------
*/
void OW_toBits(uint8_t ow_byte, uint8_t *ow_bits) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		if (ow_byte & 0x01) {
			*ow_bits = OW_1;
		} else {
			*ow_bits = OW_0;
		}
		ow_bits++;
		ow_byte = ow_byte >> 1;
	}
}
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief �������� �������������� - �� ����, ��� �������� ����� USART ����� ���������� ����
* @param ow_bits - ������ �� �����, �������� �� ����� 8 ����
* @return void
*-----------------------------------------------------------------------------
*/
uint8_t OW_toByte(uint8_t *ow_bits) {
	uint8_t ow_byte, i;
	ow_byte = 0;
	for (i = 0; i < 8; i++) {
		ow_byte = ow_byte >> 1;
		if (*ow_bits == OW_R_1) {
			ow_byte |= 0x80;
		}
		ow_bits++;
	}

	return ow_byte;
}
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief ������������ ����� � �������� �� ������� ��������� �� ���� 1- wire
* @return ow_ok ��� ���������� �� �������
*-----------------------------------------------------------------------------
*/
uint8_t OW_Reset() {
	uint8_t ow_presence;
	USART_InitTypeDef USART_InitStructure;
//----������������� �������� USART ��� ��� �������� ����� �������� reset
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART_1WIRE, &USART_InitStructure);

	// ���������� 0xf0 �� �������� 9600 
	USART_ClearFlag(USART_1WIRE, USART_FLAG_TC); //������� ���� �������� �� - transfer comlete
	USART_SendData(USART_1WIRE, 0xf0);  //�������� reset (0f) � ���� 1-wire - ������� ���������� �������, � ����� ������� �������

	while (USART_GetFlagStatus(USART_1WIRE, USART_FLAG_TC) == RESET) {  //���������� ���� �� �������� ��� ������ �� �����
          
            #ifdef OW_GIVE_TICK_RTOS
		taskYIELD();
            #endif
	}

	ow_presence = USART_ReceiveData(USART_1WIRE); //����� reset ���������� �� ���� ������ �������� ������ � 0
        
//--- �������������� �������������� ��������� USART6
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART_1WIRE, &USART_InitStructure);
//--- �������� �� ����������� ��������� 1-wire �� ����
	if (ow_presence != 0xf0) { // ���� ���������� ������������
		return OW_OK;
	}

	return OW_NO_DEVICE;  //��������� �� �������
}
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------
* @brief ���������� ���������. �������� ��������� ����� ��� �� ���������� 1-wire
* @param nbits - ����� ��� ������� ����� �������� �� ���������� 1-wire
* @return void
*------------------------------------------------------------------------
*/
void OW_SendBits(uint8_t nbits){
    portBASE_TYPE xStatus;
  
    DMA_DeInit(USART_1WIRE_TX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_1WIRE_TX_DMA_STREAM) != DISABLE)   //������� ��������� ��������� DeInit        
    {      
    }
    
    DMA_DeInit(USART_1WIRE_RX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_1WIRE_RX_DMA_STREAM) != DISABLE)   //������� ��������� ��������� DeInit        
    {      
    }
    
    DMA_InitStructureTX_1WIRE.DMA_BufferSize = nbits;
    DMA_InitStructureRX_1WIRE.DMA_BufferSize = nbits;
    DMA_Init(USART_1WIRE_TX_DMA_STREAM, &DMA_InitStructureTX_1WIRE);
    DMA_Init(USART_1WIRE_RX_DMA_STREAM, &DMA_InitStructureRX_1WIRE);

    USART_ClearFlag(USART_1WIRE, USART_FLAG_RXNE | USART_FLAG_TC | USART_FLAG_TXE);    //����������� ����� ���������� ��� �����
    USART_DMACmd(USART_1WIRE, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
    
    DMA_ClearFlag(USART_1WIRE_TX_DMA_STREAM, USART_1WIRE_TX_DMA_FLAG_TCIF);  //����������� ����� ���������� ��� �����  
    DMA_ClearFlag(USART_1WIRE_RX_DMA_STREAM, USART_1WIRE_RX_DMA_FLAG_TCIF);  
    
    DMA_Cmd(USART_1WIRE_TX_DMA_STREAM, ENABLE);
    DMA_Cmd(USART_1WIRE_RX_DMA_STREAM, ENABLE);


    //������� ������, ���� ������ ������� �� 0, �� ������ ��� �������� � ������� �� �����, ����� �� ������� ���������, �������� ������ �� ����
    xOneWireSendTimer = xTimerCreate("OneWireSendTimer", 1000/portTICK_RATE_MS, pdFALSE, 0, vOneWireSendTimerFunction);
   /* ��������� ����� ������ ������� ������������� �������. * ������ �� ������ ����������� ����� ����� ����� ������� ������������ * ��������� 2 ������� ����� ������� ������������ ��������� � ��������� ���������. */
    xTimerReset(xOneWireSendTimer, 0);
    send_timer = 0;
       
    while((DMA_GetFlagStatus(USART_1WIRE_TX_DMA_STREAM, USART_1WIRE_TX_DMA_FLAG_TCIF) == RESET)&&(DMA_GetFlagStatus(USART_1WIRE_RX_DMA_STREAM, USART_1WIRE_RX_DMA_FLAG_TCIF) == RESET)&&(send_timer == 0));

    //    ��� ������� ��������� ����� ��������� � ���� �����
    while ((USART_GetFlagStatus(USART_1WIRE, USART_FLAG_TC) == RESET)&&(send_timer == 0))
      {
      }
    //--- ������������� � ������� ������ ---------
//    xStatus = xTimerStop(xOneWireSendTimer,100);
//    if (send_timer == 1){
//    	msg_dbg(("Error in recieve data on 1-wire bus \n"));
//    }
//    xStatus = xTimerDelete(xOneWireSendTimer, 100);
    
    DMA_ClearFlag(USART_1WIRE_TX_DMA_STREAM, USART_1WIRE_TX_DMA_FLAG_HTIF | USART_1WIRE_TX_DMA_FLAG_TCIF);    //������� ����� ���
    DMA_ClearFlag(USART_1WIRE_RX_DMA_STREAM, USART_1WIRE_RX_DMA_FLAG_HTIF | USART_1WIRE_RX_DMA_FLAG_TCIF);    //������� ����� ���    
    DMA_Cmd(USART_1WIRE_TX_DMA_STREAM, DISABLE);
    DMA_Cmd(USART_1WIRE_RX_DMA_STREAM, DISABLE);
    
    USART_DMACmd(USART_1WIRE, USART_DMAReq_Tx | USART_DMAReq_Rx, DISABLE);  // ����� ������ ��� UART
}
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief ��������� ������� � ����� 1-wire
* @param sendReset - �������� RESET � ������ �������.
* 		OW_SEND_RESET ��� OW_NO_RESET
* command - ������ ����, ���������� � ����. ���� ����� ������ - ���������� OW_READ_SLOT
* cLen - ����� ������ ������, ������� ���� ��������� � ����
* data - ���� ��������� ������, �� ������ �� ����� ��� ������
* dLen - ����� ������ ��� ������. ����������� �� ����� ���� �����
* readStart - � ������ ������� �������� �������� ������ (���������� � 0)
*		����� ������� OW_NO_READ, ����� ����� �� �������� data � dLen
* @return ������ ��� �� ������ �������� ������ ����
*!�������� readStart ������� �� ���������� �������, �.�. �� ������ � RX �� ��� ������� (�������), �� � �������� - ��� ��� �� ���������� � ��������� ������ ������
* ----------------------------------------------------------------------------
*/
uint8_t OW_Send(uint8_t sendReset, uint8_t *command, uint8_t cLen,
		uint8_t *data, uint8_t dLen, uint8_t readStart) {

	// ���� ��������� ����� - ���������� � ��������� �� ������� ���������
	if (sendReset == OW_SEND_RESET) {
		if (OW_Reset() == OW_NO_DEVICE) {
			return OW_NO_DEVICE;
		}
	}

	while (cLen > 0) {

		OW_toBits(*command, ow_buf);
		command++;
		cLen--;

		OW_SendBits(8);

		// ���� ����������� ������ ����-�� ����� - ������� �� � �����
		if (readStart == 0 && dLen > 0) {
			*data = OW_toByte(ow_buf);
			data++;
			dLen--;
		} else {
			if (readStart != OW_NO_READ) {
				readStart--;
			}
		}
	}

	return OW_OK;
}
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief ������ ������� ������������ ������������ ���� 1-wire � ���������� ���������
* @param  ID ��������� � ������ buf, �� 8 ���� �� ������ ����������.
* ���������� num ������������ ���������� ��������� ���������, ����� �� �����������
* �����.
* @return - ���������� �������� �������� �� ���� 1-wire
-----------------------------------------------------------------------------*/
uint8_t OW_Scan(uint8_t *buf, uint8_t num) {

        uint8_t found = 0;
        uint8_t *lastDevice;
        uint8_t *curDevice = buf;
        uint8_t numBit, lastCollision, currentCollision, currentSelection;

        lastCollision = 0;
        while (found < num) {
                numBit = 1;
                currentCollision = 0;

                // �������� ������� �� ����� ���������
                OW_Send(OW_SEND_RESET, (uint8_t*)"\xf0", 1, 0, 0, OW_NO_READ);

                for (numBit = 1; numBit <= 64; numBit++) {
                        // ������ ��� ����. �������� � ���������������
                        OW_toBits(OW_READ_SLOT, ow_buf);
                        OW_SendBits(2);

                        if (ow_buf[0] == OW_R_1) {
                                if (ow_buf[1] == OW_R_1) {
                                        // ��� �������, ���-�� ���������� � ����������� �����
                                        return found;
                                } else {
                                        // 10 - �� ������ ����� ������ 1
                                        currentSelection = 1;
                                }
                        } else {
                                if (ow_buf[1] == OW_R_1) {
                                        // 01 - �� ������ ����� ������ 0
                                        currentSelection = 0;
                                } else {
                                        // 00 - ��������
                                        if (numBit < lastCollision) {
                                                // ���� �� ������, �� ����� �� ��������
                                                if (lastDevice[(numBit - 1) >> 3]
                                                                & 1 << ((numBit - 1) & 0x07)) {
                                                        currentSelection = 1;

                                                        // ���� ����� �� ������ �����, ���������� ����� ����
                                                        if (currentCollision < numBit) {
                                                                currentCollision = numBit;
                                                        }
                                                } else {
                                                        currentSelection = 0;
                                                }
                                        } else {
                                                if (numBit == lastCollision) {
                                                        currentSelection = 0;
                                                } else {
                                                        // ���� �� ������ �����
                                                        currentSelection = 1;

                                                        // ���� ����� �� ������ �����, ���������� ����� ����
                                                        if (currentCollision < numBit) {
                                                                currentCollision = numBit;
                                                        }
                                                }
                                        }
                                }
                        }

                        if (currentSelection == 1) {
                                curDevice[(numBit - 1) >> 3] |= 1 << ((numBit - 1) & 0x07);
                                OW_toBits(0x01, ow_buf);
                        } else {
                                curDevice[(numBit - 1) >> 3] &= ~(1 << ((numBit - 1) & 0x07));
                                OW_toBits(0x00, ow_buf);
                        }
                        OW_SendBits(1);
                }
                found++;
                lastDevice = curDevice;
                curDevice += 8;
                if (currentCollision == 0)
                        return found;

                lastCollision = currentCollision;
        }

        return found;
}

//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief ������ ����������� � ��������.DS18b20+ � �������� �� �� ���� �� ETH
* @param
* @return void
* !�������� � ������� OW_Send �������� readStart ������� �� ���������� �������, �.�. �� ������ � RX �� ��� ������� (�������), �� � �������� - ��� ��� �� ���������� � ��������� ������ ������
*/
void vGetTemperature (void * dev){
//-------- ���������� -----------------------------------------   
  struct owidbuf_t       owidbuf[MAX_DS18]; // ���������� ��� �������� ���������� � ���� �������� ID, Temperature
  temp_status_t          temp_status[MAX_DS18];
  
  uint8_t buf[9]; //����� ��� ������ ������ �� �������
  uint8_t idbuf[MAX_DS18][8]; //������ ��� �������� ID ��������
  uint8_t i;  // ���������� ��� �����
  uint8_t j;  // ���������� ��� �����
  uint8_t found; // ���-�� �������� ��������� 1-wire
  signed char integer_temp = 0; //����������� ����� �����    
  signed char frac_temp;        //����������� ������� �����

  portBASE_TYPE xStatus;
//--------------------------------------------------------------
  
  OW_Init();  //������������� ���� 1-wire
  found = OW_Scan((uint8_t *)idbuf, MAX_DS18); // ���� ���������� �� ���� 1-wire
// ������� ���-�� ������� ��������  
  msg_dbg_tsk(("Found 1-wire device: %d \n\r",found));

//---------�������� ID �������� �������� �� ���� ------------------------------
  for(j = 0; j < found; j++ ){
      for(i = 0; i < 8; i++){
        temp_status[j].id[i] = idbuf[j][i];
        msg_dbg_tsk(("%2x:",idbuf[j][i]));
      }
    msg_dbg_tsk(("\n\r"));  //������� �� ��������
    temp_status[j].mode = ADDDEV_1WIRE;
    if (temp_write_output_queue(&temp_status[j]) != pdTRUE) {
       msg_dbg_tsk(("Error in power_control_thread: can not write to output queue\n\r"));
    }     
  }
  
//-----���������� ������� ������ ----------------------------------------------  

  for (j = 0; j < found; j++){  //������� ������ ��� ������� � ������� ���������� �� ���� �� ��� ID
    owidbuf[j].id[0] = 0x55;  //������� ��������� �� ID �� ���� 1-wire
    for(i = 0; i < 8; i++){
      owidbuf[j].id[i+1] = idbuf[j][i];  //���������� � ������ ID
    }
    owidbuf[j].id[9] = 0xbe;  //���������� ���������� ������ - ������� ������ ���� ��������� ������� DS18B20
    
    for (i=10; i < 15; i++){
       owidbuf[j].id[i] = 0xff;  //����� 5 ��� ff � ������ ��� �� ����� ����� ���� ������� 5 ���� �� �������
       //��� ���� ����� ������� ���� ����� ������� ���� ff
    }    
  }
//---- ��������� ����������� � �������� ����������� --------------------------  
  for (;;){  // � ����� �������� ����������� �� ���� �������� 
/*----------------------------------------------------------------------------*/    
//#ifdef Debug_my
//  taskENTER_CRITICAL();
//    size_t mem = xPortGetFreeHeapSize();
//    printf ("Free Heap size in temperature Task = %d \n\n", mem);
//  taskEXIT_CRITICAL();
//#endif
/*----------------------------------------------------------------------------*/
  
    OW_Send(OW_SEND_RESET, "\xcc\x44", 2, NULL, NULL, OW_NO_READ); // �������� ����������� �� ���� ��������
    vTaskDelay(1000);  // ���� �����  1 ��� ���� ���������� ������� ��������� ����������� !!!�����������
    
    for (j = 0; j < found; j++){ // ��������� ����������� ��� ������� �������
      //�������� 15 ����, ��������� 13 ����, �� � ����� ������, ������ � ��������, �.�. � ������ ����� 3 �����
    OW_Send(OW_SEND_RESET, owidbuf[j].id, 15, buf, 13, 10); // ��������� �����������  
    
/****************************************************************/ 

      frac_temp    = buf[0] & 0x0f;                            //�������� ������� �����      
      integer_temp = (buf[0]>>4) | ((buf[1] & 0x0f)<<4);      //�������� ����� ����� 
      
//-------- ���������� ����������� ��� �������� �� �� ���� --------------
        temp_status[j].int_path = integer_temp;      
        temp_status[j].float_path = ((frac_temp * 10)/16);      
        for(i = 0; i < 8; i++){  // ���������� ID ������� � ���������
          temp_status[j].id[i] = idbuf[j][i];
        }        
//----------------------------------------------------------------------
        
      //���� ����������� �������������
      if(integer_temp < 0)
      {
        integer_temp = 0 - integer_temp - 1;
        owidbuf[j].temp  = integer_temp * 10;                            //��������� ����� �����
        frac_temp = frac_temp | 0xf0;
        frac_temp = 0 - frac_temp ;                                 
      }
      //���� ����������� �������������
      else {
        owidbuf[j].temp  = integer_temp * 10;                    //��������� ����� �����        
      }

      owidbuf[j].temp =  owidbuf[j].temp + ((frac_temp * 10)/16);                  //��������� ������� ����� //����� �������� �������� �����������, ����� ��� ����� ��������� �� 10.
//      printf("Temperature %d\n\n", owidbuf[j].temp);
      
//------------------------------------------------------------------------------
//---- ���������� ����������� � ID ������� �� ���� ----------------------------- 
      msg_dbg_tsk(("Tem  1-wire device: %d \n\r", owidbuf[j].temp));      
      msg_dbg_tsk(("Tem  Int path: %d \n\r", temp_status[j].int_path));      
      msg_dbg_tsk(("Tem  float path: %d \n\r", temp_status[j].float_path)); 
      
      temp_status[j].mode = SENDTEMP_1WIRE;  // �������� ����������� �� ����
      if (temp_write_output_queue(&temp_status[j]) != pdTRUE) {
        msg_dbg_tsk(("Error in power_control_thread: can not write to output queue\n\r"));
      } 
      
//------------------------------------------------------------------------------      
      
      
    } //end for ������� �������� �����������
//**********************************************************************/
    // �������� � 5 ����� - ������ ������ �� ������
  vTaskDelay(request_1wire);
  }  
}  

