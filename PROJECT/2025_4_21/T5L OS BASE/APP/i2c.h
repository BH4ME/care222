#ifndef __I2C_H__
#define __I2C_H__

#include <T5LOS8051.H>
#include <string.h>
//#include "rtc_sys.h"
//#include <stdint.h>
#define	      SELECT_RTC_TYPE			1		            //RX8130=1;SD2058=2 ;?TRTC=0


void    Clock();
void RTC_SET_IN();
void  Init_Rtc();




extern uint16_t	SysTick_RTC;

#endif