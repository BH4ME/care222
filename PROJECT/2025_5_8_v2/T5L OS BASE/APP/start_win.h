#ifndef __START_WIN_H__
#define __START_WIN_H__
#include "sys.h"


#define tubiao_chang 580	//ͼ��ĳ���
//��Ϊ290����������10��590
#define num_maxdian 290

#define tubiao_kuan 120	//ͼ��Ŀ��

#define tubiao_zuoshang_x 10	//ͼ�������x����
#define tubiao_zuoshang_y 346	//ͼ�������y����

#define tubiao_0zuobiao_x 10 //0������x
#define tubiao_0zuobiao_y 426 //0������y

#define tubiao_zuoxia_x 10	//ͼ�������x����
#define tubiao_zuoxia_y 466	//ͼ�������y����


#define CHART_1_ADDR		0xb000
#define CHART_2_ADDR		0x5500		//�ο�ͼ��

#define COLOUR_WHITE	0XFFFF
#define COLOUR_DEEP_RED 0X8000
#define COLOUR_BLACK	0X0000
#define COLOUR_DEEP_GREEN	0X07E0
#define COLOUR_BLUE	0X001F
#define COLOUR_YELLOW	0XFFE0
#define COLOUR_CYAN	0X07FF
#define COLOUR_BROWN	0XA145
#define COLOUR_ORANGE	0XFD20


extern u16 X_calculate_1s;
extern void Draw_chart();	
extern void start_win_Init();
#endif