#ifndef _MODE_P_H_
#define _MODE_P_H_

/*****************************************************************************
系统库*/
#include "string.h"
#include "sys.h"
#include "suanfa.h"
/*****************************************************************************
自定义头文件*/


//extern bit ZIWAIXIAN_FINISH_FLAG; 
#define T_SHIJIAN 0x2600			/*****检测功率工作模式*******/
#define T_SHIJIAN_1 0x2700		/*****检测变量值a***********/
#define T_XIANSHI_1 0x2800		/*****剩余时间***********/
extern uint ZIWAIXIAN_FINISH_FLAG;
extern uint L_T_FLAG;
extern uint T_XIANSHI;
extern uint a;
extern uint First_shuju_FLAG;
extern void CLOSE_W_bianliangchongzhi(void);
extern double W_FINISH;
extern double W_Static;
extern void First_shuju(void);
extern uint T_P_First;
extern u8 PWM_ON[2];
extern u8 PWM_OFF[2];
extern uint Off_On_pwm_flag;
extern u8 MODE_2_Finish;
extern u16 dynamic_static_flag;
extern uint T_dynamic;
extern u8 MODE_2_RUN_Draw;
extern bit reset_mode2;
extern double P_FINISH;

extern bit yichu_T;
void Static_T_Current_mode2();
void zhiliao_SET();
void zhiliao_Choose_Mode_3();
void zhiliao_Choose_Mode_2();
	void zhiliao_Choose_Mode_1();
	void set_zhiliao_Init();
	void zhiliao_read_norflash();
#endif