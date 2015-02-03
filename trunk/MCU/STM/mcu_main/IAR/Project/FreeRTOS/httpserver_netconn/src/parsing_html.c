#include "parsing_html.h"
//char *store_str;
char store_str[10];	
//------------------------------------------------------------------------------
/**
* @brief Function parsing html to get all device properties
* @param input_str - input string for parsing, key_str - key string for find text in input string
* @return int - begin char there start key string
*/
int parsing_html_parsingGET(char *input_str, char *key_str){
	short size = strlen(key_str);
        uint8_t error = 0;
	char *ptr_begin;
	char *ptr_end;
	char local_str[10];
	uint8_t i;
	uint8_t length;
	// ������ ������ �������� ������
	ptr_begin = strstr(input_str, key_str);
	// ������ ptr ��������� �� ������ ����� �� ������� ������
        if (ptr_begin !=0) {
          ptr_begin += size + 1; // ����������  � ����� 1

          // ������ �����
          ptr_end = ptr_begin;

          while((*ptr_end != '&')&&(*ptr_end != ' '))
                  ptr_end++;

          // ������ ��������� ����� ������� ��������

          length = ptr_end - ptr_begin;

          // �������� ������� �������� � ��������� �������
          for(i = 0; i < length; i++)
                  local_str[i] = *ptr_begin++;

          // ������� ������������� ����
          local_str[length] = '\0';

	// �������� ��������� ������ �������
// store_str - ��������� �� ������, ���� ��������� ������� ��������
          strncpy(store_str,local_str,10); // ��� 50  - ��� ������������ ����� ��� �����������
        }
        else{
          error = 1;          
        }
    return error;
}
//-------------------------------------------------------------------------------
