#include "column_chart.h"


//��״ͼԴ��
//����:�ܼ��� 
//��ʲô���õĽ�����߹���,�뷴������΢��(15727652280)


static struct{
	u16 cmd;//�̶�Ϊ0x0004
	u16 num;
	struct{
		u16 sx;
		u16 sy;
		u16 ex;
		u16 ey;
		u16 color;
	}datas[COLUMN_MAX_NUM];
	u16 end_flag;//������־,�̶�Ϊ0xff00
}column_data;


//������״ͼ
//void column_chart_draw(COLUMN_CHART * chart)
//{
//	#define END_FLAG		0xff00
//	u16 i;
//	
//	if((!chart)||(chart->column_num>COLUMN_MAX_NUM))
//		return;
//	column_data.cmd = 0x0004;
//	column_data.num = chart->column_num;
//	for(i=0;i<column_data.num;i++)
//	{
//		column_data.datas[i].sx = chart->start_x+(chart->x_offset*i);
//		column_data.datas[i].sy = chart->start_y-chart->columns[i].height;
//		column_data.datas[i].ex = column_data.datas[i].sx+chart->column_width;
//		column_data.datas[i].ey = chart->start_y;
//		column_data.datas[i].color = chart->columns[i].color;
//	}
//	if(i<COLUMN_MAX_NUM)
//		column_data.datas[i].sx = END_FLAG;
//	else
//		column_data.end_flag = END_FLAG;
//	
////	ReadDGUS(S_LED_0_Address,(uint8_t *)&LED_Buff[0],sizeof(LED_Buff[0]));
//	WriteDGUS(chart->vp,(uint8_t*)&column_data,(3+5*i)*2);
//}

