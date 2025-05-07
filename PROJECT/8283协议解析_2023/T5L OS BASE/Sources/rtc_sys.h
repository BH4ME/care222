#ifndef __RTC_SYS_H__
#define __RTC_SYS_H__

#include "T5LOS8051.h"

extern u16    xdata        SysTick_RTC;
extern bit                 RTC_Flog;



//void Sys_Cpu_Init();

void Write_Dgus(u16 Dgus_Addr,u16 Val);
u16 Read_Dgus(u16 Dgus_Addr);
void write_dgus_vp(unsigned int addr,unsigned char *buf,unsigned int len);
void read_dgus_vp(unsigned int addr,unsigned char *buf,unsigned int len);
void Write_Dgusii_Vp_byChar(unsigned int addr,unsigned char *buf,unsigned int len);
#endif