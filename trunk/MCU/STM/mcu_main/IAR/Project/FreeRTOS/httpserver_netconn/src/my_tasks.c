#include "my_tasks.h"

//extern volatile struct PORT_struct PORTs[MAX_PORTs];  // ������ ������ ��� ����� ��������� ��� �� ���������.
//extern const uint16_t GPIOM_PIN[MAX_PORTs];
//extern GPIO_TypeDef* GPIOM_PORT[MAX_PORTs];
//extern volatile struct ADC_PORT_struct ADC_PORTs[ADC_PORTn];
//extern __IO uint16_t ADC1ConvertedValue[ADC_CH];
//extern const portTickType xTicksToWait;
////const portTickType xTicksToWait = 100 / portTICK_RATE_MS;  // ���� ����� ������ ��� ������ ��� ������ ������ ��\� ������� 
//extern xQueueHandle xQueue_sendText_t ,xQueue_port_struct;
extern xQueueHandle xQueue_sendText_t ;//������� ��� �������� ��������� ���������� � ���� ����� ETH
extern const portTickType xTicksToWait;
//extern uint32_t        period;
//extern uint32_t        pulse;
//extern TIM_OCInitTypeDef               TIM_OCInitStructure;

//---- Init treads -------------------------------------------------------------
/**
* @brief Function init task my_tasks_vConfigPort
* @param dev - Device structure parsing_html_device_pt
* @return - None
*/
//void my_tasks_vConfigPort_init(void * dev)
//{
//   xTaskCreate(my_tasks_vConfigPort, "cfgPort", configMINIMAL_STACK_SIZE * 2, dev ,ConfigPort_TASK_PRIO, NULL);
//}

//------------------------------------------------------------------------------
/**
* @brief Function config Port 
* @param dev - Device structure parsing_html_device_ptuint8_t i; // ���������� � �����
*/
//---------------------------------------------
//void my_tasks_vConfigPort (void *dev)
//{
//volatile struct PORT_struct *device ;
//uint8_t i; // ���������� � �����
////Ports_TypeDef port;
//
//  device = (volatile struct PORT_struct *)dev;
//  
////  for (;;)
////  {
////    // ������� �������� �� ����� key ���� ������� � �� ���� �� ���������� ����� �������� �������
////    for(int i=0; i < MAX_PORTs; i++){
////      if ((PORTs[i].key == device->key)&&(PORTs[i].mode == OUT_pin)){
////            PORTs[i].status = device->status;
////        if ((device->enable == enable)&&(device->status == On)){
////              if (PORTs[i].enable == enable){
////                port = getPort_name(PORTs[i].num_port);
////                make_pulse_init ((void*)&port);
//////                set_ports_PORTOn(getPort_name(PORTs[i].num_port));                
////              }
//////              set_ports_PORTOn(getPort_name(device->num_port));
////                port = getPort_name(device->num_port);
////                make_pulse_init ((void*)&port);              
////            } //end enable       
////        else {
////          if (PORTs[i].enable == enable){
////             port = getPort_name(PORTs[i].num_port);
////             make_pulse_init ((void*)&port);          
//////            set_ports_PORTOff(getPort_name(PORTs[i].num_port));
////          }
////             port = getPort_name(device->num_port);
////             make_pulse_init ((void*)&port);                        
//////             set_ports_PORTOff(getPort_name(device->num_port));
////        }  //end else enable
////      }  //end key
////    }  //end for PORTs
////    //set_ports_setAllValues(device);
////    vTaskDelete( NULL );
//////    vTaskDelay(200);
////  } //end for ;;
////------------------------------------------------------------------------------
//  for (;;)
//  {
//    // ������� �������� �� ����� key ���� ������� � �� ���� �� ���������� ����� �������� �������
//    for(i = 0; i < MAX_PORTs; i++){
//      if ((PORTs[i].key == device->key)&&(PORTs[i].mode == OUT_pin)){
//        PORTs[i].status = device->status;
////        port = getPort_name(device->num_port);
//        make_pulse_init ((void*)&device);                        
//        make_pulse_init ((void*)&PORTs[i]);                        
//      }  //end key
//    }  //end for PORTs
//    //set_ports_setAllValues(device);
//    vTaskDelete( NULL );
////    vTaskDelay(200);
//  } //end for 
//}
//------------------------------------------------------------------------------
/**
* @brief  - ������������� ������ ����������� ������� ������
* @param  - *pt ����������� PORT_struct
* @return - None
*/
//void my_tasks_vReceiveButton_init (void * pt)
//{
//  portBASE_TYPE xReturn;  //  TaskCreate Error
//  
//  char but[10] = "Button";
//  char port_str[3];
//  volatile struct PORT_struct *port ;  // �������� ���� ������������ � ������
//  Ports_TypeDef port_num;
//  
//  port = (volatile struct PORT_struct *)pt;  
//  port_num = getPort_name(port->num_port);
//  
//  sprintf(port_str, "%d", port_num);
//  strcat(but,port_str); // ������� ������ ����������� ������� ������ � ���������� ������� "Button1 ....Button.."
//  
//  xReturn = xTaskCreate(my_tasks_vReceiveButton, (signed char*)but, configMINIMAL_STACK_SIZE * 2  , pt ,INPUTS_TASK_PRIO, NULL);
//  if (xReturn != pdPASS){
//      printf ("Can't create Button Tasks, see errors %d in file 'projdefs.h' \n", xReturn);    
//      LED_alarm_On();
//  }
//  
//}

