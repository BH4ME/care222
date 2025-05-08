#include "start_win.h"
#include <math.h>
#include "qrj.h"
#include "home.h"
#include "uart.h"
u8 STATE_FIRST_FLAG;	//��λ����0


//״̬��
typedef enum {
    STATE_IDLE,
	  STATE_INIT,
    STATE_FIRST_RUNNING,
	  STATE_RUNNING,
		STATE_RUNNING_FINISH,
    STATE_PAUSED,
    STATE_RESET,
    STATE_ERROR
} SystemState;

SystemState current_state = STATE_INIT;



typedef struct {
    u16 x;  // X���� (0-65535)
    u16 y;  // Y���� (0-65535) 
} DgusPoint;

DgusPoint point_get;



typedef struct {
    u16  cmd_type_line;     //  (0x00)    // (�̶�0x02)
    u16  chart_num;     // ����
    u16  color;        // RGB565��ɫֵ
    DgusPoint points[2];    // 2�������
    u16   end_marker;   // ������־ (0xFF)
  //  u8   reserved;     //  (0x00)
} DgusLineCommand;



typedef struct {
    DgusPoint points;    // �����
    u16   end_marker;   // ������־ (0xFF)
  //  u8   reserved;     //  (0x00)
} Dguspoint_update;


Dguspoint_update point_update;

static	DgusLineCommand Line[2];	//��ʼ������ͼ���


//��ʼ��
void start_win_Init()	//��ʼ��ֻ����һ��
{

	Line[0].cmd_type_line = 0X0002;
	Line[0].end_marker = 0xff00;
	Line[0].chart_num = 0x0001;
	
	Line[1].cmd_type_line = 0X0002;
	Line[1].end_marker = 0xff00;
	Line[1].chart_num = 0x0001;
	
	point_update.end_marker= 0xff00;
}



//��ȡY������
u16 Y_log_calculate(double P_uW)	//���ݹ��ʴ�С����Yֵ�Ĵ�С�������꣩
{
	
	static double P_log_uW;	//log 0.11x~3.22x
	P_log_uW=log10(P_uW);	//P_uW 1.3~1666
	/*******************************����Y������ֵΪ����**********************************/

	if(P_log_uW>=0)	// 0.11x~3.22x ������[0,120]
	{
		
		return tubiao_zuoxia_y - (((double)(P_log_uW - 0.11f))/3.11f)*120;
	}
	else
		return tubiao_zuoxia_y;
}


//��ȡX������
u16 X_calculate_1s=0;//��ʱ����
static u8 num_dian = 0;	//�Ѿ�д�������� ��λҪ������Ϊ0
u16 X_calculate()		//�����Ƿ���X��ַ���������0x0000/0xffff/0xfffe
{
	static u8 wait_1s = 0;	
	static u8 doing_1s = 0;
	static u16 last_y=0;
//	if(X_calculate_1s>=10)	//1s
//	{
		//Uart4SendByte();
		//printf("1s\n");
		doing_1s++;
		X_calculate_1s=0;
		if(num_dian==0)	//first��һ��д���λ���ǹ̶��õ�
		{
			
			num_dian++;
			last_y = tubiao_zuoshang_x;
		//	printf("num_dian:%d\n",num_dian);
			return tubiao_zuoshang_x;
		}
		else if(TIME_static>=tubiao_chang-last_y+10)//����580
		{
			//���������290����
			wait_1s = TIME_static/(tubiao_chang-last_y+10)+1 ;
			if(doing_1s >= wait_1s)
			{
				doing_1s=0;
				num_dian++;
				last_y = last_y+0x02;
					if(last_y>tubiao_zuoshang_y || last_y<tubiao_zuoxia_y)
					{
						return 0xfffe;
					}
				return last_y;
			}
		}
		else if(TIME_static<=tubiao_chang-last_y+10 && TIME_static>=(tubiao_chang-last_y+10)/2)//290~580
		{
			wait_1s = 2;
			if(doing_1s >= wait_1s)
			{
				doing_1s=0;
				num_dian++;
				last_y = last_y+0x02;
					if(last_y>tubiao_zuoshang_y || last_y<tubiao_zuoxia_y)
					{
						return 0xfffe;
					}
				return last_y;
			}
		}

//		else if(TIME_static>=tubiao_chang-last_y+10)//����580
//		{
//			//���������290����
//			wait_1s = TIME_static/(tubiao_chang-last_y+10)+1 ;
//			if(doing_1s >= wait_1s)
//			{
//				doing_1s=0;
//				num_dian++;		

//		//		printf("num_dian:%d\n",num_dian);
//				last_y = last_y+0x02;
//					if(last_y>tubiao_zuoshang_y || last_y<tubiao_zuoxia_y)
//					{
//						return 0xfffe;
//					}
//				return last_y;
//			}
//		}
//		else if(TIME_static<=tubiao_chang-last_y+10 && TIME_static>=(tubiao_chang-last_y+10)/2)//290~580
//		{
//			wait_1s = 2;
//			if(doing_1s >= wait_1s)
//			{
//				doing_1s=0;
//				num_dian++;			
//				
//	//			printf("num_dian:%d\n",num_dian);
//				last_y = last_y+0x02;
//				return last_y;
//			}
//			return 0;
		//}
		else if(TIME_static<=(tubiao_chang-last_y+10)/2)//<290 
		{
				wait_1s = (tubiao_chang-last_y+10)/TIME_static;
				doing_1s=0;
				num_dian++;
				last_y = last_y+wait_1s;
	
		//	printf("num_dian:%d\n",num_dian);
					if(last_y>tubiao_zuoshang_y || last_y<tubiao_zuoxia_y)	//�������̷�Χ����Ϊ������
					{
						return 0xfffe;
					}
	
				return last_y;
		}
	//	return 0;
	//}

	
	if(num_dian>num_maxdian)	//��������������ֵ290����Ϊ������
	{
		return 0xffff;
	}
	
			return 0;
}



