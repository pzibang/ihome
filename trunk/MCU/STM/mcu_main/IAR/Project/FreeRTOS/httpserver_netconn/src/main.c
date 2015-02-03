/**
  ******************************************************************************
  * @file    main.c
  * @author  urock
  * @version V0.1.0
  * @date    07-Jan-2014
  * @brief   Main program body
  ******************************************************************************

  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "dmx512.h"
#include "gpio_low_level.h"
#include "uart_low_level.h"
#include "spi_low_level.h"
#include "tcp_server_netconn.h"
#include "power_cntrl_thread.h"
#include "uart_thread.h"
#include "flash.h"
#include "udp_server.h"
#include "tcp_protocol.h"
    
    
mcu_config_t	mcu_config;

xTimerHandle	xOneWireSendTimer;
uint8_t send_timer = 0; // ���� � ���, ��� ������ �������� - ������������� ��� �� ���������  � ��������� ������ ��� �������� ������ � 1-wire

/* ������ ������ �������������� ������� = 1 ������� */
unsigned int uiAutoReloadTimerPeriod = 60000 / portTICK_RATE_MS;  // ����� ����� 1 ������
const portTickType xTicksToWait = 300 / portTICK_RATE_MS;  // ���� ����� ������ ��� ������ ��� ������ ������ ��\� ������� 

 
int CONNECTED_BOARD_KRM[8] = {0,0,0,0,0,0,0,0};

xQueueHandle power_control_queue ;
xQueueHandle tcp_client_queue ;
xQueueHandle tcp_tempsensor_queue;

xSemaphoreHandle xUSARTSemaphore;


/*Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

 int main(void)
{ 

	 portBASE_TYPE xReturn;  //  TaskCreate Error

	 uint8_t	rd_val=1;
         uint8_t i;

#ifdef SERIAL_DEBUG
	 DebugComPort_Init();

	 vSemaphoreCreateBinary(xUSARTSemaphore);
	 if (xUSARTSemaphore == NULL) {
		msg_dbg(("vSemaphoreCreateBinary failed\n"));
		while(1);
	 }
#endif

	 msg_dbg(("DBG: Program start, base type size is %d\n\n\r",sizeof(portBASE_TYPE)));
	 // ������������� �����������
	 init_leds();
	 // ������������� spi ����������
	 init_spi();
	 // �������� 1 ������������ ����� KRM - ���� ����� ���������� � ��� �� � ������ �� ����� ���� ����� ������ 1 ���� ��� ����� �� 0
         for (i = 1; i < NUM_RELAY_BOARDS; i++){
            rd_val = spi_read_id(i);  	// ��������� ID ����� KRM
            if (rd_val == 0x81){		// ���� ID �������, �� ����� ����������
              CONNECTED_BOARD_KRM[i-1] = 1;
              
              msg_dbg(("board present %d\n\r",i));
            }
            else{
              CONNECTED_BOARD_KRM[i-1] = 0;
            }
         }

	 led_off(Warning_Led);
	 led_off(Alarm_Led);
	 led_on(Status_Led);

// ������������� ������
	 init_config_t(&mcu_config);

/* configure Ethernet (GPIOs, clocks, MAC, DMA) */ 
	 ETH_BSP_Config();
    
/* Initilaize the LwIP stack */
	 LwIP_Init();

//	 UDP_server_init();
	  
//  /* Initialize webserver demo */
  tcp_server_netconn_init(); //   // DEFAULT_THREAD_STACKSIZE = 500B

	msg_dbg(("Done Init, Start tasks\n\r"));
