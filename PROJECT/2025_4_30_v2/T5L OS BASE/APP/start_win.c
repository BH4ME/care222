#include "start_win.h"



typedef struct {
    u16 x;  // X���� (0-65535)
    u16 y;  // Y���� (0-65535) 
} DgusPoint;


typedef struct {
    u16  cmd_type_line;     //  (0x00)    // (�̶�0x02)
    u16  chart_num;     // ����
    u16  color;        // RGB565��ɫֵ
    DgusPoint points[2];    // ���������
    u16   end_marker;   // ������־ (0xFF)
  //  u8   reserved;     //  (0x00)
} DgusLineCommand;

static	DgusLineCommand Line[2];	//��ʼ������ͼ���

void start_win_Init()	//��ʼ��ֻ����һ��
{

	Line[0].cmd_type_line = 0X0002;
	Line[0].end_marker = 0xff00;
	Line[0].chart_num = 0x0001;
	
	Line[1].cmd_type_line = 0X0002;
	Line[1].end_marker = 0xff00;
	Line[1].chart_num = 0x0001;
}

void Draw_chart(u8 line_num)	//
{
	
	
	
}

//void sys_write_chart_zhexian_Two(u16 x1,u16 y1,u16 x2,u16 y2)		//����2����
//{
//	#define CHART_ADDR		0xb000	//��̬���߹�������Ӧ��ϵͳ�����ӿڵ�ַ
//	//#define CHART_NUM			0xff				//�߶�������,����ֻд��chart_idָ��������ͨ��,��1��
//	//#define POINT_NUM			1  			//һ����д����ٸ����ݵ�
//	#define colour 0Xffff
//	u8 chart_cmd[16] = {0x00,0x02};
//	chart_cmd[2]=(u8)(chart_num>>8);
//	chart_cmd[3]=(u8)(chart_num&0xff);
//	chart_cmd[4] = (u8)(colour>>8);//����ͨ��id
//	chart_cmd[5] = (u8)(colour&0xff);//һ����д����ٸ����ݵ�
//	chart_cmd[6] = (u8)(x1>>8);//x1
//	chart_cmd[7] = (u8)(x1&0xff);
//	chart_cmd[8] = (u8)(y1>>8);//y1
//	chart_cmd[9] = (u8)(y1&0xff);
//	chart_cmd[10] = (u8)(x2>>8);//x2
//	chart_cmd[11] = (u8)(x2&0xff);
//	chart_cmd[12] = (u8)(y2>>8);//y2
//	chart_cmd[13] = (u8)(y2&0xff);
//		chart_cmd[14] = (u8)0xff;//y2
//	chart_cmd[15] = (u8)0x00;			//0xa007 
////	y1=y2;
////	x1=x2;
//	Adress_Now=CHART_ADDR+0x7;chart_num++;
//	//��������
//	WriteDGUS(CHART_ADDR,chart_cmd,sizeof(chart_cmd));
//}
//void sys_write_chart_zhexian_Two_Cankao(u16 x1,u16 y1,u16 x2,u16 y2)		//����2����
//{
//	#define CHART_ADDR_cankao		0x5500	//��̬���߹�������Ӧ��ϵͳ�����ӿڵ�ַ
//	#define CHART_NUM_cankao			0xff				//�߶�������,����ֻд��chart_idָ��������ͨ��,��1��
//	//#define POINT_NUM			1  			//һ����д����ٸ����ݵ�
//	#define colour_cankao 0X8410
//	u8 chart_cmd[16] = {0x00,0x02,0x0f,CHART_NUM_cankao};
//	chart_cmd[4] = (u8)(colour_cankao>>8);//����ͨ��id
//	chart_cmd[5] = (u8)(colour_cankao&0xff);//һ����д����ٸ����ݵ�
//	chart_cmd[6] = (u8)(x1>>8);//x1
//	chart_cmd[7] = (u8)(x1&0xff);
//	chart_cmd[8] = (u8)(y1>>8);//y1
//	chart_cmd[9] = (u8)(y1&0xff);
//	chart_cmd[10] = (u8)(x2>>8);//x2
//	chart_cmd[11] = (u8)(x2&0xff);
//	chart_cmd[12] = (u8)(y2>>8);//y2
//	chart_cmd[13] = (u8)(y2&0xff);
//	chart_cmd[14] = (u8)0xff;//y2
//	chart_cmd[15] = (u8)0x00;			//0xa007 
////	y1=y2;
////	x1=x2;
//	//Adress_Now=0xa007;
//	//��������
//	WriteDGUS(CHART_ADDR_cankao,chart_cmd,sizeof(chart_cmd));
//}
