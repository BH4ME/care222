#ifndef __QRJ_H__
#define __QRJ_H__
#include "sys.h"
#include "tof.h"
#include "handle.h"
extern u8 current_st;
extern void QRJ_run();
extern void QRJ_prepare();
extern void QRJ_Reset()	;
extern void QRJ_Home();

extern void QRJ_mm_to_s(u16 L_T);	

extern double TIME_static;

extern float qirongjiao_j;

extern u16 icon[4];
#endif