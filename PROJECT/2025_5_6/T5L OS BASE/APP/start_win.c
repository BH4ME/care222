#include "start_win.h"
#include <math.h>
#include "qrj.h"
#include "home.h"
typedef struct {
    u16 x;  // X坐标 (0-65535)
    u16 y;  // Y坐标 (0-65535) 
} DgusPoint;


typedef struct {
    u16  cmd_type_line;     //  (0x00)    // (固定0x02)
    u16  chart_num;     // 条数
    u16  color;        // RGB565颜色值
    DgusPoint points[2];    // 两个坐标点
    u16   end_marker;   // 结束标志 (0xFF)
  //  u8   reserved;     //  (0x00)
} DgusLineCommand;



typedef struct {
    DgusPoint points;    // 坐标点
    u16   end_marker;   // 结束标志 (0xFF)
  //  u8   reserved;     //  (0x00)
} Dguspoint_update;


Dguspoint_update point_update;

static	DgusLineCommand Line[2];	//初始化两个图像表


//初始化
void start_win_Init()	//初始化只运行一次
{

	Line[0].cmd_type_line = 0X0002;
	Line[0].end_marker = 0xff00;
	Line[0].chart_num = 0x0001;
	
	Line[1].cmd_type_line = 0X0002;
	Line[1].end_marker = 0xff00;
	Line[1].chart_num = 0x0001;
	
	point_update.end_marker= 0xff00;
}



//获取Y的坐标
u16 Y_log_calculate(double P_uW)	//根据功率大小计算Y值的大小（纵坐标）
{
	
	static double P_log_uW;	//log 0.11x~3.22x
	P_log_uW=log10(P_uW);	//P_uW 1.3~1666
	/*******************************计算Y坐标结果值为像素**********************************/

	if(P_log_uW>=0)	// 0.11x~3.22x 量化到[0,120]
	{
		
		return tubiao_zuoxia_y - (((double)(P_log_uW - 0.11f))/3.11f)*120;
	}
	else
		return tubiao_zuoxia_y;
}


//获取X的坐标
u16 X_calculate_1s=0;//定时器中
static u8 num_dian = 0;	//已经写入点的数量 复位要将其设为0
u16 X_calculate()		//正常是返回X地址，错误代码0x0000/0xffff/0xfffe
{
	static u8 wait_1s = 0;	
	static u8 doing_1s = 0;
	static u16 last_y=0;
	if(X_calculate_1s>=10)	//1s
	{
		doing_1s++;
		X_calculate_1s=0;
		if(num_dian==0)	//first第一次写入点位置是固定好的
		{
			num_dian++;
			last_y = tubiao_zuoshang_x;
			return tubiao_zuoshang_x;
		}
		else if(TIME_static>=tubiao_chang-last_y+10)//大于580
		{
			//限制了最大290个点
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
			return 0;
		}else if(TIME_static<=(tubiao_chang-last_y+10)/2)//<290 
		{
				wait_1s = (tubiao_chang-last_y+10)/TIME_static;
				doing_1s=0;
				num_dian++;
				last_y = last_y+wait_1s;
			
					if(last_y>tubiao_zuoshang_y || last_y<tubiao_zuoxia_y)	//超出量程范围则认为有问题
					{
						return 0xfffe;
					}
	
				return last_y;
		}
		return 0;
	}

	
	if(num_dian>num_maxdian)	//如果点数大于最大值290则认为有问腿
	{
		return 0xffff;
	}
			return 0;
}



//逻辑上根据时间去判断是否需要取样，再进行对y轴的取样工作
//获得X Y的坐标
DgusPoint point_get;
u8 get_X_Y(DgusPoint *point)
{
	point->x=X_calculate();
	if(point->x != 0 ||	point->x != 0xffff || point->x != 0xfffe )	//排除三个error
	{
		//x坐标获取正常 则获取Y的坐标
		point->y =	 Y_log_calculate(change_P_uW_doing);
		return 1;
	}
	return 0;
}


//	chart[0]=(u8)(chart_num_2>>8);
//	chart[1]=(u8)(chart_num_2&0xff);
//先写入两个点之后，每次只更新一个点

u16 Adress_line_1 = 0xb007;	//CHART_1_ADDR + 0x07    复位后要置0xb007
u16 Adress_line_2 = 0x5507;	//CHART_2_ADDR + 0x07
void update_point(const DgusPoint *point,u8 line_num)
{
	switch(line_num)
	{
		case 0:	//曲线1
		{
			
			if(get_X_Y(point))	//获取XY坐标成功返回1
			{
				WriteDGUS(Adress_line_1,(uint8_t *)&point,sizeof(point));
				//num_dian
				Adress_line_1 += 0x2; 
			}
			break;
		}
		case 1:
		{
			
			break;
		}
		
		default :
		{
			
			break;
		}
	}

}	


void Draw_chart(u8 line_num)	//分第1，2条曲线
{
	switch(line_num)
	{
		case 0:	//曲线1
		{
			
			break;
		}
		case 1:
		{
			
			break;
		}
		
		default :
		{
			
			break;
		}
	}
	
}

//void sys_write_chart_zhexian_Two(u16 x1,u16 y1,u16 x2,u16 y2)		//输入2个点
//{
//	#define CHART_ADDR		0xb000	//动态曲线功能所对应的系统变量接口地址
//	//#define CHART_NUM			0xff				//线段数数据,我们只写入chart_id指定的曲线通道,即1个
//	//#define POINT_NUM			1  			//一次性写入多少个数据点
//	#define colour 0Xffff
//	u8 chart_cmd[16] = {0x00,0x02};
//	chart_cmd[2]=(u8)(chart_num>>8);
//	chart_cmd[3]=(u8)(chart_num&0xff);
//	chart_cmd[4] = (u8)(colour>>8);//曲线通道id
//	chart_cmd[5] = (u8)(colour&0xff);//一次性写入多少个数据点
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
//	//发送命令
//	WriteDGUS(CHART_ADDR,chart_cmd,sizeof(chart_cmd));
//}
//void sys_write_chart_zhexian_Two_Cankao(u16 x1,u16 y1,u16 x2,u16 y2)		//输入2个点
//{
//	#define CHART_ADDR_cankao		0x5500	//动态曲线功能所对应的系统变量接口地址
//	#define CHART_NUM_cankao			0xff				//线段数数据,我们只写入chart_id指定的曲线通道,即1个
//	//#define POINT_NUM			1  			//一次性写入多少个数据点
//	#define colour_cankao 0X8410
//	u8 chart_cmd[16] = {0x00,0x02,0x0f,CHART_NUM_cankao};
//	chart_cmd[4] = (u8)(colour_cankao>>8);//曲线通道id
//	chart_cmd[5] = (u8)(colour_cankao&0xff);//一次性写入多少个数据点
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
//	//发送命令
//	WriteDGUS(CHART_ADDR_cankao,chart_cmd,sizeof(chart_cmd));
//}
