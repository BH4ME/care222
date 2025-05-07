#ifndef __MODE_3_H_
#define _MODE_3_H_
#include "string.h"
#include "sys.h"
#include "sys_params.h"
#include "norflash.h"
#include "mode_p.h"
#include "start_win.h"
extern u8 chengxu_run_flag;	//1¿ªÆô 0¹Ø±Õ
extern u8 UE;
extern u8 rtc_chengxu[8];
void chengxu_only_first();	
void chengxu_Updae();
void chengxu_Init();
void chengxu_Run();





#endif