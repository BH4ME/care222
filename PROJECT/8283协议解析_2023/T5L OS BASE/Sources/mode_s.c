#include "T5LOS8051.H"
#include "sys.h"
#include "handle.h"
//#include<stdio.h>
#include "math.h"
#include "mode_s.h"
#include "start_win.h"


uint MODE_S_C[4]={0,0,0,0};
bit MODE_S_Aerosols=0;				/*****0关闭*****/
bit MODE_S_Surfaces_Centre=0;
bit MODE_S_Surfaces_All=0;
uint ZIWAIXIAN_S_FINISH_FLAG=0; 

uint First_S_shuju_FLAG=0;
uint L_T_S_FLAG=0;
double T_y=0.0;
double T_y_Last=0.0;
double T_beishu=1.0;
double T_S_WORK=0.0;
//uint T_XIANSHI=0;
uint T_S_First=0;
//double percent=1.0;
float W_mJ_XIANSHI=0.0;
//uint Reset_flag=0;	
u8 MODE_1_Finish=0;		//模式一 自己时间消耗完 自动结束 0代表未结束，1代表自动结束模式1
u16 Read_log_j;
float qirongjiao_j;
//float quyu_j;
float quyu_zhongxin_j;
void Mode_s_work(void);
void S_bianliangqingling(void);
void Transform_S_L_T_Aerosols(uint L_T);
void Transform_S_L_T_Surfaces_Centre(uint L_T);
void Transform_S_L_T_Surfaces_All(uint L_T);
void S_LAST_Time(void);
void qirongjiao_log(u16 log);
void quyu_zhongxin_log(u16 log);


void MODE_S_CHOOSE(void)
{
	
		if(First_S_shuju_FLAG==0)
	{
		L_FINISH =	L_int;
		
	}
	//W_S[0]
	
//	ReadDGUS(0x5100,(uint8_t *)&Read_log_j,sizeof(Read_log_j));	
	
	
	switch(W_S[0])
	{
		case 1:
		{
			MODE_S_Aerosols=1;
			MODE_S_Surfaces_All=0;
			MODE_S_Surfaces_Centre=0;
			ReadDGUS(0x5100,(uint8_t *)&Read_log_j,sizeof(Read_log_j));	//气溶胶log
			 qirongjiao_log(Read_log_j);
			Transform_S_L_T_Aerosols(L_int);
			break;
		}
				case 3:
		{
			MODE_S_Aerosols=0;
			MODE_S_Surfaces_All=1;
			MODE_S_Surfaces_Centre=0;
			ReadDGUS(0x5102,(uint8_t *)&Read_log_j,sizeof(Read_log_j));
			quyu_zhongxin_log(Read_log_j);
			Transform_S_L_T_Surfaces_Centre(L_int);
			break;
		}
				case 2:
		{
			MODE_S_Aerosols=0;
			MODE_S_Surfaces_All=0;
			MODE_S_Surfaces_Centre=1;
			ReadDGUS(0x5101,(uint8_t *)&Read_log_j,sizeof(Read_log_j));
			quyu_zhongxin_log(Read_log_j);
			Transform_S_L_T_Surfaces_All(L_int);
			break;
		}
				default:
				{
					MODE_S_Aerosols=0;				/*****0关闭*****/
					MODE_S_Surfaces_All=0;
					MODE_S_Surfaces_Centre=0;
				}
	}
	//T_y=T_y;
	//T_y=T_y*percent;
}



void quyu_zhongxin_log(u16 log)
{
	switch(log)
	{
		case 1:
		{
			quyu_zhongxin_j	=	1000;
			break;
		}
				case 2:
		{
			quyu_zhongxin_j	=	2000;
			break;
		}
				case 3:
		{
			quyu_zhongxin_j	=	3000;
			break;
		}
				case 4:
		{
			quyu_zhongxin_j	=	5000;
			break;
		}
				case 5:
		{
			quyu_zhongxin_j	=	7000;
			break;
		}
		
	}
}


void qirongjiao_log(u16 log)
{
	switch(log)
	{
		case 1:
		{
			qirongjiao_j	=	500;
			break;
		}
				case 2:
		{
			qirongjiao_j	=	1000;
			break;
		}
				case 3:
		{
			qirongjiao_j	=	1500;
			break;
		}
				case 4:
		{
			qirongjiao_j	=	2500;
			break;
		}
				case 5:
		{
			qirongjiao_j	=	4000;
			break;
		}
		
	}
}


