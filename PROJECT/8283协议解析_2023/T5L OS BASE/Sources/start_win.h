#ifndef __START_WIN_H__
#define __START_WIN_H__
#include "sys.h"


#define chart_Y_4_Adress 0x4140
#define chart_Y_5_Adress 0x4150
#define chart_X_1_Adress 0x4200
#define chart_X_2_Adress 0x4210

/*********************************SP指针地址********************/
#define P_D_SP_Adress 0XFF10
#define W_SP_Adress 0xff20
extern uint First_Draw_Flag;
extern uint Draw_W_Flag;
extern double T_First;
extern uint T_doing;
extern float W_mJ_doing;
extern 	float Square;	
extern	float D;
extern uint T_All;
extern 	float Volume;
extern u8 Page_Lasttime;
extern u8 Page_s[2];

void sin_chart_draw();
void Verb_Draw();
void zhexian_Draw();
void sys_write_chart_zhexian_Two(u16 x1,u16 y1,u16 x2,u16 y2);
//函数申明
void start_win_init(void);
void Draw_Reset();
void Lock_current(u8 id_1,u8 daima_1);
void Unlock_current(u8 id_2,u8 daima_2);
/*****************************进度条********************************/
void Draw_juxing_Init();
void Rest_Draw_juxing();
void S_D_Surfaces_All();
void Unlock_current_page();
void Lock_current_page();
void Unlock_current_page_zengliang(u8 id_1,u8 daima_1,u8 id_2,u8 daima_2);
void Lock_current_page_zengliang(u8 id_1,u8 daima_1,u8 id_2,u8 daima_2);
//void start_win_btn_click_handler(u16 btn_val);
//void start_win_scan(void);
void Draw_Rest_2();
/*****************第三条曲线最后汇总曲线*******************/
void dynamic_draw_chart_Init();
#endif

