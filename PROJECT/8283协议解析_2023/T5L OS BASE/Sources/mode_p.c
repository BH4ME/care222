#include "T5LOS8051.H"
#include "sys.h"
#include "handle.h"
//#include<stdio.h>
#include "math.h"
#include "mode_p.h"
#include "suanfa.h"
#include "start_win.h"


uint L_T_FLAG=0;
double P_mW_T=0.00;

uint l_lasttime;

#define W_Mode_18mJ 18
double W_18mJ=0.018;
double W_Mode;
double W_Static;
double W_FINISH;
uint First_shuju_FLAG=0;			/*****0需要进行第一次数据*****/
double W_LASTTIME=0.0;
u8 MODE_2_Finish=0;		//模式2 自己时间消耗完 自动结束 0代表未结束，1代表自动结束模式1
		
		u8 MODE_2_RUN_Draw=0;	//模式2 自己消耗完时间进行 第三条曲线汇总曲线进行画
	

uint T_FINISH;
uint ZIWAIXIAN_FINISH_FLAG=0;  /******0为未开启紫外线********/
uint T_LASTTIME=0;
uint T_WORK=0;
uint T_XIANSHI=0;
double P_FINISH=1;
//double P_FINISH=1;
double P_LASTTIME=1;

uint a=0;
uint T_Static_MODE2=0;
uint T_P_First=0;

u8 PWM_ON[2];
u8 PWM_OFF[2];

uint T_ON_OFF=0;
uint T_dynamic=0;

u16 dynamic_static_flag=0;	//动态时间和静态时间的标志位
void T_jisuan(void);
void shujuchuli(void);
void First_shuju(void);
void Transform_L_T(uint L_T);
void UART5_isr();
void TIM0_suanfa_isr();
void shuju_bianliang_chongzhi(void);
void Mode_p_work(void);

void CLOSE_W_bianliangchongzhi(void);
		
		

//		
///**********转换函数**输入距离mm***转换成所需要的功率mW******/
//void Transform_L_T(uint L_T)	/*****此时距离是mm单位*******/
//{
//	

//	P_FINISH=change_P_mW_doing/1000.000;		//变成uw
//}
	

void First_shuju(void)
{
	//高字节vp
			
	EA=0;

	if(First_shuju_FLAG==0)
	{
		MODE_2_RUN_Draw=0;
		l_lasttime=l_mode2;
	 W_Static=W_FINISH;
	//	Transform_L_T(L_int);
		//T_jisuan();
		Static_T_Current_mode2();

		First_shuju_FLAG=1;
		T_dynamic=T_All;
//		T_XIANSHI=T_y;
//		T_S_First=T_y;
		T_First=T_All;
		P_LASTTIME=P_FINISH;
		Draw_W_Flag=1;
		//T_FINISH=W_Static/P_FINISH;
	}
//	EA=1;
}




///*********W=P*T*****************T=W/P*****T_FINISH*******P_mW_T*****P=mW*****W=mJ*/
void T_jisuan(void)
{
//	EA=0;
//	static l_lasttime;
//	W_LASTTIME=P_LASTTIME*T_WORK;
//	if(W_Static>=W_LASTTIME)
//	{
//		W_Static=W_Static-W_LASTTIME;T_XIANSHI=W_Static/P_FINISH;
//		T_XIANSHI=maichong_suanfa(T_XIANSHI,PWM_ON[1],PWM_OFF[1]);
//		T_dynamic=T_XIANSHI+T_doing;
//		T_WORK=0;
//	}	
//	else return;
//	P_LASTTIME=P_FINISH;
	W_LASTTIME=P_LASTTIME;
	if(W_Static>=W_LASTTIME)
	{
		W_Static=W_Static-W_LASTTIME;
		if(l_lasttime<=l_mode2-2	||l_lasttime>=l_mode2+2	)//变化
		{
		T_XIANSHI=W_Static/P_FINISH;
		T_XIANSHI=maichong_suanfa(T_XIANSHI,PWM_ON[1],PWM_OFF[1]);
		T_dynamic=T_XIANSHI+T_doing;
		}
	}	
	else return;
	P_LASTTIME=P_FINISH;
	l_lasttime=l_mode2;
//	EA=1;
}