// ������������� ��������
	// ������� ��� �������� ��������� ������ �� ����� KRM
	power_control_queue 	= xQueueCreate( 8, sizeof(struct  relay_data_t ) );
	// ������� ��� ������ ������� � ���� �����������
	tcp_client_queue 	= xQueueCreate( 8, sizeof(struct relay_port_status_t) );
	// ������� �������� �������� � ����������� �� ���� ���������
    tcp_tempsensor_queue 	= xQueueCreate( 8, sizeof(temp_status_t) );
	
	if ( (power_control_queue != NULL) && (tcp_client_queue != NULL) && (tcp_tempsensor_queue != NULL)) { // ���� ������� ���������
// ������ ������ ������ �� ������ KRM �� ���������� spi
		// @todo: ��������� ������ ����������� �����
		xReturn = xTaskCreate(power_control_thread, (signed portCHAR *)"power_control_thread", configMINIMAL_STACK_SIZE * 2, NULL, POWER_CNTRL_PRIO, NULL);
		if (xReturn != pdPASS){
			msg_dbg(("Can't create thread_relay. Error = %d \n", (int)xReturn));
		}
// @todo: ??? ��� �� ������ ??
//		xReturn = xTaskCreate(thread_comport_out, ( signed portCHAR * )"thread_comport_out", configMINIMAL_STACK_SIZE * 2, NULL , thread_comport_out_PRIO, NULL);
//		if (xReturn != pdPASS){
//			msg_dbg(("Can't create thread_comport_out. Error = %d \n", xReturn));
//		}
// @todo: ��� �� ������ ??
		xReturn = xTaskCreate(thread_comport_in, ( signed portCHAR * )"thread_comport_in", configMINIMAL_STACK_SIZE * 2, NULL , thread_comport_in_PRIO, NULL);
		if (xReturn != pdPASS){
			msg_dbg(("Can't create thread_comport_in. Error = %d \n", xReturn));
		}

//  ----- Read  1-wire interface -----------------------------------------------
/* @todo: ��� ���������� ����� �������� 1-wire ���������� ����������� ������� stack configMINIMAL_STACK_SIZE * 12  !!!! � ���������� #define MAX_DS18
 ����� ���� �������� ���������� configMINIMAL_STACK_SIZE �� ���-�� ��������, ����� ������� ������� ������ ����� ��� ������ �������*/
                //������ ����� 64 �������
        xReturn = xTaskCreate(vGetTemperature, (signed portCHAR *)"Temperature", configMINIMAL_STACK_SIZE * 12, NULL, ONEWIRE_TASK_PRIO, NULL);   // ������ ������ 1-wire ���� � ���������� ����������� � �������� ����������� � �������� �� ����
        if (xReturn != pdPASS){
            printf ("Can't create Get_Temperature Task, see errors %d in file 'projdefs.h' \n", xReturn);
        }
//------------------------------------------------------------------------------
// �������� ������ ��� ���������� DMX512 � ������������� ��
// @todo: � ������ 2 ����� KCM ������� ��������� RS485

//       xReturn = xTaskCreate(vSendDMX512, (signed portCHAR *)"DMX512", configMINIMAL_STACK_SIZE * 5, NULL, DMX512_TASK_PRIO, NULL);   // ������ ������ 1-wire ���� � ���������� ����������� � �������� ����������� � �������� �� ����
//       if (xReturn != pdPASS){
//           printf ("Can't create Send DMX 512 Task, see errors %d in file 'projdefs.h' \n", xReturn);
//       }

//  ����� ���������� ����� �� ��������..
    printf(" Create task_list \n\n");    
      xReturn = xTaskCreate(my_task_TaskList, (signed portCHAR *)"TaskList", configMINIMAL_STACK_SIZE * 6, NULL, ONEWIRE_TASK_PRIO, NULL);        
      if (xReturn != pdPASS){
        printf ("Can't create TaslList task, see errors %d in file 'projdefs.h' \n", xReturn);             
      }                
//------------------------------------------------------------------------------
// ������ ������� ������� TCP ����� ���������� ������ �� ����
      printf(" Create TCP Client task \n\n");
        xReturn = xTaskCreate(client_netconn_thread, (signed portCHAR *)"TCP Client", configMINIMAL_STACK_SIZE * 2, NULL, TCP_SERVER_THREAD_PRIO, NULL);
        if (xReturn != pdPASS){
          printf ("Can't create TCP Client task, see errors %d in file 'projdefs.h' \n", xReturn);
        }
//------------------------------------------------------------------------------
//		tcp_client_netconn_init();
				
	} else {
		msg_dbg(("Can't create xQueue_relay_x \n"));    
	}

	/* Start scheduler */
	vTaskStartScheduler();
  
	while(1);

	return 0;

}


