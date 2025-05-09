/******************************************************************************
* 文 件 名   : main.c
* 版 本 号   : V1.0
* 作    者   : pinot
* 生成日期   : 2019年11月01日
* 功能描述   : 主函数，外设和参数初始化，主循环中主要功能函数入口。
* 修改历史   :
* 日    期   :
* 作    者   :
* 修改内容   :
******************************************************************************/

/*****************************************************************************
系统库*/
//short 和 int 都是2个字节 long 是4个字节

/*****************************************************************************
自定义头文件*/
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
主函数*/
//u8 aa[2];
//unsigned long  bb=55; 4个
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
	start_win_Init();	//画图初始化
//	JumpPage(0X00);
  while(1)
  {
	//aa[1]=sizeof(bb);
		//	WriteDGUS(0x5100,(uint8_t *)&(aa),sizeof(aa));
	 	//WriteDGUS(TOF_Address, (uint8_t *)&L,sizeof(L));
		//JumpPage(0x16);
    HandleProc();	//包含串口解析
		ReadCurrentPage_run();	//检测当前页面	
		
		Clock();		//时钟RTC	500ms
	
	
  }
}
