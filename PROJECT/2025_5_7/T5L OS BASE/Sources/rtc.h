#ifndef __rtc_H__
#define __rtc_H__

#include "T5LOS8051.H"

//====================================================
#define  u8   unsigned char  
#define  s8     signed char
#define  u16  unsigned int 
#define  s16    signed int
#define  u32  unsigned long 
#define  s32    signed long
	
#define        RTCWrit        0x64      //RTC器件写地址
#define        ReadRTC        0x65      //RTC器件读地址
#define        BaudH	      0X03      //T5   15667200/9600=1632=0X0660    /115200=136=0X0088
#define        BaudL	      0XE4      //T5L  25804800/9600=2688=0X0A80	  /115200=224=0X03E4 1024-206438400/(64*115200)=0X03E4
#define        DTHD1          0X5A	    //帧头1
#define        DTHD2          0XA5	    //帧头2

void RTC_Start(void);


#endif