/** ----------------------------------------------------------------------------
* @brief � ���� ������� ����������� ����� ���� �������� ���� ����������� ���� ������������
* �������� ����� ������ ���������
* @param
* @return void
*/
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName ){
// ���� ������������� Stack �� �������� ���������� ��������� alarm
  led_off(Alarm_Led);
  portCHAR PAGE_BODY[1024];
  printf("\n\n");

  strcat((char *)PAGE_BODY, "Name          State  Priority  Stack   Num\n\r" );
  strcat((char *)PAGE_BODY, "---------------------------------------------\n\r");
  
    
  /* The list of tasks and their status */
  vTaskList((signed char *)(PAGE_BODY + strlen(PAGE_BODY)));
  printf("\n%s\n", "task name status priority HWMTS ID");
  strcat((char *)PAGE_BODY, "---------------------------------------------\n\r");
  strcat((char *)PAGE_BODY, "B : Blocked, R : Ready, D : Deleted, S : Suspended\n\n\r");
   
  printf ("%s\n", PAGE_BODY);
}


//---------------------------------------------------------------------------
/**
* @brief ������� ������������ ��������. 
* 
* 
*/
//------------------------------------------------------------------------------
void vOneWireSendTimerFunction(xTimerHandle xTimer) {
  send_timer = 1;  // ��������� ������
  return;
}

//------------------------------------------------------------------------------

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

//  ������ ������ ������ ����� �� ��������
void my_task_TaskList (void *dev){
  for (;;){  
  
  portCHAR PAGE_BODY[512];
  portCHAR pagehits[10] = {0};

  memset(PAGE_BODY, 0,512);

  /* Update the hit count */
  strcat(PAGE_BODY, pagehits);
  strcat((char *)PAGE_BODY, "Name          State  Priority  Stack   Num\n" );
  strcat((char *)PAGE_BODY, "---------------------------------------------\n");
    
  /* The list of tasks and their status */
  vTaskList((signed char *)(PAGE_BODY + strlen(PAGE_BODY)));
  strcat((char *)PAGE_BODY, "---------------------------------------------\n");
  strcat((char *)PAGE_BODY, "B : Blocked, R : Ready, D : Deleted, S : Suspended\n\n");
  printf ("%s\n", PAGE_BODY);
    
  vTaskDelay (10000);  // ����� ������ �������
  } //end for 
  
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

// @todo: ������ ������� � ��� ����� �������

/*
         rd_val = spi_read_id(1);
         msg_dbg(("board 1 state %d\n\r",rd_val));

         relay_write_reg_spi(1, 0xA1);
         relay_write_reg_spi(3, 0xA3);

         rd_val = relay_read_reg_spi(3);
         msg_dbg(("board 3 state %d\n",rd_val));

         relay_write_reg_spi(1, 0xFF);
         relay_write_reg_spi(3, 0xFF);

         rd_val = relay_read_reg_spi(1);
         msg_dbg(("board 1 state %d\n",rd_val));

         rd_val = relay_read_reg_spi(3);
         msg_dbg(("board 3 state %d\n",rd_val));
*/


/*
	 if (rd_val != rd_val) {
	 	errors++;
		printf("errors = %d\n",errors);
	 }

	 if (count == 0) {
	 	printf("cycles = %d, errors = %d\n",cycles, errors);
		cycles++;
	 }
	 count++;
	 wr_val++;

	 }
*/