void Mode_p_work(void)				/***********检测到距离变化*******/
{
		//	Transform_L_T(L_int);
		//	T_jisuan();
}

uint Off_On_pwm_flag=0;	//0表示正在工作
void TIM0_1S_isr(void)
{
//	EA=0;
	if(dynamic_static_flag)	T_jisuan();

	if(PWM_OFF[1]!=0	&&T_doing!=0)
	{	
				if(T_ON_OFF>=PWM_ON[1]	&&	Off_On_pwm_flag==0)	//工作时间到了 要停止工作了
		{
			Off_On_pwm_flag=1;
			UV=0;
			T_ON_OFF=0;
		}

	else 	if(Off_On_pwm_flag==1 &&	T_ON_OFF>=PWM_OFF[1])		//休息时间到了 要开启工作了
		{
			Off_On_pwm_flag=0;	
			UV=1;
			T_ON_OFF=0;
		}
		
		
	}
	else //常开
	{		
			UV=1;
	}
	
	if(Off_On_pwm_flag==0&&MODE_2_Finish==0)
	{
		UV=1;
	}
	
	
				if((UV_St==0&&Off_On_pwm_flag==0)||(Off_On_pwm_flag&&UV_St))
	{
	
	T_doing++;
	T_WORK++;
	if(T_XIANSHI>0)	T_XIANSHI--;
	T_ON_OFF++;
	}
	
	if(T_XIANSHI<=0 ||	T_doing>=T_dynamic)
	{
			JumpPage(Page_zhiliao); MODE_2_Finish=1;	
		//Reset_flag=1;
		
		if(dynamic_static_flag)MODE_2_RUN_Draw=1;
		
		CLOSE_W_bianliangchongzhi();
		
		
	}
	else if(T_doing>=2*T_First)
	{
			Draw_W_Flag=0;
	}

//	EA=1;
}

bit reset_mode2=0;
void CLOSE_W_bianliangchongzhi(void)
{
	//EA=0;
//Reset_flag
	if(MODE==2)	JumpPage(Page_zhiliao);
	reset_mode2=0;
	W_P_uJ[2]=0x01;P_FINISH=1;P_LASTTIME=1;
	//uchar CLOSE[1]={0x0000};
	ZIWAIXIAN_FINISH_FLAG=0;
	T_FINISH=0;
	T_WORK=0;
	T_XIANSHI=0;
	First_shuju_FLAG=0;
	shuju_change=0;
	L_T_FLAG=0;
	//MODE=0;
	//MODE_Static=0;
	norflash_Close_UV_Flag=1;
	//Work_Time=0;
	UV=0;
	T_P_First=0;
	Draw_W_Flag=0;//停止画线
	OPEN_P=0;
		First_Draw_Flag=0;	//开启画线
	W_mJ_XIANSHI=0;
	Off_On_pwm_flag=0;
	//T_doing=0;	//画进度条 清零
	//	 Draw_Reset();
	//	Unlock_current_page();

		T_ON_OFF=0;
		//T_dynamic=0;
		start_first_flag=0;
		//EA=1;
}
bit yichu_T=0;
void Static_T_Current_mode2()
{
	if(MODE==2)
	{
//		EA=0;
	//	P_FINISH=change_P_mW_doing/1000.000;		//变成uw
	//Transform_L_T(L_int);
	T_XIANSHI=(W_FINISH/P_FINISH);
		
		//第一次计算溢出
	//if((W_FINISH/P_FINISH)>=0xef10){yichu_T=1;return;}//溢出
		if(PWM_OFF[1]!=0)
		{
			T_XIANSHI=maichong_suanfa(T_XIANSHI,PWM_ON[1],PWM_OFF[1]);
			yichu_run();
		yichu_set();
		}
		else 
		{
			
		}
	T_All=T_XIANSHI;
	T_dynamic=T_All;						//写入总时间
	//change_P_mW_doing=P_FINISH;		//写入功率
//		EA=1;
	}
}

static struct{
	u8 Head_set_H;
	u8 H_mj;
	u8 H_on;
	u8 H_off;
	
