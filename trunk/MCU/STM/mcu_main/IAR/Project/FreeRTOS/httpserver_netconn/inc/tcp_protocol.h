#ifndef __TCP_PROTOCOL_H
#define __TCP_PROTOCOL_H

#define MAX_TCP_MSG_SIZE	128

/*********
 *
 * ������ ���������:
 * 					�����
 * CommandID		1
 * CommandPAR		1
 * DataLEN			2		(MSB ������ LSB ������)
 * Data				n
 * CRC8						algorithm?
 *
 * ��������� ����� ���� ������������ ��� ������, ��� ������ ��.
 * ��� ��������� ������������ ����� � ������ ��������� TCP ����������.
 *
 * ���������, ��������� � ������� ����:
 * ���� -> ��
 *
 * 1. GET RELAY STATE
 * 1.1 ���� ���������� ���������:
 *
 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= GET_RSTATE_REQ 			���� ������ ���������
 * 		LEN 	= n 			n - ���������� ����, ��������� ������� ���� �������
 * 		DAT 	= BoardIndex_i, BoardIndex_i+1 ...  - ������ ����
 * 		CRC8
 *
 * ��, ������ ��� ���������, ������ �������� ��������� ����������, ������ ������ � ���������
 * ����� ����� �� ����������� �������
 * 		int current_relay_state[NUM_RELAY_BOARDS];  (���� �� ��������� ��� ������ relay_thread)
 *
 * 1.2 �� ��������

 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= GET_RSTATE_ANS 			�� �������� �� ������ ������ ���������
 * 		LEN 	= 2*n 			n - ���������� ����, ��������� ������� ���� �������
 * 		DAT 	= {BoardIndex, RelayState}
 * 		CRC8
 *
 * 2. SET RELAY STATE
 * 2.1 ���� ���������� ���������:
 *
 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= SET_RSTATE_REQ 			���� ������������� ���������
 * 		LEN 	= 2*n 			n - ���������� ����, ��������� ������� ���� ����������
 * 		DAT 	= {BoardIndex, RelayState}
 * 		CRC8
 *
 * 2.2 �� ��������
 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= SET_RSTATE_ACK 			�� ��������, ��� ������� ��������� ��������� �������
 * 		LEN 	= 0x00 			0 ����� ������
 * 		CRC8
 *

//////////////////////////////////////////////////

*	SET RELAY PORT	- ���� ������������� ���������� ������ ������ �����
 *
	
 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= SET_RPORT_REQ 		���� ������������� ��������� ������ ������ �����
 * 		LEN 	= 3 			
 * 		DAT 	= {BoardIndex, RelayPort, PortStatus}
 * 		CRC8


* 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= SET_RPORT_ACK 		�� ��������, ��� ������� ��������� ��������� �������
 * 		LEN 	= 0x00 			0 ����� ������
 * 		CRC8
 

//////////////////////////////////////////////////////////////////////////////////
 * 	�� -> ����
 *
 * 	�� - ������
 *
 * 	3. REPORT RELAY STATE
 * 	3.1 �� ���������� ���������
 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= REPORT_RSTATE_REQ 			�� ��������, ��� ���������� ��������� ����� ��� ���������� ���� ����
 * 		LEN 	= 2*n 			n - ���������� ���� ���� � ����� ����������
 * 		DAT 	= {BoardIndex, RelayState} * n
 * 		CRC8
 *
 * 	3.2 ���� ��������
 *
	 * 	CID 	= CID_RELAY 			���������, ��������� � ������ ����
	 * 	CPAR 	= REPORT_RSTATE_ACK 			���� ��������, ��� ������� �������
	 * 	LEN 	= 0x00 			0 ����� ������
	 * 	CRC8
 *


 *
 * 	4. REPORT PORT STATUS
 * 	4.1 �� ���������� ���������
 * 		CID 	= CID_RELAY 			���������, ��������� � ������ ����
 * 		CPAR 	= REPORT_PORTSTA_REQ 			�� ��������, ��� ���������� ��������� ������ ����� 
 * 		LEN 	= 3 			                3 �����
 * 		DAT 	= {BoardIndex, PortNumber, PortStatus}

                        ��� BoardIndex   - 0-7
                            PortNumber   - 0-7 
                            PortStatus - 0,1
 *
 * 	4.2 ���� ��������
 *
	 * 	CID 	= CID_RELAY 			���������, ��������� � ������ ����
	 * 	CPAR 	= REPORT_PORTSTA_ACK 			���� ��������, ��� ������� �������
	 * 	LEN 	= 0x00 			0 ����� ������
	 * 	CRC8
 *
 */
