/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tcp_server_netconn.h"
#include "tcp_protocol.h"
#include "flash.h"
#include "dmx512.h"

//---- ���������� ���������� ---------------------------------------------------
extern xQueueHandle power_control_queue ;
extern xSemaphoreHandle xUSARTSemaphore;
extern uint8_t	current_relay_state[NUM_RELAY_BOARDS];
extern int CONNECTED_BOARD_KRM[8];
extern mcu_config_t	mcu_config;
extern struct dmx512_t dmx, dmx_old;
//------------------------------------------------------------------------------ 

void process_buffer(struct netconn *conn, void *buf, uint16_t buflen)
{
	uint8_t *lbuf;			// local buffer ptr
	uint8_t	n;			// number of boards
        uint8_t adr;                    //address for DMX

	uint8_t wr_buf[MAX_TCP_MSG_SIZE];	// write buffer to send to host
	uint8_t *tmp_ptr;

	struct relay_data_t	relay_data; 


	int i;

	lbuf = (uint8_t *)buf;
//-------------- CID_RELAY ----------------------------------------------------
	if (lbuf[0] == CID_RELAY) {

		if (lbuf[1] == GET_RSTATE_REQ) {

			// get number of boards to address
			// in this case it equals to number of payload bytes
			// skip lbuf[2] - MSB of length
			n = lbuf[3];

//			if (n > NUM_RELAY_BOARDS) {			// data field error
                        if (n > NUM_RELAY_BOARDS) {			// data field error

				send_simple_answer(conn, CID_RELAY, MSG_ERROR, 1, ERROR_DATA);

			} else {

				//prepare write buffer
				wr_buf[0] = CID_RELAY;
				wr_buf[1] = GET_RSTATE_ANS;
				wr_buf[2] = 0; 		// MSB of Length
				wr_buf[3] = 2*n;	// LSB of length

				for(i = 0; i<n; i++) {
					wr_buf[2*i + 4] = lbuf[i+4];
					wr_buf[2*i + 5] = current_relay_state[lbuf[i+4]];
				}

				netconn_write(conn,wr_buf,2*n+4,NETCONN_COPY);
			}

		} 	// lbuf[1] == GET_RSTATE_REQ
//-------------- SET PORT STATE ALL ----------------------------------------------------
		else if (lbuf[1] == SET_RSTATE_REQ) {

			// get number of boards to address
			// in this case it equals to half of number of payload bytes
			// skip lbuf[2] - MSB of length
			n = (lbuf[3] >> 1);

//			if (n > NUM_RELAY_BOARDS) {			// data field error
			if (n > NUM_RELAY_BOARDS) {			// data field error                          

				send_simple_answer(conn, CID_RELAY, MSG_ERROR, 1, ERROR_DATA);

			} else {
				
			 	relay_data.MsgType = 0; 

				for (i=0; i<n; i++) {
					relay_data.byte1 = lbuf[2*i+4];		// BoardIndex
					relay_data.byte2 = lbuf[2*i+5]; 	// RelayState
					xQueueSend(power_control_queue, &relay_data, QUEUE_WRITE_TICKS_TO_WAIT);
				}

				send_simple_answer(conn, CID_RELAY, SET_RSTATE_ACK, 0, 0);

			}

		}	// lbuf[1] == SET_RSTATE_REQ
//-------------- SET PORT STATE PINS ----------------------------------------------------                
                else if (lbuf[1] == SET_RPORT_REQ) {
			
		  	// assuming that host is updating only one relay port at a time
		  
			if (lbuf[3] != 3) {			// data field error                          
				send_simple_answer(conn, CID_RELAY, MSG_ERROR, 1, ERROR_DATA);
			} else {
				
			  	relay_data.MsgType = 1; 
				relay_data.byte1 = lbuf[4];	// BoardIndex;
				relay_data.byte2 = lbuf[5];	// PortNumber
				relay_data.byte3 = lbuf[6];	// PortStatus
				
				
				xQueueSend(power_control_queue, &relay_data, QUEUE_WRITE_TICKS_TO_WAIT);

				send_simple_answer(conn, CID_RELAY, SET_RPORT_ACK, 0, 0);
			}                                    
                }
//------------------------------------------------------------------------------------------                
		else {

			send_simple_answer(conn, CID_RELAY, MSG_ERROR, 1, ERROR_CPAR_NACK);

		}

	} // lbuf[0] == CID_RELAY
//-------------- CID_CONFIG ----------------------------------------------------
	else if (lbuf[0] == CID_CONFIG) {

		if (lbuf[1] == GET_CONFIG_T_REQ) {		// host reads mcu_config

			//prepare write buffer
			wr_buf[0] = CID_CONFIG;
			wr_buf[1] = GET_CONFIG_T_ANS;
			wr_buf[2] = 0; 		// MSB of Length
			wr_buf[3] = sizeof(mcu_config_t);	// LSB of length

			tmp_ptr = (uint8_t *)(&mcu_config);
			for(i = 0; i<sizeof(mcu_config_t); i++) {
				wr_buf[i + 4] = *tmp_ptr++;
			}

			netconn_write(conn, wr_buf, 4 + sizeof(mcu_config_t), NETCONN_COPY);


		}

		else if (lbuf[1] == SET_CONFIG_T_REQ) {		// host writes mcu_config

			// check data length
			// skip lbuf[2] - MSB of length

			if (lbuf[3] != sizeof(mcu_config_t)) {

				send_simple_answer(conn, CID_CONFIG, MSG_ERROR, 1, ERROR_DATA);

			} else {

				// save config ro ram
				tmp_ptr = (uint8_t *)(&mcu_config);
				for(i = 0; i<sizeof(mcu_config_t); i++) {
					*tmp_ptr++ = lbuf[4+i];
				}

				// write config to flash
				write_config_to_flash(&mcu_config);

				send_simple_answer(conn, CID_CONFIG, SET_CONFIG_T_ACK, 0, 0);
			}


		}

		else {

			send_simple_answer(conn, CID_CONFIG, MSG_ERROR, 1, ERROR_CPAR_NACK);

		}

	} // lbuf[0] == CID_TEMP
        else if (lbuf[0]==CID_TEMP){
          
        }

//------------ //lbuf[0] == CID_DMX	
        else if (lbuf[0]==CID_DMX){
          if (lbuf[1] == SET_VAL_DMX_REQ) {     // ���������� �������� �� ��������� DMX ����������      
            adr = lbuf[4];
            if (adr != 0){
              if (dmx_old.data[adr] < lbuf[5]){ // ���������� �������� DMX ����������
                dmx.direction = 1;   
              }
              else{                           
                dmx.direction = 0;          // ���������� �������� DMX ����������
              }
              dmx.adr = adr;                // ��������� ����� ����������� DNX ����������
//              dmx_old.data[adr] = dmx.data[adr]; // ��������� ������ �������� ����������� DNX ����������                 
              dmx.data[adr] = lbuf[5];      // ������������� ����� �������� DNX ����������
              dmx.smooth_step = lbuf[6];    // ������������� ��� ���������
              dmx.smooth_time = lbuf[7];    // ������������� ����� ����� ���������� ������
              dmx.flag_send = 1;            // ���������� ��������� ������
            }
            send_simple_answer(conn, CID_DMX, SET_VAL_DMX_ACK, 0, 0);
          }
          else if (lbuf[1] == SET_RGB_DMX_REQ) {   // ���������� �������� �� RGB DMX ����������
            adr = lbuf[4];
            if (adr != 0){
              dmx.data[adr] = lbuf[5];  //R
              dmx.data[adr + 1] = lbuf[6];  //G
              dmx.data[adr + 2] = lbuf[7];  //B
              dmx.flag_send = 1;
            }
            send_simple_answer(conn, CID_DMX, SET_RGB_DMX_ACK, 0, 0);
            
          }
        }
        
        else {

		send_simple_answer(conn, CID_ERROR, MSG_ERROR, 1, ERROR_CID_NACK);
	}


}