	u8 Head_set_M;
	u8 M_mj;
	u8 M_on;
	u8 M_off;
	
	u8 Head_set_L;
	u8 L_mj;
	u8 L_on;
	u8 L_off;

}zhiliao_set_nor;

void zhiliao_SET()
{
	
	uchar MJ_H[4];	//2023 02 18  年月日
								//0	 1	2	 3
	u8 MJ_M[4];	//00	12 45 23	时分秒
								//0		1	 2	3
	u8	MJ_L[4];
 uchar MJ_H_M_L[12]={0};
//u8 Nor_buff[10];	//0xf7
	u32 MJ_H_HEX,MJ_L_HEX,MJ_M_HEX;
	
 /***********固定头*************/
zhiliao_set_nor.Head_set_H=0XF0;
zhiliao_set_nor.Head_set_M=0XF1;
zhiliao_set_nor.Head_set_L=0XF2;

	ReadDGUS(0X1310,(uint8_t *)&MJ_H,sizeof(MJ_H));
	ReadDGUS(0X1450,(uint8_t *)&MJ_M,sizeof(MJ_M));
	ReadDGUS(0X1320,(uint8_t *)&MJ_L,sizeof(MJ_L));
// Nor_buff[0]=0xf7;			//头枕=帧0xf7
// Nor_buff[1]=MJ_H[1];
// Nor_buff[2]=MJ_H[2];
// Nor_buff[3]=MJ_H[3];
// Nor_buff[4]=MJ_M[1];
// Nor_buff[5]=MJ_M[2];
// Nor_buff[6]=MJ_M[3];
// Nor_buff[7]=MJ_L[1];
// Nor_buff[8]=MJ_L[2];
// Nor_buff[9]=MJ_L[3];
		
		//WriteNorFlash_Str(Nor_zhiliao_set_initial_1_Ad, (uint8_t *)&Nor_buff,6);		//写入norflash中未经过算法处理的源码
 			WriteNorFlash_Dgus(Nor_zhiliao_set_initial_3_Ad, 0X1320,2);	
 			WriteNorFlash_Dgus(Nor_zhiliao_set_initial_2_Ad, 0X1450,2);	
			WriteNorFlash_Dgus(Nor_zhiliao_set_initial_1_Ad, 0X1310,2);
 
		MJ_H_HEX=(u32)(MJ_H[0])<<24	|	(u32)(MJ_H[1])<<16	| (u32)(MJ_H[2])<<8	| (u32)(MJ_H[3]);
		MJ_M_HEX=(u32)(MJ_M[0])<<24	|	(u32)(MJ_M[1])<<16	| (u32)(MJ_M[2])<<8	| (u32)(MJ_M[3]);
		MJ_L_HEX=(u32)(MJ_L[0])<<24	|	(u32)(MJ_L[1])<<16	| (u32)(MJ_L[2])<<8	| (u32)(MJ_L[3]);
	
	
	zhiliao_set_nor.H_mj=(MJ_H_HEX/10000)%1000;
	zhiliao_set_nor.H_on=(MJ_H_HEX/100)%100;
	zhiliao_set_nor.H_off=MJ_H_HEX%100;
	
	zhiliao_set_nor.M_mj=(MJ_M_HEX/10000)%1000;
	zhiliao_set_nor.M_on=(MJ_M_HEX/100)%100;
	zhiliao_set_nor.M_off=MJ_M_HEX%100;

	
	zhiliao_set_nor.L_mj=(MJ_L_HEX/10000)%1000;
	zhiliao_set_nor.L_on=(MJ_L_HEX/100)%100;
	zhiliao_set_nor.L_off=MJ_L_HEX%100;

			WriteNorFlash_Str(Nor_zhiliao_set_Ad, (uint8_t *)&zhiliao_set_nor,6);	
			WriteDGUS(0X3200, (uint8_t *)&zhiliao_set_nor,sizeof(zhiliao_set_nor));			//测试
}

