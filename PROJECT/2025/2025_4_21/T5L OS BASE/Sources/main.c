/******************************************************************************
* �� �� ��   : main.c
* �� �� ��   : V1.0
* ��    ��   : pinot
* ��������   : 2019��11��01��
* ��������   : ������������Ͳ�����ʼ������ѭ������Ҫ���ܺ�����ڡ�
* �޸���ʷ   :
* ��    ��   :
* ��    ��   :
* �޸�����   :
******************************************************************************/

/*****************************************************************************
ϵͳ��*/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "T5LOS8051.H"
#include "sys.h"
#include "handle.h"
#include "i2c.h"


#define 	S_LED_0_Address	0x2000	
#define 	S_SPK_Address	0xA0		
//uint8_t code CLED_R_1[] = "R ON";
//uint8_t code CLED_R_0[] = "R OFF";
//uint8_t code CLED_G_1[] = "G ON";
//uint8_t code CLED_G_0[] = "G OFF";
//uint8_t code CLED_B_1[] = "B ON";
//uint8_t code CLED_B_0[] = "B OFF";
//uint16_t LED_Buff[4]={0x0000,0,0x00,0x00};
//uint16_t SPK_1S[4]={0x007d,0X00ff};




//Uart2SendStr(CLED_B_0,sizeof(CLED_B_0));
			
/*****************************************************************************
������*/
void main(void)
{
  InitCPU();
  //InitUartSend();
	//InitUart();
	Init_Rtc();
	
	
  while(1)
  {

	 	//WriteDGUS(TOF_Address, (uint8_t *)&L,sizeof(L));
		//JumpPage(0x16);
     HandleProc();
		
  }
}
