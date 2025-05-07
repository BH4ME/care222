#include  "qrj.h"
#include "dgus.h"
#include "home.h"
//气溶胶消毒
double TIME_static=0.0;
void QRJ_mm_to_s(u16 L_T);


u8 current_st;
//初始化0x00，第一次进入界面0x01，就绪态0x02（第一次进入界面只运行一次后，未执行任务），运行态0x03开始，
//运行态暂停0x04，运行态结束0x05,复位0x06
/*******************************************************
//开机初始化QRJ_Init 只运行一次
第一次进入界面（从别的页面跳转过来的），或者点击复位按键 QRJ_Reset 只运行一次
在当前页面但未进行任务及未开始或未处于暂停状态QRJ_prepare
进行任务中（处于开始或暂停状态，任务完成仍为开始状态）QRJ_run 但只有按键按的时候才执行
******************************************************/



void QRJ_run()	//按下操作 正在照射		在handle.c中调用 局限性每次有按压反馈的时候才会调用
{


}


void QRJ_Init()		//开机初始化
{
	current_st = 0x00;
	
}



//u8 last_page;
void 	QRJ_Set()				//每次进入页面初始化
{

}

void 	QRJ_Reset()				//按下复位键 只进行一次 
{

}
	
//		L_int   mm

float qirongjiao_j=0.0;

void QRJ_prepare()	//在当前页面 但未操作
{
	u16 ResTime_sec;
  u16 ResTime_min;	
	u16 QRJ_mj;	


	ReadDGUS(0x5100,(uint8_t *)&QRJ_mj,sizeof(QRJ_mj));  	//读取杀灭log值 范围1~5
	switch(QRJ_mj)
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
	
	QRJ_mm_to_s(L_int);	//根据距离和功率得到时间
	
	ResTime_min=(u16)(TIME_static)/16;
	ResTime_sec=(u16)(TIME_static)%16;
	WriteDGUS(0x2820,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***剩余时间*分钟****/
	WriteDGUS(0x2800,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***剩余时间*分钟****/
	WriteDGUS(0x2805,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***剩余时间*秒****/
	WriteDGUS(0x2825,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***剩余时间*秒****/

}
	
	
	
/******************输入距离给出时间 mm->s*********/
u8 L_T_S_FLAG;

void QRJ_mm_to_s(u16 L_T)	
{
	double L_CM;
	L_CM=L_T/10.0;
	if(L_CM<400	&& L_CM>5) L_T_S_FLAG=1;		//在5cm~4m范围
				else L_T_S_FLAG=5;

	switch(L_T_S_FLAG)
	{
		case 1:
		{
			TIME_static=qirongjiao_j/change_P_uW_doing;
			break;
		}

				default:
		{
			L_T_S_FLAG=6;
			
			break;
		}
	}
}


void QRJ_stop()
{
	
}



void QRJ_Home()	//循环运行
{
	
		if(last_page!=CurrentPageNum[1])	//第一次进入页面
	{
		last_page=CurrentPageNum[1];	
		current_st = 0x01;
	}
//	else 
//	{
//		
//	}	
	
	switch(current_st)		//在handle.c触摸反馈中检测
	{
		case 0:		//开机初始化状态
		{
			current_st = 0x01;	//初始化成功，跳转到第一次初始化态
		//	break;
		}
		
		case 1:		//第一次初始化态
		{
			current_st = 0x02;	//第一次初始化成功，跳转到就绪态
			QRJ_Set();
			
		//	break;
		}
		
		case 2:		//就绪态
		{
			QRJ_prepare();
			break;
		}
		
				case 3:		//运行态开始
		{
			
			QRJ_run();
			break;
		}
		
				case 4:		//运行态暂停
		{
			QRJ_stop();
			break;
		}
		
				case 5:		//运行态结束 此为自然结束不包含复位
		{
			
			break;
		}
				case 6:		//运行态复位 只进行一次 然后跳转到第一次初始化
		{
			current_st = 0x01;	// 置位 到第一次初始化
			QRJ_Reset();	//只运行一次
			
			break;
		}
		
		default :
		{
			break;
		}
		
	}
	
	
}