void zhiliao_Choose_Mode_1()		//在治疗页面进行
{
	u8 icon_esc[2]={0};
	u8 receive_buff[2];
	u8 set_buff[4];
	u16 buff_zhiliao[3];
	ReadDGUS(0x2440,(uint8_t *)&receive_buff,sizeof(receive_buff));
	if(receive_buff[1]==0x01)	//这次是开启
	{
		WriteDGUS(0x2450, (uint8_t *)&icon_esc,sizeof(icon_esc));
		WriteDGUS(0x2460, (uint8_t *)&icon_esc,sizeof(icon_esc));
		ReadDGUS(SET_zhiliao_moshi_Adress,(uint8_t *)&set_buff,sizeof(set_buff));	//读取设置的缓冲空间
		if(set_buff[0]==0xf0)
		{
			buff_zhiliao[0]=0x00<<8|set_buff[1];
			buff_zhiliao[1]=0x00<<8|set_buff[2];
			buff_zhiliao[2]=0x00<<8|set_buff[3];
		WriteDGUS(0x2400, (uint8_t *)&buff_zhiliao[0],sizeof(buff_zhiliao[0]));		//毫焦
		WriteDGUS(0x2410, (uint8_t *)&buff_zhiliao[1],sizeof(buff_zhiliao[1]));		//脉冲on
		WriteDGUS(0x2420, (uint8_t *)&buff_zhiliao[2],sizeof(buff_zhiliao[2]));		//脉冲off
		}
	}
	
}

void zhiliao_Choose_Mode_2()		//在治疗页面进行
{
	u8 icon_esc[2]={0};
	u8 receive_buff[2];
	u8 set_buff[4];
	u16 buff_zhiliao[3];
	ReadDGUS(0x2450,(uint8_t *)&receive_buff,sizeof(receive_buff));
	if(receive_buff[1]==0x01)	//这次是开启
	{
		WriteDGUS(0x2440, (uint8_t *)&icon_esc,sizeof(icon_esc));
		WriteDGUS(0x2460, (uint8_t *)&icon_esc,sizeof(icon_esc));
		ReadDGUS(SET_zhiliao_moshi_Adress+0x2,(uint8_t *)&set_buff,sizeof(set_buff));	//读取设置的缓冲空间
		if(set_buff[0]==0xf1)
		{
			buff_zhiliao[0]=0x00<<8|set_buff[1];
			buff_zhiliao[1]=0x00<<8|set_buff[2];
			buff_zhiliao[2]=0x00<<8|set_buff[3];
		WriteDGUS(0x2400, (uint8_t *)&buff_zhiliao[0],sizeof(buff_zhiliao[0]));		//毫焦
		WriteDGUS(0x2410, (uint8_t *)&buff_zhiliao[1],sizeof(buff_zhiliao[1]));		//脉冲on
		WriteDGUS(0x2420, (uint8_t *)&buff_zhiliao[2],sizeof(buff_zhiliao[2]));		//脉冲off
		}
	}
	
}

void zhiliao_Choose_Mode_3()		//在治疗页面进行
{
	u8 icon_esc[2]={0};
	u8 receive_buff[2];
	u16 buff_zhiliao[3];
	u8 set_buff[4];
	ReadDGUS(0x2460,(uint8_t *)&receive_buff,sizeof(receive_buff));
	if(receive_buff[1]==0x01)	//这次是开启
	{
		WriteDGUS(0x2450, (uint8_t *)&icon_esc,sizeof(icon_esc));
		WriteDGUS(0x2440, (uint8_t *)&icon_esc,sizeof(icon_esc));
		ReadDGUS(SET_zhiliao_moshi_Adress+0x4,(uint8_t *)&set_buff,sizeof(set_buff));	//读取设置的缓冲空间
		if(set_buff[0]==0xf2)
		{
			
			buff_zhiliao[0]=0x00<<8|set_buff[1];
			buff_zhiliao[1]=0x00<<8|set_buff[2];
			buff_zhiliao[2]=0x00<<8|set_buff[3];
		WriteDGUS(0x2400, (uint8_t *)&buff_zhiliao[0],sizeof(buff_zhiliao[0]));		//毫焦
		WriteDGUS(0x2410, (uint8_t *)&buff_zhiliao[1],sizeof(buff_zhiliao[1]));		//脉冲on
		WriteDGUS(0x2420, (uint8_t *)&buff_zhiliao[2],sizeof(buff_zhiliao[2]));		//脉冲off
		}
	}
	
}

