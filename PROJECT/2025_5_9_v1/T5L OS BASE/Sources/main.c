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
//short �� int ����2���ֽ� long ��4���ֽ�

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "T5LOS8051.H"
#include "sys.h"
#include "handle.h"
#include "i2c.h"
#include "home.h"
#include "start_win.h"


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
//u8 aa[2];
//unsigned long  bb=55; 4��
//unsigned short  bb=55;
void main(void)
{
  InitCPU();
  //InitUartSend();
	//InitUart();
	//JumpPage(0x14);
	Init_Rtc();
	UV=0;
	PageLock_Init();
	start_win_Init();	//��ͼ��ʼ��
//	JumpPage(0X00);
  while(1)
  {
	//aa[1]=sizeof(bb);
		//	WriteDGUS(0x5100,(uint8_t *)&(aa),sizeof(aa));
	 	//WriteDGUS(TOF_Address, (uint8_t *)&L,sizeof(L));
		//JumpPage(0x16);
    HandleProc();	//�������ڽ���
		ReadCurrentPage_run();	//��⵱ǰҳ��	
		
		Clock();		//ʱ��RTC	500ms
	
	
  }
}