//------------------------------------------------------------------------------

/**
* @brief  - ������������� ������ ����������� ������� ������
* @param  - *pt ����������� PORT_struct
* @return - None
*/
//void my_tasks_vReceiveButton (void * pt)
//{  
//  portBASE_TYPE xStatus;  
//  // ��������� ����������
//  struct sendText_t send2host_gi;
//  uint8_t val;
//  char str[5];
//  char port_gi[3], stat_gi[2]; //, ena_gi[2], val_gi[4], key_gi[4], mode_gi[2];   
//  uint8_t i; // ���������� � �����
//  
//  volatile struct PORT_struct *port_st ;  // �������� ���� ������������ � ������
//  Ports_TypeDef port_num;
//  
//  port_st = (volatile struct PORT_struct *)pt;  
//  port_num = getPort_name(port_st->num_port); // ����� ����� ����� ��� ������
//
//  uint8_t stat1_pin = 0, stat2_pin = 0;
//  
//  for(;;){
/////*---------- Debug  -------------------------*/      
////#ifdef Debug_my
////  vTaskDelay(8000);
////  taskENTER_CRITICAL();  
////    size_t mem = xPortGetFreeHeapSize();  
////    printf ("Free Heap size = %d \n\n", mem);      
////  taskEXIT_CRITICAL();
////#endif  
/////*-------------------------------------------*/
//  
//  stat1_pin = GPIO_ReadInputDataBit(GPIOM_PORT[port_num],GPIOM_PIN[port_num]);    // ��������� ��������� ������
//  vTaskDelay(50); 
//  stat2_pin = GPIO_ReadInputDataBit(GPIOM_PORT[port_num],GPIOM_PIN[port_num]);    // ��������� ��������� ������
//    
//  if (stat1_pin == stat2_pin){
//    continue;
//  }
//  else{ 
//// sergey begin �������� �� ����� ��������� ��������� �������� �����, � ������� ������ ��������� ��������� �����    
//// ---- ������������� ��������� �������� ����� ------             
//          if ((stat1_pin == 0)&&(stat2_pin == 1)){
//            port_st->status = On;
//            printf("PORTs 'On'  Port %d \n", port_st->num_port);
//            //�������� �����            
//          }
//          if ((stat1_pin == 1)&& (stat2_pin == 0)){
//            //��������� �����      
//            port_st->status = Off;
//            printf("PORTs 'Off' Port  %d\n", port_st->num_port);
//          }
////----sergey end ---------    
//          for(i = MIN_PORT_OUT; i < MAX_PORT_OUT ; i++){  //��������� ��� ������ ���������� � ������� � ����� ���������
//            if (PORTs[i].key == port_st->key){  
//              
//              PORTs[i].status = port_st->status;
//                            
//              make_pulse_init ((void*)&PORTs[i]); 
//              
//              printf("Send pulse port %d \n", PORTs[i].num_port);
//                  
//                  val = PORTs[i].status;  //Status int to string
//                  sprintf(str, "%d", val);
//                  strcpy(stat_gi,str);
//                        
//                  val = PORTs[i].num_port;  //Port int to string
//                  sprintf(str, "%d", val);
//                  strcpy(port_gi,str);
//                            
//                    // String for Status   
//                  send2host_gi.cmd[0]=0; // �������� ��������� ������, ����� �������� ����� ������
//                  strcat(strcpy(send2host_gi.cmd, begin_pack), port_gi);
//                  strcat(send2host_gi.cmd, status_pack);
//                  strcat(send2host_gi.cmd, stat_gi);
//                  strcat(send2host_gi.cmd, end_pack);  // �������� ��� ����� ������
//                               
//                  send2host_gi.len = strlen(send2host_gi.cmd);                  //length of send_com
//
//                  xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_gi, xTicksToWait); // �������� ������ � �������
//                  if (xStatus != pdPASS){
//                    LED_alarm_On();
//                    printf("(my_tasks)Can't to write port status, error is %d ", xStatus);
//                  }
//                  
//                  vNetconnClient_init(NULL);         // ��������� ������ ������� ������ �� ethernet                                       
//                  vTaskDelay(100);
//                  printf("send message to host\n");
//                                                      
//            }  //end if key
//          }  //end for PORTs    
//     } //end else stat1 == stat2 
//  } // end for (;;)
//}               