/**
  * @brief serve tcp connection  
  * @param conn: pointer on connection structure 
  * @retval None
  */
void tcp_server_serve(struct netconn *conn)
{
  portBASE_TYPE xStatus;
  struct netbuf *inbuf;
  
  void *buf;
  uint16_t buflen;

  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
  inbuf = netconn_recv(conn);
  
    
  //*****************************************************************************
  if (inbuf != NULL)
  {
    if (netconn_err(conn) == ERR_OK) 
    {
      netbuf_data(inbuf, (void**)&buf, &buflen);

      process_buffer(conn, buf,buflen);

  
    }
  }
  /* Close the connection (server closes in HTTP) */
  netconn_close(conn);

  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
}



/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void server_netconn_thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err;
  
  struct ip_addr ipaddr;

  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);

  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
    /* Bind to port TCP_SERVER_PORT  */
    err = netconn_bind(conn, &ipaddr, TCP_SERVER_PORT);
    
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
      while(1) 
      {
        // accept any icoming connection
    	// this will block thread until new connection is incoming
        newconn = netconn_accept(conn);
      
        /* serve connection */
        tcp_server_serve(newconn);
      
        /* delete connection */
        netconn_delete(newconn);
      }
    }
    else
    {
    	msg_dbg_tsk(("can not bind netconn\n\r "));
    }
  }
  else
  {
	  msg_dbg_tsk(("can not create netconn \n\r "));
  }

  while(1);
}

/**
  * @brief  Initialize the HTTP server (start its thread) 
  * @param  none
  * @retval None
  */
void tcp_server_netconn_init()
{
  sys_thread_new("NetconnServer", server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCP_SERVER_THREAD_PRIO);
}



/**
  * @brief  Send short answer to host with one byte data length
  * @param  none
  * @retval None
  */
void send_simple_answer(struct netconn *conn, uint8_t cid, uint8_t cpar, uint8_t len, uint8_t dat)
{
	uint8_t	wr_buf[5];
	wr_buf[0] = cid;
	wr_buf[1] = cpar;

	if (len == 0) {
		wr_buf[2] = 0;
		wr_buf[3] = 0;
		netconn_write(conn,wr_buf,4,NETCONN_COPY);
	} else if (len == 1) {
		wr_buf[2] = 0;
		wr_buf[3] = 1;
		wr_buf[4] = dat;
		netconn_write(conn,wr_buf,5,NETCONN_COPY);
	}

}
