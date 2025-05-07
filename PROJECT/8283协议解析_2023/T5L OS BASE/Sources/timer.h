/******************************************************************************
文 件 名   : timer.h
版 本 号   : V1.0
作    者   : pinot
生成日期   : 2019年11月27日
功能描述   : 定时器应用
修改历史   :
日    期   :
作    者   :
修改内容   :
******************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

/*****************************************************************************
系统库*/

/*****************************************************************************
自定义头文件*/
#include "T5LOS8051.H"
#include "sys.h"
#include "sys_params.h"
#include "buttom.h"
/*****************************************************************************
宏定义变量*/
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
对外函数声明*/

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