/**------------------------------------------------------------------------------------
* @brief ������ ����������� � ��������.DS18b20+ � �������� �� �� ���� �� ETH
* @param
* @return void
* !�������� � ������� OW_Send �������� readStart ������� �� ���������� �������, �.�. �� ������ � RX �� ��� ������� (�������), �� � �������� - ��� ��� �� ���������� � ��������� ������ ������
*/
//void my_task_vGetTemperature (void * dev){
//  
//  struct sendText_t send2host_temper;  // ��������� ��� �������� ������ �� ����
//
//  struct owidbuf_t owidbuf[MAX_DS18]; // ���������� ��� �������� ���������� � ���� �������� ID, Temperature
//  
//  uint8_t buf[9]; //����� ��� ������ ������ �� �������
//  uint8_t idbuf[MAX_DS18][8]; //������ ��� �������� ID ��������
//  uint8_t i;
//  uint8_t j;
////  uint8_t err_ds18b20 = 0;
////  char id[64]; 
//  char id_tmp[5];  //���������   ��� ID � �����������
//  signed char integer_temp = 0; //����������� ����� �����    
//  signed char frac_temp;        //����������� ������� �����
////  int  result;             //�������� �����������
//  
//  portBASE_TYPE xStatus;
//
//  OW_Init();  //������������� ���� 1-wire
//  
//  uint8_t found = OW_Scan((uint8_t *)idbuf, MAX_DS18); // ���� ���������� �� ���� 1-wire
//  printf ("found %d devices\n", found);
//  
//  for (j = 0; j < found; j++){  //������� ������ ��� ������� � ������� ���������� �� ���� �� ��� ID
////    id[0] = 0;
//    owidbuf[j].id[0] = 0x55;  //������� ��������� �� ID �� ���� 1-wire
//    for(i = 0; i < 8; i++){
//      owidbuf[j].id[i+1] = idbuf[j][i];  //���������� � ������ ID
//      //������ � ���� ����� �������� ��� �������� ID �� ���� ������
////      sprintf(id_tmp, "%02x",idbuf[j][i]); //������� ������ � ID ������� ����� �������� �� ����
////      strcat(id,id_tmp);
////      strcat(id," ");
////      // ���������� ��� ID �� ���� ���������!!!!
////      send2host_temper.cmd[0]=0;
////      strcat(strcpy(send2host_temper.cmd, tp), id_tmp);
////      strcat(send2host_temper.cmd, ed);
////             
////      send2host_temper.len = strlen(send2host_temper.cmd);                  //length of send_com
////      xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_temper, xTicksToWait );
////                  if (xStatus != pdPASS){
////                    LED_alarm_On();
////                    printf("(my_tasks)Can't to write port status, error is %d ", xStatus);
////                  }
////      vNetconnClient_init(NULL);   
////      vTaskDelay(200);
//    }
//    owidbuf[j].id[9] = 0xbe;  //���������� ���������� ������ - ������� ������ ���� ��������� ������� DS18B20
//    
//    for (i=10; i < 15; i++){
//       owidbuf[j].id[i] = 0xff;  //����� 5 ��� ff � ������ ��� �� ����� ����� ���� ������� 5 ���� �� �������
//       //��� ���� ����� ������� ���� ����� ������� ���� ff
//    }    
//  }
//  
//  for (;;){  // � ����� �������� ����������� �� ���� �������� 
///*----------------------------------------------------------------------------*/    
//#ifdef Debug_my
//  taskENTER_CRITICAL();  
//    size_t mem = xPortGetFreeHeapSize();  
//    printf ("Free Heap size in temperature Task = %d \n\n", mem);      
//  taskEXIT_CRITICAL();
//#endif  
///*----------------------------------------------------------------------------*/
//  
//    OW_Send(OW_SEND_RESET, "\xcc\x44", 2, NULL, NULL, OW_NO_READ); // �������� ����������� �� ���� ��������
//    vTaskDelay(1000);  // ���� �����  1 ��� ���� ���������� ������� ��������� ����������� !!!�����������
//    
//    for (j = 0; j < found; j++){
//      //�������� 15 ����, ��������� 13 ����, �� � ����� ������, ������ � ��������, �.�. � ������ ����� 3 �����
//    OW_Send(OW_SEND_RESET, owidbuf[j].id, 15, buf, 13, 10); // ��������� �����������  
//    
///****************************************************************/ 
//
//      frac_temp    = buf[0] & 0x0f;                            //�������� ������� �����      
//      integer_temp = (buf[0]>>4) | ((buf[1] & 0x0f)<<4);      //�������� ����� ����� 
//     
//      //���� ����������� �������������
//      if(integer_temp < 0)
//      {
//        integer_temp = 0 - integer_temp - 1;
//        owidbuf[j].temp  = integer_temp * 10;                            //��������� ����� �����
//        frac_temp = frac_temp | 0xf0;
//        frac_temp = 0 - frac_temp ;                                 
//      }
//      //���� ����������� �������������
//      else      owidbuf[j].temp  = integer_temp * 10;                    //��������� ����� �����
//     
//      owidbuf[j].temp =  owidbuf[j].temp + ((frac_temp * 10)/16);                  //��������� ������� ����� //����� �������� �������� �����������, ����� ��� ����� ��������� �� 10.
//      printf("Temperature %d\n\n", owidbuf[j].temp);
//    } //end for ������� �������� �����������
////**********************************************************************/
////////-------------- ���������� ����������� �� ���� �������� � ����� ������� ��������
////  if (found == 3){
////    if (((owidbuf[0].temp - owidbuf[1].temp) < 10)&&((owidbuf[0].temp - owidbuf[1].temp)> -10)){  // <>10 �.�. � ��� ��� ����������� �������� �� 10 �.�. �� �� ��������� ������� ���� �������.
////      if (((owidbuf[1].temp - owidbuf[2].temp) < 10)&&((owidbuf[1].temp - owidbuf[2].temp)> -10)){
////      result = (owidbuf[0].temp + owidbuf[1].temp + owidbuf[2].temp)/3;
////      }
////      else{
////        result = (owidbuf[0].temp + owidbuf[1].temp)/2;
////        err_ds18b20 = 1;
////        // �������� alarm owidbuf[2].temp �� �������� ������ �����������
////      }
////    }
////    else if (((owidbuf[1].temp - owidbuf[2].temp) < 10)&&((owidbuf[1].temp - owidbuf[2].temp)> -10)){
////      result = (owidbuf[2].temp + owidbuf[1].temp)/2;
////       // �������� alarm owidbuf[0].temp �� �������� ������ �����������
////      err_ds18b20 = 1;
////    }
////    else{
////      result = (owidbuf[2].temp + owidbuf[0].temp)/2;
////       // �������� alarm owidbuf[1].temp �� �������� ������ �����������      
////      err_ds18b20 = 1;
////    }
////  }   
//  // �������� ���������� ��������� �� ���� ���������
//    
//    sprintf(id_tmp, "%d", owidbuf[j].temp); 
//    send2host_temper.cmd[0] = 0;
//    strcpy(send2host_temper.cmd, temper_pack);
//    strcat(send2host_temper.cmd, id_tmp);
//    strcat(send2host_temper.cmd, end_pack);
//           
//    send2host_temper.len = strlen(send2host_temper.cmd);                  //length of send_com
//
//    xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_temper, xTicksToWait );
//    if (xStatus != pdPASS){
//       LED_alarm_On();
//       printf("(my_tasks)Can't to write port temperature, error is %d ", xStatus);
//    }
//    
//    vNetconnClient_init(NULL);   
//    vTaskDelay(200);
//    
// //-----------   
////    //���� � ������� ���� ������ �� �������� ��������������
////    send2host_temper.cmd[0]=0;
////    if (err_ds18b20 == 1){
////    strcat(send2host_temper.cmd, "GET /objects/?object=tempSensor&op=m&m=errSensor&error=1");
////    err_ds18b20 = 0;
////    }
////    else{
////    strcat(send2host_temper.cmd, "GET /objects/?object=tempSensor&op=m&m=errSensor&error=0");  
////    }
////    strcat(send2host_temper.cmd, end_pack);          
////    send2host_temper.len = strlen(send2host_temper.cmd);                  //length of send_com
////    xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_temper, xTicksToWait );
////      if (xStatus != pdPASS){
////        LED_alarm_On();
////      }    
////    vNetconnClient_init(NULL);   
// //-----------     
//
//  vTaskDelay(10000);  //�������� ����� ��������� ���������� �����������
/////**************************************************************************/
//  }//end for(;;)
//}
//----------------****************************************
//---- Init treads wather ADC -------------------------------------------------------------
/**
* @brief Function init task wather_sensor_init
* @param dev - null
* @return - None
*/
//void vWather_sensor_init(void *dev)
//{
//portBASE_TYPE xReturn;  //  TaskCreate Error
//   xTaskCreate(vWather_sensor, "wather_sensor", configMINIMAL_STACK_SIZE * 2, NULL ,CLIENT_HTTP + 1, NULL);
//      if (xReturn != pdPASS){
//        printf ("See errors %d in file 'projdefs.h' \n", xReturn);    
//        LED_alarm_On();   
//      }

