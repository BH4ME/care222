#ifndef __MODE_4_H__
#define __MODE_4_H__
#include "sys.h"
#include "timer.h"
#include "handle.h"

void mode_4_Init();
void mode_4_reset();
void mode_4_run();
void mode4_isr();
void Static_T_Current_mode4();
void shoudong_read_norflash();
void set_shoudong_Init();


extern u8 MODE_4_Finish;
extern u8 mode4_maichong_on[2];
extern u8 mode4_maichong_off[2];
extern float mj_mode4;
extern bit reset_mode4;
extern bit start_mode4;



#endif