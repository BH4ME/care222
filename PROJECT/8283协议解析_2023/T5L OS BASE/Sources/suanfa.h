#ifndef __SUANFA_H_
#define _SUANFA_H_

/*****************************************************************************
系统库*/
#include "string.h"
#include "sys.h"
/*****************************************************************************
自定义头文件*/

#define  u8   unsigned char  
//#define  s8     signed char
#define  uint  unsigned int 
//#define  s16    signed int
#define  u32  unsigned long 

//extern bit ZIWAIXIAN_FINISH_FLAG; 
//#define T_SHIJIAN 0x2600			/*****检测功率工作模式*******/
//#define T_SHIJIAN_1 0x2700		/*****检测变量值a***********/
//#define T_XIANSHI_1 0x2800		/*****剩余时间***********/
//extern bit ZIWAIXIAN_FINISH_FLAG;
//extern uint L_T_FLAG;
//extern uint T_XIANSHI;
extern void Mode_p_work(void);
extern void Mode_s_work(void);
extern uint L_FINISH;
extern bit shuju_change;
void L_mm_TO_P_mw(uint L_mm_TO_P);
extern double change_P_mW;
extern double change_P_mW_First;
extern float change_P_mW_doing;
void LED_jiguang_Check();
 uint maichong_suanfa(uint T,u8 on,u8 off);
 extern u8 start_first_flag;
 extern u16  attend_sp_0;
extern u16 attend_sp_1 ;
extern u8 exit_sp[2];
void yichu_run();
void yichu_set();

uint mode4_maichong(uint T,uint on,uint off);
#endif