//}

//------------------------------------------------------------------------------
/**
* @brief Function read ADC for Wather Sensor 
* @param None
*/
//---------------------------------------------
//void vADC_sensor (void *dev)
//{  
////  int a;     // ������ ���������
//  uint8_t b; // ������ ���������
//  uint8_t i;
//  int read_value;
//  
//  ADC_SoftwareStartConv(ADC1);
//  
//  for (;;)
//  {
//    
//    for (i = 0; i < ADC_PORTn; i++){
//      switch (ADC_PORTs[i].name){
//      case 0:{   //GAS sensor
////        if (ADC1ConvertedValue[i] > 500){  // ��� ������ �����, ��� ������ ������������, ������� ��� �������� 300 ��� ������� MQ-4
////------ Warning GAS Atack -----------------------------------------------------
////------ Send to HOST messages !!!          
//          printf("GAS value = %d, sensor out = %d \n\n", ADC1ConvertedValue[i], i);
////       }
////       else{
//////------ GAS is normal ---------------------------------------------------------         
////          
////       }
//        
//        break;
//      }
////******************************************************************************      
//      case 1:{  //water sensor
//        read_value = ((3300 * ADC1ConvertedValue[i]) / 4095);
//        if ((0 < read_value)&&(read_value < 300)){
//          b = 1;  // �����
//        }
//        
//        if ((301 < read_value)&&(read_value < 700)){
//          b = 2;  // ������
//        }
//        
//        if (701 < read_value){
//          b = 3;   //����
//        }
////******************************************************************************
////------ Send to HOST value of sensor water ------------------------------------
//        
//       
//        break;
//      }
////******************************************************************************      
//      case 2:{
//        
//        break;
//      }
////******************************************************************************      
//      case 3:{
//        
//        break;
//      }
////******************************************************************************      
//      case 4:{
//        
//        break;
//      }
////******************************************************************************      
//      default:{
//        break;
//      }
////******************************************************************************        
//      } //end switch
//    }//end for     
//  vTaskDelay(10000);  
//  } //end for ;;
//}
//------------------------------------------------------------------------------
//void my_task_TaskList (void *dev){
//  for (;;){  
//  
//  portCHAR PAGE_BODY[512];
//  portCHAR pagehits[10] = {0};
//
//  memset(PAGE_BODY, 0,512);
//
//  /* Update the hit count */
//  strcat(PAGE_BODY, pagehits);
//  strcat((char *)PAGE_BODY, "Name          State  Priority  Stack   Num\n" );
//  strcat((char *)PAGE_BODY, "---------------------------------------------\n");
//    
//  /* The list of tasks and their status */
//  vTaskList((signed char *)(PAGE_BODY + strlen(PAGE_BODY)));
//  strcat((char *)PAGE_BODY, "---------------------------------------------\n");
//  strcat((char *)PAGE_BODY, "B : Blocked, R : Ready, D : Deleted, S : Suspended\n\n");
//  printf ("%s\n", PAGE_BODY);
//    
//  vTaskDelay (10000);
//  } //end for 
//  
//}
////------------------------------------------------------------------------------
//void vPWM (void *dev){
//  float i = 80;
//  init_ports_PORTPwm();  
//        /* TIM1 counter enable */
//        TIM_Cmd(TIM3, ENABLE);
//  
//        /* TIM1 Main Output Enable */
//        TIM_CtrlPWMOutputs(TIM3, ENABLE);  
//        
//        
////        
////    for (;;){         
////        TIM_OCInitStructure.TIM_Pulse = ((uint32_t) period / 1);        
////        TIM_OC1Init(TIM3, &TIM_OCInitStructure);
////             
////        vTaskDelay (1000);
////        
////        TIM_OCInitStructure.TIM_Pulse = ((uint32_t) period / 100);        
////        TIM_OC1Init(TIM3, &TIM_OCInitStructure);
////               
////        vTaskDelay (1000);
////    }
//        
//  for (;;){         
//i = 0;    
//    while (i < 99){
//      TIM_OCInitStructure.TIM_Pulse = ((uint32_t) i);  //12
//      TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//        i = i + 1;
//      vTaskDelay (10);      
//    }
//    
//    vTaskDelay (5000);
//    i = 99;
//    
//    while (i > 1){
//      TIM_OCInitStructure.TIM_Pulse = ((uint32_t)  i);  //12
//      TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//        i = i - 1;
//      vTaskDelay (10);      
//    }
//    vTaskDelay (5000);
//  } //end for 
//  
//}
//------------------------------------------------------------------------------
void vSend_HTTP (void *dev){
//   portBASE_TYPE xStatus;
//   struct sendText_t send2host_text;
//   //="GET /objects/?object=Port0&op=m&m=changedStatus&status=1 HTTP/1.1\r\nHost: 192.168.0.111\r\nConnection: close\r\n\r\n\0";;
//   char send_text[MAX_CMDSTR]="GET http://192.168.0.103/objects/?object=ToHost&op=m&m=getBoardStatus&Board=1&Inputs=0&Outputs=0 HTTP/1.1\r\nHost: 192.168.0.103\r\nConnection: close\r\n\r\n\0";
////                                  http://127.0.0.1:80/objects/?object=ToHost&op=m&m=changeStatus&
////   http://127.0.0.1/objects/?object=ToHost&op=m&m=changeStatus&status=855
//   send2host_text.cmd[0] = 0;
//   strcpy(send2host_text.cmd, send_text);
//   
//   send2host_text.len = strlen(send2host_text.cmd);
//   
//   xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_text, xTicksToWait );
//    if (xStatus != pdPASS){
//        printf ("Error\n");
//    }
//    vNetconnClient_init(NULL);  
//    vTaskDelete(NULL);
    
}
//------------------------------------------------------------------------------

