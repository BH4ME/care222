#ifndef __ZX_H__
#define __ZX_H__
#include "sys.h"
//#include "tof.h"


//extern void QRJ_run();
//extern void QRJ_prepare();
extern float zhongxin_j;
extern void ZX_mm_to_s(u16 L_T);
extern void ZX_run();
extern void ZX_prepare();
extern void ZX_Reset()	;
extern void ZX_Home();
void ZX_INIT();

#endif