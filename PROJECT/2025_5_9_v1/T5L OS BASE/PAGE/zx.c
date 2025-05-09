#include "zx.h"
#include "dgus.h"
#include "home.h"
#include "qrj.h"


void ZX_done()	//正常照射完成
{
	WriteDGUS(0x3010,(uint8_t *)&icon[0],sizeof(icon[0]));	//图标 变开始
}


void 	ZX_Reset()				//按下复位键 只进行一次 
{
	
	mJ_doing = 0;	//总照射能量 
	WriteDGUS(0x2700,(uint8_t *)&mJ_doing,sizeof(mJ_doing));		/***辐射量mj***/
	
	baifenbi_T = 0;	//完成百分比为0
	WriteDGUS(0x2830,(uint8_t *)&(baifenbi_T),sizeof(baifenbi_T));		
	
	time_doing=0;	//已进行 的时间置为0
	
	WriteDGUS(0x3010,(uint8_t *)&icon[0],sizeof(icon[0]));	//图标 变开始
	 
	
}

void ZX_INIT()		//开机初始化
{
	//current_st = 0x00;
	//WriteDGUS(0x3010,(uint8_t *)&icon[0],sizeof(icon[0]));
}



void ZX_run()	//按下操作 正在照射		
{
	//QRJ_ZX__QY_RUN();
  QRJ_ZX__QY_RUN(ZX_mm_to_s);	

	//触发 照射完成
	if(mJ_doing*1000 >= zhongxin_j  ||  TIME_static<=0  ||  baifenbi_T >=100)
	{		
		current_st = 0x05;	//状态位设置为 自然完成	
	}
	
}


//
void ZX_mm_to_s(u16 L_T)	
{u8 L_T_S_FLAG;
	double L_CM;
	L_CM=L_T/10.0;
	if(L_CM<400	&& L_CM>5) L_T_S_FLAG=1;		//在5cm~4m范围
				else L_T_S_FLAG=5;

	if(current_st!=0x03)	//不等于运行态
	{
	switch(L_T_S_FLAG)
	{
		case 1:
		{
			TIME_static=zhongxin_j/change_P_uW_doing;
			break;
		}

				default:
		{
			L_T_S_FLAG=6;
			
			break;
		}
	}
}
	
	else	//运行态
		
	{
		
		switch(L_T_S_FLAG)
	{
		case 1:
		{
			//TIME_static=((float)(qirongjiao_j *100- uJ_doing))	/ (14.3f*100) ;
			TIME_static=((float)(zhongxin_j - mJ_doing*1000))	/ (change_P_uW_doing) ;
			break;
		}

				default:
		{
			L_T_S_FLAG=6;
			
			break;
		}
	}
	
	}
}



float zhongxin_j=0.0;

void 	ZX_prepare()	//在当前页面 但未操作
{
	u16 ResTime_sec;
  u16 ResTime_min;	
	u16 ZX_mj;	


	ReadDGUS(0x5101,(uint8_t *)&ZX_mj,sizeof(ZX_mj));  	//读取杀灭log值 范围1~5
	switch(ZX_mj)
	{
		case 1:
		{
			zhongxin_j	=	1000;			//单位是uj 而不是mj
			break;
		}
				case 2:
		{
			zhongxin_j	=	2000;
			break;
		}
				case 3:
		{
			zhongxin_j	=	3000;
			break;
		}
				case 4:
		{
			zhongxin_j	=	5000;
			break;
		}
				case 5:
		{
			zhongxin_j	=	7000;
			break;
		}
	}
	
	ZX_mm_to_s(L_int);	//根据距离和功率得到时间
	
	ResTime_min=(u16)(TIME_static)/60;
	ResTime_sec=(u16)(TIME_static)%60;
	WriteDGUS(0x2820,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***总时间*分钟****/
	WriteDGUS(0x2800,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***剩余时间*分钟****/
	WriteDGUS(0x2805,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***剩余时间*秒****/
	WriteDGUS(0x2825,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***总时间*秒****/

}







//主要功能解锁触控单元
void 	ZX_Set()				//每次进入页面初始化
{

}



void 	ZX_stop()				//每次进入页面初始化
{

}


void ZX_Home()	//循环运行
{
	
		if(last_page!=CurrentPageNum[1])	//第一次进入页面
	{
		last_page=CurrentPageNum[1];	
		current_st = 0x01;
	}

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
			ZX_Set();
			
		//	break;
		}
		
		case 2:		//就绪态
		{
			ZX_prepare();
			break;
		}
		
				case 3:		//运行态开始
		{
			UV=1;
			ZX_run();
			break;
		}
		
				case 4:		//运行态暂停
		{
			UV=0;
			ZX_stop();
			break;
		}
		
				case 5:		//运行态结束 此为自然结束不包含复位
		{
			UV=0;
			ZX_done();	
			break;
		}
				case 6:		//运行态复位 只进行一次 然后跳转到第一次初始化
		{
			UV=0;
			ZX_Reset();	//只运行一次
			Unlock_current_page();//解锁
			current_st = 0x01;	// 置位 到第一次初始化		
			break;
		}
		
		default :
		{
			break;
		}
		
	}
	
}