//�߼��ϸ���ʱ��ȥ�ж��Ƿ���Ҫȡ�����ٽ��ж�y���ȡ������
//���X Y������


u8 get_X_Y(DgusPoint *point)
{
	if(X_calculate_1s>=10)	//1s
	{
		point->x=X_calculate();
	}
	
	//�����߼������߼�����ĳ��߼���
	if(point->x != 0 &&	point->x != 0xffff && point->x != 0xfffe )	//�ų�����error	
	{
		//x�����ȡ���� ���ȡY������
		point->y =	 Y_log_calculate(change_P_uW_doing);
		return 1;
	}
	return 0;
}


//	chart[0]=(u8)(chart_num_2>>8);
//	chart[1]=(u8)(chart_num_2&0xff);
//��д��1����֮��ÿ��ֻ����һ����

u16 Adress_line_1 = 0xb005;	//CHART_1_ADDR + 0x07    ��λ��Ҫ��0xb005
u16 Adress_line_2 = 0x5505;	//CHART_2_ADDR + 0x07

void update_point( DgusPoint *point,u8 line_num)
{
	u8 chart_numline[2]={0};
	switch(line_num)
	{
		case 1:	//����1
		{
			
			if(get_X_Y(point))	//��ȡXY����ɹ�����1
			{
				chart_numline[0]=(u8)((num_dian)>>8);
				chart_numline[1]=(u8)((num_dian)&0xff);			//���������ߣ�ȡ���ڵ������������ڵ���-1
				point_update.points.x = point->x;
				point_update.points.y = point->y;		
				WriteDGUS(CHART_1_ADDR+0X01,(uint8_t *)&chart_numline,sizeof(chart_numline));			//��������
				WriteDGUS(Adress_line_1,(uint8_t *)&point_update,sizeof(point_update));
				//num_dian
				Adress_line_1 += 0x2; 
				
				
			}
			break;
		}
		case 2:
		{
			
			break;
		}
		
		default :
		{
			
			break;
		}
	}

}	


void CHART_RUNNING()
{

	update_point(&point_get,1);	//��������1��ÿ��д��һ����ͽضϴ��������+1
	
}


//�ο�ͼ��
void CHART_FIRST__RUNNING_CANKAO(u8 num_lines_cankao)		//Ĭ�ϵڶ�������Ϊ�ο�����
{
		
	
		Line[num_lines_cankao-1].chart_num = 1;//��������1
		
		Line[num_lines_cankao-1].color = COLOUR_WHITE;
		
		Line[num_lines_cankao-1].points[0].x = tubiao_zuoxia_x + 0x01;
		Line[num_lines_cankao-1].points[0].y = Y_log_calculate(change_P_uW_doing);
	
		Line[num_lines_cankao-1].points[1].x = tubiao_zuoxia_x + tubiao_chang;
		Line[num_lines_cankao-1].points[1].y = Line[num_lines_cankao-1].points[0].y;
	if(num_lines_cankao == 2)
	{
		WriteDGUS(CHART_2_ADDR,(uint8_t *)&Line[num_lines_cankao-1],sizeof(Line[num_lines_cankao-1]));
	}
	
}