/*
�������� ������ �� ����, ������ �������� ��������� � ��������� ������� �� ������� 
������� ������ ��� ��������

*/
//------------------------------------------------------------------------------
void vSend_HTTP_task (void *dev){
//   portBASE_TYPE xStatus;
//   struct sendStateRelays_t StateRelays;

//   struct sendText_t send2host_text;
   //="GET /objects/?object=Port0&op=m&m=changedStatus&status=1 HTTP/1.1\r\nHost: 192.168.0.111\r\nConnection: close\r\n\r\n\0";;
//   char send_text[MAX_CMDSTR]="GET http://192.168.0.103/objects/?object=ToHost&op=m&m=getBoardStatus&Board=   
//     1&Inputs=0&Outputs=0 HTTP/1.1\r\nHost: 192.168.0.103\r\nConnection: close\r\n\r\n\0";
       
//    char begin_text[80]="GET http://192.168.0.103/objects/?object=ToHost&op=m&m=getBoardStatus&Board=";        
//    char end_text[70] = "HTTP/1.1\r\nHost: 192.168.0.103\r\nConnection: close\r\n\r\n\0";
//    char send_text[200];  
      
//                                  http://127.0.0.1:80/objects/?object=ToHost&op=m&m=changeStatus&
//   http://127.0.0.1/objects/?object=ToHost&op=m&m=changeStatus&status=855
//   send2host_text.cmd[0] = 0;
//   strcpy(send_text, begin_text);
//   strcpy(send_text, begin_text);
//   
//   send2host_text.len = strlen(send2host_text.cmd);
//   
//   xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_text, xTicksToWait );
//    if (xStatus != pdPASS){
//        printf ("Error\n");
//    }
//    vNetconnClient_init(NULL);  
//    vTaskDelete(NULL);
    
}
//------------------------------------------------------------------------------


