#ifndef __PARSING_HTML_H__
#define __PARSING_HTML_H__

//#include "init_ports.h"
#include "string.h"
//#include "ihome_board.h"
//#include "httpserver-netconn.h"
#include "stm322xg_eval.h"
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
//------------------------------------------------------
///**
//* @brief Deviev Structure content all device proprties
//*/
//  struct parsing_html_device_pt {
//        Ports_TypeDef Port;  // Using port
//	int value;  // value input or output 
//        int status;   // 0 - output, 1 - input
//	int mode;  // mode input (interrupts) or output (switch or pwm)
//        int key; // ���� ��� ������ ����� � ���������� ������ (���� ����� ����������, �� ���������� � ���� ��� �����)
//        int ena_int; // ���������� ����������
//        char *object_p[300]; // ������ ���� GET ������� ������� � �������� � majordomo � �������� ��� �������� 
//        int action ; // ���� ���� ������ ������, �� � ����������� ���������� ���������� ���� ���� � ��������� �����
//                     // � ������� ���������� ��� ����� � ���� ���� action=1 ����� ������ ���� � ��������� �������� 
//        int noPort;  // if = 0 then NULL Port is absent
//  };
//----------------------------------------------------------
/**
* @brief Heaader Function parsing html to get all device properties
* @param input_str - input string for parsing, key_str - key string for find text in input string
* @return int - begin char there start key string
*/
int parsing_html_parsingGET(char *input_str, char *key_str);

#endif /* __PARSING_HTML_H__ */
