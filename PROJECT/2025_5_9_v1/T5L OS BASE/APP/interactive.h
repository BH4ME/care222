#ifndef __INTERACTIVE_H__
#define __INTERACTIVE_H__
#include "sys.h"

#define Nor_set_light_Ad 0x45000	//屏幕亮度、待机亮度、待机时间存储地址
#define Nor_set_sudoopen_Ad	0x45100	//开机自动启动	数据经过处理后
#define Nor_set_sudoopen_initial_Ad	0x45150	//开机自动启动的初始数据 没经过算法处理
#define Nor_zhiliao_user_Ad 0x45160	//治疗模式界面下 用户自主设置的mj脉冲onoff数值 没有按下快捷键
#define Nor_shoudong_user_Ad 0x4520	//手动界面下 用户自主设置的分钟脉冲onoff数值 没有按下快捷键
#define Nor_P_set 0x48000 //功率自适应 

#define Nor_zhiliao_set_initial_1_Ad	0x44100	//设置界面中 治疗模式快捷键 的源码地址 未经过算法处理
#define Nor_zhiliao_set_initial_2_Ad	0x44400	
#define Nor_zhiliao_set_initial_3_Ad	0x44300	
#define Nor_zhiliao_set_Ad 0x44000	//设置界面中的治疗预设10byt
#define SET_zhiliao_moshi_Adress 0x3800	//把nor数据有关治疗页面的读取到dgus3800寄存器

	
#define Dgus_zhiliao_mode_1_virtual	0x2400	//存储在DGUS中治疗模式。未经过处理的u16
#define Dgus_zhiliao_mode_2_virtual	0x2410	
#define Dgus_zhiliao_mode_3_virtual	0x2420	
	
		
#define Dgus_shezhi_zhiliao_mode_1	0X1310	//存储在DGUS中设置页面治疗模式。经过处理的u16
#define Dgus_shezhi_zhiliao_mode_2	0X1450	
#define Dgus_shezhi_zhiliao_mode_3	0X1320	

void set_page_LED_write();
extern u16 shandong_LED_100ms;
#endif