//------------------------------------------------------------------------------
// Command IDs
#define CID_RELAY			0x01	// commands for working with relay boards
#define CID_CONFIG			0x02	// ���������������� ����������������
#define CID_TEMP            0x03    // ������ ������������� �������� � ���� 1-wire
#define CID_DMX             0x04    // ��������� �������� ��� DMX512 ���������
#define CID_ERROR			0xFF    // ��������� ������
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Command Parameter
//------------------------------------------------------------------------------
// Command Parameters for CID_RELAY
#define GET_RSTATE_REQ		0x01    // �������� ��������� ����� ����������
#define GET_RSTATE_ANS		0x02    // ����� �������
#define SET_RSTATE_REQ		0x03    // ���������� ��������� ����� ����� ����������
#define SET_RSTATE_ACK		0x04    // ��������� ����������

#define REPORT_RSTATE_REQ	0x05    //      
#define REPORT_RSTATE_ACK	0x06    //

#define REPORT_PORTSTA_REQ	0x07    //
#define REPORT_PORTSTA_ACK	0x08    //

#define SET_RPORT_REQ       0x09    // ���������� ������������ ��� � ����� �� ����� ����������
#define SET_RPORT_ACK       0x0a    // ��������� ����������

//------------------------------------------------------------------------------

// Command Parameters for CID_CONFIG
#define GET_CONFIG_T_REQ		0x01	// host is reading mcu_config_t
#define GET_CONFIG_T_ANS		0x02
#define SET_CONFIG_T_REQ		0x03
#define SET_CONFIG_T_ACK		0x04

//------------------------------------------------------------------------------
// Command Parameters for CID_TEMP
#define REPORT_TEMP_REQ		0x01    // �������� ����������� � �������
#define REPORT_TEMP_ACK		0x02    // ����������� �������

#define REPORT_ADD_TEMP_REQ	0x03    // �������� ID ������� �����������
#define REPORT_ADD_TEMP_ACK	0x04    // ID ��������

//------------------------------------------------------------------------------

// Command Parameters for CID_DMX
#define SET_VAL_DMX_REQ		0x01    // ���������� �������� ��� ������������� ���������� DMX
#define SET_VAL_DMX_ACK		0x02    // �������� �����������

#define SET_RGB_DMX_REQ		0x03    // ���������� RGB �������� ��� ������������� ���������� DMX
#define SET_RGB_DMX_ACK		0x04    // �������� RGB �����������
   
//------------------------------------------------------------------------------
// Common Command Parameters
#define MSG_ERROR		0xFF


// Error codes
#define ERROR_CID_NACK		1		// cmd id not recognized
#define ERROR_CPAR_NACK		2		// cmd parameter not recognized
#define ERROR_DATA		3


#define RELAY                   1
//----1-wire TEMPERATURE ---------------
#define TEMP_SENSOR             2   //set param relay_port_status_t ->device

#define ADDDEV_1WIRE            1
#define SENDTEMP_1WIRE          2   //set param relay_port_status_t ->device
//------------------------------------------------------------------------------   

//-----�������� �������� -------------------------------------------------------
// power control thread output data
// client_netconn_thread input data
struct relay_port_status_t {
  uint8_t BoardIndex;
  uint8_t PortNumber;  
  uint8_t PortStatus;  
}; 
//------------------------------------------------------------

// power control thread input data
struct  relay_data_t {
  uint8_t MsgType; 	// 0 - set state	1 - set port status
  uint8_t byte1;	// BoardIndex		BoardIndex;
  uint8_t byte2; 	// RelayState		PortNumber;  
  uint8_t byte3;	// dummy		PortStatus;  
} ; 
//------------------------------------------------------------
//--��������� ��� �������� ������ ������� � �����������
typedef struct  {
  uint8_t id[8];  
  uint8_t int_path;  
  uint8_t float_path;  
  uint8_t mode; // search device or send temperature
} temp_status_t; 
//------------------------------------------------------------


#endif // __TCP_PROTOCOL_H
