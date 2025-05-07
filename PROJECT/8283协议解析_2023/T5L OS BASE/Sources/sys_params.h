#ifndef __SYS_PARAMS_H__
#define __SYS_PARAMS_H__
#include "sys.h"
#include "timer.h"

//#define SYS_PARAMS_ADDR				0
#define SYS_PARAMS_FLAG_OK		0x55555555
#define RTC_Y_M 0X9D
#define RTC_D_H 0X9E
#define RTC_M_S 0X9F
#define RTC_Adress 0x10
#define NOR_FLASH_BUFF_Reserve 0x0		/***0x00-0x04****/
#define Dgus_Buff_Reserve 0xff20			/***0xff00-0xff10***/

#define Nor_Next_Adress 0				/****0x00-0x03**此地址为关机前的地址******/
#define Nor_Guanji_Adress 0x04				/****0x04-0x07**此地址为关机RTC地址*0x07-0xa0**为关机地址***/
#define Nor_24_Adress 0x000028

#define Nor_Mode 0x22
#define Nor_Last_Mode 0x24
#define Nor_zhiliao_set_Ad 0x44000	//设置界面中的治疗预设10byt
#define Nor_zhiliao_set_initial_1_Ad	0x44100	//设置界面中 治疗模式快捷键 的源码地址 未经过算法处理
#define Nor_zhiliao_set_initial_2_Ad	0x44400	
#define Nor_zhiliao_set_initial_3_Ad	0x44300	
#define Nor_chengxu_Ad 0x46000	//程序化消毒分配36字的存储空间
#define Nor_chengxu_week_Ad 0x47000	//程序化消毒星期存储空间
//#define Nor_chengxu_week_Ad 0x48000	//程序化消毒星期存储空间
#define Nor_set_light_Ad 0x45000	//屏幕亮度、待机亮度、待机时间存储地址
#define Nor_set_sudoopen_Ad	0x45100	//开机自动启动	数据经过处理后
#define Nor_set_sudoopen_initial_Ad	0x45150	//开机自动启动的初始数据 没经过算法处理
#define Nor_zhiliao_user_Ad 0x45160	//治疗模式界面下 用户自主设置的mj脉冲onoff数值 没有按下快捷键
#define Nor_shoudong_user_Ad 0x4520	//手动界面下 用户自主设置的分钟脉冲onoff数值 没有按下快捷键
#define Nor_P_set 0x48000 //功率自适应 

void Init_Adress(void);
void norflash_Write_Open_UV(void);
void norflash_Write_guanji(void);
void norflash_Write_Close_UV(void);
void norflash_Write_Stop_UV(void);
void Uart4_Open(void);
void rtc_write(void);
void set_page_write();
void Screen_Init();
void Screen_review();
void open_sudo_Init();
void Sudoopen_set_handle();
void lianji_run();
void reset_data();
void sudo_click_start();
extern u16 Adress_Change_0;
//extern float percent;
extern u8 Screen_Light[4];
extern u16 Screen_set_Time_min;	//息屏时间
extern u8 Screen_Touch_flag;
//extern  u8 sudo_buff[4];//开机自动启动
#endif