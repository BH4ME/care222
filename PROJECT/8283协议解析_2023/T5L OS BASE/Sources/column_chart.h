#ifndef __COLUMN_CHART_H__
#define __COLUMN_CHART_H__
#include "sys.h"
#include "dgus.h"

#define COLUMN_MAX_NUM	20		//最多列数,可以根据自身情况相应修改


//变量申明
typedef struct{
	u16 vp;//基础图形控件的VP地址
	u16 start_x;//柱状图中第一列的左下角x坐标
	u16 start_y;//柱状图中第一列的左下角y坐标
	u16 x_offset;//柱状图中每一列之间的x轴间隔
	u16 column_width;//柱状图中每一列的宽度
	u16 column_num;//柱状图中总共有多少列
	struct{
		u16 height;//柱状图中每一列的高度
		u16 color;//柱状图中每一列的颜色
	}columns[COLUMN_MAX_NUM];
}COLUMN_CHART;



//函数申明
void column_chart_draw(COLUMN_CHART * chart);









#endif