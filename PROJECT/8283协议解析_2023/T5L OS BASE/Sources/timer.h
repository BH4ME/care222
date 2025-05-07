/******************************************************************************
�� �� ��   : timer.h
�� �� ��   : V1.0
��    ��   : pinot
��������   : 2019��11��27��
��������   : ��ʱ��Ӧ��
�޸���ʷ   :
��    ��   :
��    ��   :
�޸�����   :
******************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

/*****************************************************************************
ϵͳ��*/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "T5LOS8051.H"
#include "sys.h"
#include "sys_params.h"
#include "buttom.h"
/*****************************************************************************
�궨�����*/
extern uint norflash_Close_UV_Flag;
extern uint16_t Wait_Count_2;
extern uint norflash_Open_UV_Flag;
extern uint norflash_Stop_UV_Flag;
//extern uint Work_Time;
extern uint Draw_1s;
extern uint Draw_1ms;
extern uint Screen_Time_500ms;
extern uint mode_4_500ms;
extern uint16_t T_1S_S;
/*****************************************************************************
���⺯������*/

void InitTimer(void);
extern void TIM0_isr(void);
extern void TIM0_suanfa_isr(void);
extern void TIM0_1S_isr(void);
extern void TIM0_1S_S_isr(void);
extern void T_100MS_ADC_isr(void);
extern void mode_3_isr();
extern void 		t2_isr();
extern void 		t1_isr();
#endif