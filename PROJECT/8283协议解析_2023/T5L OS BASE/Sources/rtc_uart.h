#ifndef __RTC_UART_H__
#define __RTC_UART_H__

#include "T5LOS8051.h"
#include "rtc_sys.h"
#include "crc16.h"


extern u8     xdata        T_O2;		  //��2��ʱ������
extern u8     xdata        T_O3;		  //��3��ʱ������
extern u8     xdata        T_O4;		  //��4��ʱ������
extern u8     xdata        T_O5;		  //��5��ʱ������


extern bit    Response_flog;  //Ӧ����
extern bit    Auto_data_upload;//�����Զ��ϴ���
extern bit    Crc_check_flog;//CrcУ����

void  uart_init();
void  Sw_Data_Send ();
void  uart_frame_deal();



#endif