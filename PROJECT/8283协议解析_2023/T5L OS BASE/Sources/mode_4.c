#include "mode_4.h"
//u8 MODE_4_Finish=0;	
u8 mode4_min[2];
u8 mode4_maichong_on[2];
u8 mode4_maichong_off[2];
u8 mode4_first_flag=0;
uint T_4_ON_OFF=0;
//u8 icon_exit[2]={0};
//uint mode4_1s=0;
float mj_mode4=0.0;
uint t_mode4=0;
// uint maichong_suanfa(uint T,u8 on,u8 off);
void Static_T_Current_mode4();

bit reset_mode4=0;
bit start_mode4=0;
void mode_4_Init()			//在按下开始后只进行一次
{
	EA=0;
		if(start_mode4==1	&& mode4_first_flag==0)
	{
		JumpPage(Page_shoudong_JUMP);
		MODE_4_Finish=0;
		MODE_Static=4;
		mode4_first_flag=1;
	//	mode_4_500ms=0;
		Static_T_Current_mode4();
		T_First=T_All;
		Draw_W_Flag=1;	
		
		shoudong_read_norflash();
		mode_4_flag_start=1;
	}
	
}
void mode_4_reset()
{	
	JumpPage(Page_shoudong);
	reset_mode4=0;
	WriteDGUS(0x3010,(uint8_t *)&icon_exit,sizeof(icon_exit));
	start_first_flag=0;
	start_mode4=0;
	mode_4_flag_start=0;	
	Unlock_current_page();
	Draw_W_Flag=0;
	norflash_Close_UV_Flag=1;
	mode4_first_flag=0;
	First_Draw_Flag=0;
	UV=0;
	Off_On_pwm_flag=0;
	//MODE_Static=0;
	T_4_ON_OFF=0;
	//mode4_1s=0;	

}

u8 MODE_4_Finish=0;	//1代表自己完成
void mode_4_run()
{
	EA=0;
	if(mode_4_flag_start==1)
	{
//		if(mode4_maichong_off[1]!=0	)
//		{
//			if(mode4_1s >= T_All)
//			{
//				mode_4_reset();MODE_4_Finish=1;
//			}
//			else if(Off_On_pwm_flag==0	&&	(T_4_ON_OFF>=mode4_maichong_on[1])&& mode4_1s!=0)//工作时间到了 要停止工作了
//			{
//				Off_On_pwm_flag=1;
//				T_4_ON_OFF=0;
//				UV=0;
//			}
//			else if((Off_On_pwm_flag==1) &&	T_4_ON_OFF>=mode4_maichong_off[1]&& mode4_1s!=0)		//休息时间到了 要开启工作了
//			{
//				Off_On_pwm_flag=0;	
//				UV=1;T_4_ON_OFF=0;
//			}
//			
//		}		
//		else 
//		{
//			UV=1;
////			if(mode4_1s >= T_All)
////			{
////				mode_4_reset();MODE_4_Finish=1;
////			}
//		}
		
	
	}
	else
		{		
			ReadDGUS(0x1495,(uint8_t *)&mode4_maichong_on,sizeof(mode4_maichong_on));		//读取on
			ReadDGUS(0x1500,(uint8_t *)&mode4_maichong_off,sizeof(mode4_maichong_off));		//读取off
		if(MODE_Static==0)	//未开启显示剩余时间
	{
		  Static_T_Current_mode4();
	}
		}
//EA=1;
}

void Static_T_Current_mode4()
{
	EA=0;
		if(MODE==4)
	{
	//Transform_L_T(L_int);
	ReadDGUS(0x1490,(uint8_t *)&mode4_min,sizeof(mode4_min));
	T_XIANSHI=(uint)(mode4_min[1]*60);
		
		if(mode4_maichong_off[1]!=0)	
		{
			t_mode4= mode4_maichong(T_XIANSHI,mode4_maichong_on[1],mode4_maichong_off[1]);
			mj_mode4=	t_mode4*P_FINISH;
		}
		else 	//常开
		{
			mj_mode4=T_XIANSHI*P_FINISH;
		}
			T_All=T_XIANSHI;							//写入总时间
	//change_P_mW_doing=P_FINISH;		//写入功率
	}
}

void mode4_isr()
{

	if((UV_St==0&&Off_On_pwm_flag==0)||(Off_On_pwm_flag&&UV_St))
	{
		T_doing++;
		if(T_XIANSHI>0){T_XIANSHI--;}
	//mode4_1s++;
	T_4_ON_OFF++;
	}
		
	
	
			if(mode4_maichong_off[1]!=0	)
		{
//			if(mode4_1s >= T_All)
//			{
//				mode_4_reset();MODE_4_Finish=1;
//			}
			 if(Off_On_pwm_flag==0	&&	(T_4_ON_OFF>=mode4_maichong_on[1])&& T_doing!=0)//工作时间到了 要停止工作了
			{
				Off_On_pwm_flag=1;
				T_4_ON_OFF=0;
				UV=0;
			}
			else if((Off_On_pwm_flag==1) &&	T_4_ON_OFF>=mode4_maichong_off[1]&& T_doing!=0)		//休息时间到了 要开启工作了
			{
				Off_On_pwm_flag=0;	
				UV=1;
				T_4_ON_OFF=0;
			}
			
		}		
		else 
		{
			UV=1;
		}
		
		
				if(	T_XIANSHI<=0)
				{
					mode_4_reset();MODE_4_Finish=1;
				}
				
	if(Off_On_pwm_flag==0&&MODE_4_Finish==0)
	{
		UV=1;
	}
}


/************治疗模式页面下 写入norflash，下次开机保留**********/
void shoudong_read_norflash()		//头帧 0xf6
{
	u16 buff_user[4]={0x00f6,0,0,0};
EA=0;
	
		ReadDGUS(0x1490, (uint8_t *)&buff_user[1],sizeof(buff_user[1]));	
		ReadDGUS(0x1495, (uint8_t *)&buff_user[2],sizeof(buff_user[2]));	
		ReadDGUS(0x1500, (uint8_t *)&buff_user[3],sizeof(buff_user[3]));	
		
		WriteNorFlash_Str(Nor_shoudong_user_Ad, (uint8_t *)&buff_user,4);	//写入norflash
		
}


void set_shoudong_Init()
{
		u16 buff_user[4];

					ReadNorFlash_Str(Nor_shoudong_user_Ad,(uint8_t*)&buff_user,4);			//加载上次 手动录入三个变量
				if(buff_user[0]==0x00f6)
				{
					WriteDGUS(0x1490, (uint8_t *)&buff_user[1],sizeof(buff_user[1]));	
					WriteDGUS(0x1495, (uint8_t *)&buff_user[2],sizeof(buff_user[2]));	
					WriteDGUS(0x1500, (uint8_t *)&buff_user[3],sizeof(buff_user[3]));	
				}			
}