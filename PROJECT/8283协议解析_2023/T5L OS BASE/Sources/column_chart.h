#ifndef __COLUMN_CHART_H__
#define __COLUMN_CHART_H__
#include "sys.h"
#include "dgus.h"

#define COLUMN_MAX_NUM	20		//�������,���Ը������������Ӧ�޸�


//��������
typedef struct{
	u16 vp;//����ͼ�οؼ���VP��ַ
	u16 start_x;//��״ͼ�е�һ�е����½�x����
	u16 start_y;//��״ͼ�е�һ�е����½�y����
	u16 x_offset;//��״ͼ��ÿһ��֮���x����
	u16 column_width;//��״ͼ��ÿһ�еĿ��
	u16 column_num;//��״ͼ���ܹ��ж�����
	struct{
		u16 height;//��״ͼ��ÿһ�еĸ߶�
		u16 color;//��״ͼ��ÿһ�е���ɫ
	}columns[COLUMN_MAX_NUM];
}COLUMN_CHART;



//��������
void column_chart_draw(COLUMN_CHART * chart);









#endif