void set_zhiliao_Init()
{
		u16 buff_user[4];
	//	u8 check[6];
//	u8 Nor_buff[12];
//	u8 H[3];
//	u8 M[3];
//	u8 L[3];
	ReadNorFlash_Dgus(Nor_zhiliao_set_Ad,0x3200,6);	//读取到虚拟地址 0x3200
	
//	
//	ReadNorFlash_Str(Nor_zhiliao_set_initial_1_Ad,(uint8_t *)&Nor_buff,6);		//写入norflash中未经过算法处理的源码
//	if(Nor_buff[0]==0xf7)
//	{
//		H[0]=Nor_buff[1];
//		H[1]=Nor_buff[2];
//		H[2]=Nor_buff[3];
//		M[0]=Nor_buff[4];
//		M[1]=Nor_buff[5];
//		M[2]=Nor_buff[6];
//		L[0]=Nor_buff[7];
//		L[1]=Nor_buff[8];
//		L[2]=Nor_buff[9];
//		
//		WriteDGUS(0x1300, (uint8_t *)&H,sizeof(H));	
//		WriteDGUS(0x1315, (uint8_t *)&M,sizeof(M));	
//		WriteDGUS(0x1320, (uint8_t *)&L,sizeof(L));	
//	}
					ReadNorFlash_Str(Nor_zhiliao_user_Ad,(uint8_t*)&buff_user,4);			//加载上次 手动录入三个变量
				if(buff_user[0]==0x00f6)
				{
					WriteDGUS(0x2400, (uint8_t *)&buff_user[1],sizeof(buff_user[1]));	
					WriteDGUS(0x2410, (uint8_t *)&buff_user[2],sizeof(buff_user[2]));	
					WriteDGUS(0x2420, (uint8_t *)&buff_user[3],sizeof(buff_user[3]));	
					//WriteDGUS(0x1700, (uint8_t *)&buff_user,sizeof(buff_user));	
				}				
 			ReadNorFlash_Dgus(Nor_zhiliao_set_initial_3_Ad, 0X1320,2);	
 			ReadNorFlash_Dgus(Nor_zhiliao_set_initial_2_Ad, 0X1450,2);	
			ReadNorFlash_Dgus(Nor_zhiliao_set_initial_1_Ad, 0X1310,2);
				
	//	ReadDgus(0x1310,(uint8_t*)&check,sizeof(check));
		
 	
}


/************治疗模式页面下 用户自动调整 不是采用三个快捷键 按下开始后写入norflash，下次开机保留**********/
void zhiliao_read_norflash()		//头帧 0xf6
{
	u16 buff_read[3];
	u16 buff_user[4]={0x00f6,0,0,0};

	ReadDGUS(0x2440,(uint8_t *)&buff_read[0],sizeof(buff_read[0]));		//检测三个按键
	ReadDGUS(0x2450,(uint8_t *)&buff_read[1],sizeof(buff_read[1]));
	ReadDGUS(0x2460,(uint8_t *)&buff_read[2],sizeof(buff_read[2]));	
	if(buff_read[0]==0	&&	buff_read[1]==0	&&	buff_read[2]==0)		//用户自己调整没有按下快捷键
	{
	
		ReadDGUS(0x2400, (uint8_t *)&buff_user[1],sizeof(buff_user[1]));	
		ReadDGUS(0x2410, (uint8_t *)&buff_user[2],sizeof(buff_user[2]));	
		ReadDGUS(0x2420, (uint8_t *)&buff_user[3],sizeof(buff_user[3]));	
		
		WriteNorFlash_Str(Nor_zhiliao_user_Ad, (uint8_t *)&buff_user,4);	//写入nrflash
		
			//	WriteDGUS(0x3210, (uint8_t *)&buff_user,sizeof(buff_user));	
	}
}



/********************治疗页面 时间随着距离的改变而动态变化***************
控制SP地址 0x1280
**********************************************************************/
