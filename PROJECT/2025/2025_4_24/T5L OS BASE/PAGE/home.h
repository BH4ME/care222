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

extern float L_m_D_V_S;				//���� ��λ:M
extern float Square;			//��� ��λ��M^2
extern float D;					//ֱ�� ��λ��M
extern float Volume;					//���ܽ���� ��λ��M^3

extern float change_P_uW_doing;	

extern float L_M;

extern u8 last_page;

extern u32 time_1s;
#endif