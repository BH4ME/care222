#ifndef __HOME_H__
#define __HOME_H__
#include "sys.h"

extern uint8_t CurrentPageNum[2];
extern void ReadCurrentPage_run();
extern void L_mm_TO_P_uw(u16 L_mm_TO_P,u8 current_page);

extern float Tanx;
extern float pai;
extern float Tan_zhongxin;
extern float zhongxin_35mm;	//=0.0175m/tan2.5

extern float L_m_D_V_S;				//距离 单位:M
extern float Square;			//面积 单位：M^2
extern float D;					//直径 单位：M
extern float Volume;					//气溶胶体积 单位：M^3

extern double change_P_uW_doing;	

extern float L_M;

extern u8 last_page;

extern u32 time_100ms;

extern float mJ_doing;	//已照射的mj

typedef void (*Callback_u16)(u16);
extern void QRJ_ZX__QY_RUN(Callback_u16 callback_u16);

extern u16 baifenbi_T;

extern u32 time_doing;

//extern u32 uJ_doing;

void PageLock_Init();
void Lock_current_page();
void Unlock_current_page();

#endif