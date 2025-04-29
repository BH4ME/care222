#include  "home.h"
#include "dgus.h"
#include "qrj.h"
#include "home.h"

#include <math.h>


uint8_t CurrentPageNum[2]={0};	//CurrentPageNum[1]是当前页面


float L_M = 0.0;//距离 m
//u16 Time_static_16;	//静态的时间由距离得出的
void Prepare_qrj_zx_qy();	//一直运行的 无论哪个状态适用与前三个模式

/*********************************************************************************
中心模式：求面积和直径	面积是区域模式的四分之一，直径是区域模式的二分之一
区域模式：求面积和直径	
气溶胶模式：求体积
********************************************************************************/
float Tanx=0.4557;
float pai=3.1415;
float Tan_zhongxin=0.04366094;
float zhongxin_35mm=0.4008159;	//=0.0175m/tan2.5

float L_m_D_V_S=0.00;				//距离 单位:M
float Square=0.00;			//面积 单位：M^2
float D=0.00;					//直径 单位：M
float Volume=0.00;					//气溶胶体积 单位：M^3


u8 last_page;
//放在主函数里循环执行当前页面的程序
void ReadCurrentPage_run()		
{

	
	ReadDGUS(0x14,(uint8_t *)&CurrentPageNum,sizeof(CurrentPageNum));
	switch(CurrentPageNum[1])
	{
		case Page_home:	
		{
			last_page = CurrentPageNum[1];
			current_st = 0x01;
			break;
		}
		
		case Page_qirongjiao:
		{
			
			
		//	QRJ_prepare();
			
			QRJ_Home();
			Prepare_qrj_zx_qy();
			
			break;
		}
			
		case Page_zhongxin:
		{
			
			Prepare_qrj_zx_qy();			
			
			
			break;
		}
		
		case Page_quyu:
		{
			
			Prepare_qrj_zx_qy();
			
			break;
		}
		
				
		case Page_chengxuhua:
		{

			break;
		}
		
				
		case Page_zhiliao:
		{
			break;
		}
		
				
		case Page_shoudong:
		{		

			break;
		}
					
		case Page_lianji:
		{
	
			break;
		}
		
				
		case Page_shezhi:
		{

			break;
		}
		
		case Page_chengxuhua_JUMP:
		{
			
			break;
		}
				
			case Page_zhiliao_JUMP:
		{
			break;
		}
		default:
		{
			
			break;
		}
		
	}
	
}






//void S_D_Surfaces_All()
//{

//	L_m	=	 (float)(L_int/1000.000)+zhongxin_35mm;
//	if(W_S[0]==2)//zx
//	{
//	D	=	Tan_zhongxin*2*L_m;
//	Square	=	(float)(pow(L_m*Tan_zhongxin,2)*pai);	
//	}
//	else 
//	{
//	D	=	Tanx*2*L_m;
//	Square	=	(float)(pow(L_m*Tanx,2)*pai);	
//	}
//	Volume	=	Square*L_m/3.00;
//}



/**************************************************
距离 换算 为功率大小
形参为毫米
*******************************************************/
double change_P_uW=0.0;	//lg change_P_uW_doing
float change_P_uW_doing=0.0;	//根据距离在推出的功率
//double change_P_mW_First=0.0;

void L_mm_TO_P_uw(u16 L_mm_TO_P,u8 current_page)		//形式参数毫米
{
	u8 L_mm_TO_P_uw_FLAG=0;
	double BEISHU=0.00000;
	double L_CM;
	double mihanshu=0.0;
	
	
	L_CM=L_mm_TO_P/10;
	
	if(L_CM<=10&& L_CM>=5) L_mm_TO_P_uw_FLAG=1;
		else 	if(L_CM<=20&& L_CM>10) L_mm_TO_P_uw_FLAG=2;
			else 	if(L_CM<=100&& L_CM>20) L_mm_TO_P_uw_FLAG=3;
			else 	if(L_CM<=300&& L_CM>100) L_mm_TO_P_uw_FLAG=4;
				else L_mm_TO_P_uw_FLAG=5;
	
	/***66300.18x^-1.86****/
	switch(L_mm_TO_P_uw_FLAG)
	{
		case 1:
		{
			mihanshu = pow((double)(L_CM),-1.6);
			BEISHU = 36527.86*mihanshu;
			change_P_uW_doing=BEISHU;
			break;
		}
		
		
				case 2:
		{
			mihanshu = pow((double)(L_CM),-1.85);
			BEISHU = 65617.44*mihanshu;
			change_P_uW_doing=BEISHU;
			break;
		}
		
				case 3:
		{
			mihanshu = pow((double)(L_CM),-1.81);
			BEISHU = 57622.25*mihanshu;
			change_P_uW_doing=BEISHU;
			break;
		}
		
				case 4:
		{
			mihanshu = pow((double)(L_CM),-2);
			BEISHU = 139224.07*mihanshu;
			change_P_uW_doing=BEISHU;
			break;
		}
				
		default:
		{
			L_mm_TO_P_uw_FLAG=0x0a;
		}
	}
	
	if(Page_qirongjiao==current_page || Page_quyu==current_page)
	{
		change_P_uW_doing=change_P_uW_doing*0.6;	//气溶胶和区域需要乘0.6
	}
	//if(W_S[0]!=2&&MODE==1)change_P_mW_doing=change_P_mW_doing*0.6;	//气溶胶和区域需要乘0.6
//	change_P_mW_First=change_P_mW_doing/1000;		//uw变mw
//	change_P_uW=log10(change_P_uW_doing);	//用在后期画图判断的时候
//	P_FINISH=change_P_mW_First;//uw变mw

}