void First_S_shuju(void)
{
	if(First_S_shuju_FLAG==0)
	{
		MODE_S_CHOOSE();
		T_XIANSHI=T_y;
		T_S_First=T_y;
		T_First=T_y;
		//T_XIANSHI=100;
		First_S_shuju_FLAG=1;
		Draw_W_Flag=1;
	}
	
}





/******************输入距离给出时间 mm->s*********/
void Transform_S_L_T_Aerosols(uint L_T)	
{
	double L_CM;
	//uint qirongjiao_j;

	L_CM=L_T/10.0;
	//改为开始的时候读取MJ
	//ReadDGUS(0x5100,(uint8_t *)&qirongjiao_j,sizeof(qirongjiao_j));
	
	if(L_CM<400	&& L_CM>5) L_T_S_FLAG=1;

				else L_T_S_FLAG=5;
	
	
	switch(L_T_S_FLAG)
	{
		case 1:
		{
			T_y=qirongjiao_j/change_P_mW_doing;
			break;
		}

				default:
		{
			L_T_S_FLAG=6;
			
			break;
		}
	}
//	T_y=T_y*percent;
}





void Transform_S_L_T_Surfaces_Centre(uint L_T)	
{
	double L_CM;
	L_CM=L_T/10.0;
	if(L_CM<400	&& L_CM>5) L_T_S_FLAG=1;

				else L_T_S_FLAG=5;
	
	
	switch(L_T_S_FLAG)
	{
		case 1:
		{
			T_y=quyu_zhongxin_j/change_P_mW_doing;
			break;
		}

				default:
		{
			L_T_S_FLAG=6;
			
			break;
		}
	}
}




void Transform_S_L_T_Surfaces_All(uint L_T)	
{
	
double L_CM;
	L_CM=L_T/10.0;

	if(L_CM<400	&& L_CM>5) L_T_S_FLAG=1;

				else L_T_S_FLAG=5;
	
	
	switch(L_T_S_FLAG)
	{
		case 1:
		{
			T_y=quyu_zhongxin_j/change_P_mW_doing;
			break;
		}

				default:
		{
			L_T_S_FLAG=6;
			
			break;
		}
	}
}

//void S_LAST_Time(void)
//{
//	
//	double T_beishu_he=0.0;
//		if(T_S_WORK>0)
//	{
//	T_beishu_he=T_S_WORK/T_y_Last;
//		if(T_beishu_he<=0	||T_beishu_he>=1)
//		{
//			S_bianliangqingling();			
//		}
//	T_beishu=(T_beishu-T_beishu_he);
//	T_XIANSHI=T_beishu*T_y;
//		
//	}
//	
//	T_S_WORK=0;
//	////T_beishu=0.008;
//if(T_XIANSHI<=0|| T_XIANSHI>60000	||	T_beishu>1)
//{
//	S_bianliangqingling();
//}
//	
//}


//void Mode_s_work()
//{
//	
////	T_y_Last=T_y;
////	MODE_S_CHOOSE();
////	S_LAST_Time();
//}


void TIM0_1S_S_isr(void)
{
	W_mJ_XIANSHI=W_mJ_XIANSHI+(change_P_mW_doing/1000.00f);
	T_doing++;
	T_S_WORK++;
	if(T_XIANSHI>0)	T_XIANSHI--;
	UV=1;
	if(T_XIANSHI<=0)
	{
		S_bianliangqingling();
		MODE_1_Finish=1;
			//Reset_flag=1;
	}
}

void S_bianliangqingling(void)
{
	MODE_S_Aerosols=0;				/*****0关闭*****/
	MODE_S_Surfaces_Centre=0;
	MODE_S_Surfaces_Centre=0;
//	MODE_Static=0;
	W_S[2]=0x01;
	shuju_change=0;
	First_S_shuju_FLAG=0;
	L_T_S_FLAG=0;
	T_S_WORK=0;
	T_beishu=1.0;
	T_XIANSHI=0;
	ZIWAIXIAN_S_FINISH_FLAG=0;
//	MODE=0;
	norflash_Close_UV_Flag=1;

	//Work_Time=0;
	UV=0;
	T_S_First=0;
	Draw_W_Flag=0;//停止画线

	
//	W_mJ_XIANSHI=0;
	// Draw_Reset();
	// Rest_Draw_juxing();
	//T_doing=0;	//画进度条清零数据
	First_Draw_Flag=0;	//开启画线
	start_first_flag=0;
	Unlock_current_page();
}

void Static_T_Current_mode1()
{
	if(MODE==1)
	{
		
//	ReadDGUS(0x5100,(uint8_t *)&Read_log_j,sizeof(Read_log_j));	
	
	MODE_S_CHOOSE();
	T_XIANSHI=T_y;
	T_All=T_y;
	}
}