/**------------------------------------------------------------------------------------
* @brief ������ ����������� � ��������.DS18b20+ � �������� �� �� ���� �� ETH
* @param
* @return void
* !�������� � ������� OW_Send �������� readStart ������� �� ���������� �������, �.�. �� ������ � RX �� ��� ������� (�������), �� � �������� - ��� ��� �� ���������� � ��������� ������ ������
*/
void vGetTemperature (void * dev){
  
  struct sendText_t send_Data_HTTP;  // ��������� ��� �������� ������ �� ����

  struct owidbuf_t owidbuf[MAX_DS18]; // ���������� ��� �������� ���������� � ���� �������� ID, Temperature
  
  uint8_t buf[9]; //����� ��� ������ ������ �� �������
  uint8_t idbuf[MAX_DS18][8]; //������ ��� �������� ID ��������
  uint8_t i;
  uint8_t j;
//  char id_tmp[5];  //���������   ��� ID � �����������
  signed char integer_temp = 0; //����������� ����� �����    
  signed char frac_temp;        //����������� ������� �����
  char id_string[3];  
  char begin_text[]="GET http://192.168.0.103/objects/?object=ToHost&op=m&m=addDevice_1wire&ID=";        
  char end_text[] = " HTTP/1.1\r\nHost: 192.168.0.103\r\nConnection: close\r\n\r\n\0";  
  char temper[6] ="&TMP=";
  char tp[7];
  
  
  portBASE_TYPE xStatus;
  
  OW_Init();  //������������� ���� 1-wire
//for(;;){
  uint8_t found = OW_Scan((uint8_t *)idbuf, MAX_DS18); // ���� ���������� �� ���� 1-wire
//}
//------------------------------------------------------------------------------  
//uint8_t found;
  for (j = 0; j < found; j++){  //������� ������ ��� ������� � ������� ���������� �� ���� �� ��� ID
//    id[0] = 0;
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
  
  for (;;){  // � ����� �������� ����������� �� ���� �������� 
/*----------------------------------------------------------------------------*/    
#ifdef Debug_my
  taskENTER_CRITICAL();  
    size_t mem = xPortGetFreeHeapSize();  
    printf ("Free Heap size in temperature Task = %d \n\n", mem);      
  taskEXIT_CRITICAL();
#endif  
/*----------------------------------------------------------------------------*/
  
    OW_Send(OW_SEND_RESET, "\xcc\x44", 2, NULL, NULL, OW_NO_READ); // �������� ����������� �� ���� ��������
    vTaskDelay(1000);  // ���� �����  1 ��� ���� ���������� ������� ��������� ����������� !!!�����������
    
    for (j = 0; j < found; j++){
      //�������� 15 ����, ��������� 13 ����, �� � ����� ������, ������ � ��������, �.�. � ������ ����� 3 �����
    OW_Send(OW_SEND_RESET, owidbuf[j].id, 15, buf, 13, 10); // ��������� �����������  
    
/****************************************************************/ 

      frac_temp    = buf[0] & 0x0f;                            //�������� ������� �����      
      integer_temp = (buf[0]>>4) | ((buf[1] & 0x0f)<<4);      //�������� ����� ����� 
     
      //���� ����������� �������������
      if(integer_temp < 0)
      {
        integer_temp = 0 - integer_temp - 1;
        owidbuf[j].temp  = integer_temp * 10;                            //��������� ����� �����
        frac_temp = frac_temp | 0xf0;
        frac_temp = 0 - frac_temp ;                                 
      }
      //���� ����������� �������������
      else      owidbuf[j].temp  = integer_temp * 10;                    //��������� ����� �����
     
      owidbuf[j].temp =  owidbuf[j].temp + ((frac_temp * 10)/16);                  //��������� ������� ����� //����� �������� �������� �����������, ����� ��� ����� ��������� �� 10.
//      printf("Temperature %d\n\n", owidbuf[j].temp);
      
//------------------------------------------------------------------------------
//---- ���������� ����������� � ID ������� �� ���� ----------------------------- 

      send_Data_HTTP.cmd [0] = 0;    
      strcat(send_Data_HTTP.cmd, begin_text);    
      for (i = 0; i < 8; i++){
        sprintf(id_string, "%02x", idbuf[j][i]);
       
        strcat(send_Data_HTTP.cmd, id_string);
        if (i < 7){
          strcat(send_Data_HTTP.cmd, ":");
        }
      }
      strcat(send_Data_HTTP.cmd, temper);
      sprintf(tp, "%d", owidbuf[j].temp);
      strcat(send_Data_HTTP.cmd, tp);
      strcat(send_Data_HTTP.cmd, end_text);
      send_Data_HTTP.len = strlen(send_Data_HTTP.cmd);        
      xStatus = xQueueSendToBack( xQueue_sendText_t, &send_Data_HTTP, QUEUE_WRITE_TICKS_TO_WAIT );
//      vNetconnClient_init(NULL);  
//      vTaskDelay(200);         

//------------------------------------------------------------------------------      
      
      
    } //end for ������� �������� �����������
//**********************************************************************/
    // �������� � 5 ����� - ������ ������ �� ������
  vTaskDelay(60000);
  vTaskDelay(60000);
  vTaskDelay(60000);
  vTaskDelay(60000);
  vTaskDelay(60000);

  }  
}  
 //-----------  