//距离、功率、体积、直径、面积
void Prepare_qrj_zx_qy()	//一直运行的 无论哪个状态适用与前三个模式
{
			L_mm_TO_P_uw(L_int,CurrentPageNum[1]);	//根据距离得到功率 
			WriteDGUS(0x2600,(uint8_t *)&change_P_uW_doing,sizeof(change_P_uW_doing));		/*********功率****************/
			L_M	=	(float)(L_int/1000.00f);	//m数据/*********距离****************/
				
	if(CurrentPageNum[1]==Page_qirongjiao)
	{
			L_m_D_V_S	=	 (float)(L_int/1000.000)+zhongxin_35mm;
			D	=	Tanx*2*L_m_D_V_S;
			Square	=	(float)(pow(L_m_D_V_S*Tanx,2)*pai);	
			Volume	=	Square*L_m_D_V_S/3.00;
			WriteDGUS(0x2850,(uint8_t *)&(Volume),sizeof(Volume));	//气溶胶为体积
	}
	else if(CurrentPageNum[1]==Page_zhongxin)
	{
					
			L_m_D_V_S	=	 (float)(L_int/1000.000)+zhongxin_35mm;
			D	=	Tan_zhongxin*2*L_m_D_V_S;
			Square	=	(float)(pow(L_m_D_V_S*Tan_zhongxin,2)*pai);	
			
			WriteDGUS(0x2850,(uint8_t *)&(Square),sizeof(Square));//面积	
	}
	else if(CurrentPageNum[1]==Page_quyu)
	{
			L_m_D_V_S	=	 (float)(L_int/1000.000)+zhongxin_35mm;
			D	=	Tanx*2*L_m_D_V_S;
			Square	=	(float)(pow(L_m_D_V_S*Tanx,2)*pai);	
			
			WriteDGUS(0x2850,(uint8_t *)&(Square),sizeof(Square));//面积	
	}
	
			WriteDGUS(0x2500, (uint8_t *)&L_M,sizeof(L_M));	
			WriteDGUS(0x2840,(uint8_t *)&(D),sizeof(D));			//直径
	
	
}




float mJ_doing=0.0;	//已照射的mj
u32 time_100ms=0;	//100ms的中断标志位
u32 time_doing=0;//已经照射的时间
//气溶胶、中心、区域 run函数
void QRJ_ZX__QY_RUN()						//放在QRJ_run
{
	u16 ResTime_sec;
  u16 ResTime_min;	
	
	if(time_100ms%100==0)	//100ms
	{
		
		mJ_doing=mJ_doing+(change_P_uW_doing/1000.00f)*0.1;	//每0.1秒累加当前的mj辐射量
		if(time_100ms>=1000)	//1s
		{
			time_100ms=0;
			time_doing++; //工作时长
			
			TIME_static--;
	
		

		}
		
	  QRJ_mm_to_s(L_int);	//根据距离和功率得到时间
	
		ResTime_min=(u16)(TIME_static)/16;
		ResTime_sec=(u16)(TIME_static)%16;

		WriteDGUS(0x2800,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***剩余时间*分钟****/
		WriteDGUS(0x2805,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***剩余时间*秒****/
		//总时间 等于剩余时间加上已经工作的时间
		ResTime_min=(u16)(TIME_static+time_doing)/16;
		ResTime_sec=(u16)(TIME_static+time_doing)%16;
		WriteDGUS(0x2825,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***总时间*秒****/
		WriteDGUS(0x2820,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***总时间*分钟****/
		
		WriteDGUS(0x2700,(uint8_t *)&mJ_doing,sizeof(mJ_doing));		/***辐射量mj***/
	}
	
	
	
	
	
}