//��һ������ ��ֻд��һ����
void CHART_FIRST_RUNNING(DgusPoint *point,u8 line_num)
{	
	//CHART_FIRST__RUNNING_CANKAO(2);			//�ο���������һ��
				if(get_X_Y(point))	//��ȡXY����ɹ�����1
			{
				
				CHART_FIRST__RUNNING_CANKAO(2);			//�ο���������һ��
					printf("��ʼ��\n");
				
				if(line_num==1)
				{
					Line[0].chart_num = 0;//��������0
					
					Line[0].color = COLOUR_DEEP_RED;
					Line[0].points[0].x = tubiao_zuoxia_x + 0x01;
			//		Line[0].points[0].x = point->x; tubiao_zuoxia_x + 0x01;
					Line[0].points[0].y = point->y;
					
				
					printf("��ʼ��\n");
					
					
					WriteDGUS(CHART_1_ADDR,(uint8_t *)&Line[0],sizeof(Line[0]));
				//num_dian
				//Adress_line_1 += 0x2; 
				}
//				else if(line_num==2)
//				{
//					
//					Line[1].chart_num = 0;//��������0
//					
//					Line[1].color = 0xffff;
//					
//					Line[1].points[0].x = point->x;
//					Line[1].points[0].y = point->y;
//					WriteDGUS(CHART_2_ADDR,(uint8_t *)&point,sizeof(point));
//				//num_dian	
//				}
				
				
				
				
				STATE_FIRST_FLAG = 1;	//��һ��������ɣ��´ν�����������״̬
			}
}



void CHART_RESET(u8 num_lines)	//1,2����1��2���������0��ȫ����������
{
	u16 Reset[8]={0};
	switch(num_lines)
	{
		case 0:
		{
			WriteDGUS(CHART_1_ADDR,(uint8_t *)&Reset,sizeof(Reset));
			WriteDGUS(CHART_2_ADDR,(uint8_t *)&Reset,sizeof(Reset));
			break;
		}
		
		case 1:
		{
			WriteDGUS(CHART_1_ADDR,(uint8_t *)&Reset,sizeof(Reset));		
			break;
		}
		
		
		case 2:
		{
			WriteDGUS(CHART_2_ADDR,(uint8_t *)&Reset,sizeof(Reset));
			break;
		}
		
		default :
		{
			
			break;
		}
	}
	
	
}
//״̬ת����֮ǰ��״̬������״̬ת����������ݺͺ�����ֲ
/**********************************************
u8 current_st;
��ʼ��0x00����һ�ν������0x01������̬0x02����һ�ν������ֻ����һ�κ�δִ�����񣩣�����̬0x03��ʼ��
����̬��ͣ0x04������̬����0x05,��λ0x06

*************************************************/
u8 STATE_FIRST_FLAG=0;	//��λ����0
void switch_state()
{
		switch(current_st)
	{

		case 2:	
		{
			
			break;
		}
		case 3:
		{
			if(STATE_FIRST_FLAG)	//1
			{
				current_state = STATE_RUNNING;
			}
			else
			{
				current_state = STATE_FIRST_RUNNING;
			}

			break;
		}
		
			case 4:	
		{
			current_state = STATE_PAUSED;
			break;
		}
		case 5:
		{
			STATE_FIRST_FLAG = 0;
			current_state = STATE_RUNNING_FINISH;
			break;
		}
		case 6:
		{
			
				current_state = STATE_RESET;
			break;
		}
		default :
		{
			
			break;
		}
	}
	
		
}





void Draw_chart()	//�ֵ�1��2������
{
	
	switch_state();			//״̬ת��
	switch(current_state)
	{
		case STATE_INIT:	
		{
			current_state = STATE_IDLE;
			break;
		}
		case STATE_IDLE:
		{
			
			break;
		}
				case STATE_FIRST_RUNNING:	
		{
			//STATE_FIRST_FLAG = 1;
			
			CHART_FIRST_RUNNING(&point_get,1);
			break;
		}
		case STATE_RUNNING:
		{
			//CHART_RUNNING();	//������
			break;
		}
		
		case STATE_RUNNING_FINISH:	//������� 
		{
			STATE_FIRST_FLAG = 0;
			current_state = STATE_IDLE;
			num_dian = 0;	
			
			
		//	CHART_RESET(0);	//����ȫ������
			
		  Adress_line_1 = 0xb005;	//CHART_1_ADDR + 0x07    ��λ��Ҫ��0xb005
			Adress_line_2 = 0x5505;	//CHART_2_ADDR + 0x07
			break;
		}
			case STATE_PAUSED:	
		{
			
			break;
		}
		case STATE_RESET:
		{
			STATE_FIRST_FLAG = 0;
			current_state = STATE_IDLE;
			num_dian = 0;	
			
			
			CHART_RESET(0);	//����ȫ������
			
		  Adress_line_1 = 0xb005;	//CHART_1_ADDR + 0x07    ��λ��Ҫ��0xb005
			Adress_line_2 = 0x5505;	//CHART_2_ADDR + 0x07
			break;
		}
		case STATE_ERROR:
		{
			
			break;
		}
		default :
		{
			
			break;
		}
	}
	
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
