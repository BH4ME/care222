#include "T5LOS8051.H"
#include "sys.h"
#include "handle.h"
#include <stdio.h>
#include "math.h"
#include "mode_p.h"

uint L_suanfa[10]={0,0,0};
uint L_i=0;
uint16_t L_s2[10]={0,0,0};
uint16_t L_q[10]={0,0,0};
uint L_3FLAG=0;
bit L_ARRAY_FINISH=0;		/****0未完成*****/

double sum = 0;//求和
uint array[4] = {0.0,0.0,0.0,0.0};
int length = 0;//数组长度
double average = 0;//求平均数
double var = 0.0; //求方差
double standard = 0; //求标准差


bit shuju_change=0;		/****0未变更数据******/

uint L_FINISH=0;

uint T_06S=0;
uint jiaoyan[9] = {0.0,0.0,0.0,0.0,0.0};
double change_P_mW=0.0;
float change_P_mW_doing=0.0;
double change_P_mW_First=0.0;
u16 attend_sp_error  =0x4610;

u16  attend_sp_min_0 =0x2800;
u16 attend_sp_sec_0  =0x2805;
u16  attend_sp_min_1 =0x2820;
u16 attend_sp_sec_1  =0x2825;
u16  attend_sp_0 =0x1710;
u16 attend_sp_1  =0x1810;u8 exit_sp[2]={0xff,0x00};


//溢出
 bit error_flag;
//u72 abbbb;
void shujuchuli(void);
void shuju_bianliang_chongzhi(void);


void TIM0_suanfa_isr(void)
{
	EA=0;
	L_ARRAY_FINISH=0;
	L_suanfa[L_i]=L_int;
	
	if(L_3FLAG>=2)					/***********flag=2,i=2****/
{
		
			switch(L_i)
	{
		case 0:
		{
			array[0]=L_suanfa[2]/10;
			array[1]=L_suanfa[3]/10;
			array[2]=L_suanfa[0]/10;
			L_ARRAY_FINISH=1;
			break;
		}
		case 1:
		{
			array[0]=L_suanfa[3]/10;
			array[1]=L_suanfa[0]/10;
			array[2]=L_suanfa[1]/10;
		L_ARRAY_FINISH=1;
			break;
		}
				case 2:
		{
			array[0]=L_suanfa[0]/10;
			array[1]=L_suanfa[1]/10;
			array[2]=L_suanfa[2]/10;
		L_ARRAY_FINISH=1;
			break;
		}
				case 3:
		{
	L_ARRAY_FINISH=1;
			array[0]=L_suanfa[1]/10;
			array[1]=L_suanfa[2]/10;
			array[2]=L_suanfa[3]/10;
			break;
		}
		default:
		{
			
		}
	}
	
	shujuchuli();
}


	if(L_i>=3) {L_i=0;}		/***取0~4九组子数据组成三组父数据****/
	else {L_i++;L_3FLAG++;}
//	EA=1;

}

///****************array cm**********数据处理****根据所得出的数据判断是否需要更改时间***********/
void shujuchuli(void)
    {
			int a=0;
			int i=0;
			int j=0;
			double average_jiaoyan=0.0;
			double var_jiaoyan=0.0;
//			EA=0;
			if(L_ARRAY_FINISH==1)				/****接受数据到ARRAY完成 进行数据分析是否更改时间****/
			{
			//shuju_bianliang_chongzhi();
        for ( i = 0; i <= 2;i++)
        {
            sum += array[i];//求和
        }
 
        length = 3;//求数组长度
        average = sum / length;//求平均值
 
        for ( j = 0; j <= 2;j++)
        {
            var += pow(array[j]-average,2)/length;//求方差
        }
    
        //standard = pow(var,0.5);//求标准差
				//a=var;
				
				if(var<=3)
				{
					shuju_change=0;			/*********数据不变化，不需要作出反应******/
					//jiaoyan[T_06S]=L_int;
					//T_06S++;
	//				if(T_06S>=8)
//					{
//							average_jiaoyan=(jiaoyan[0]+jiaoyan[8])/2;
//						var_jiaoyan = pow(jiaoyan[0]-average_jiaoyan,2)/2+pow(jiaoyan[8]-average_jiaoyan,2)/2;
//						if(var_jiaoyan>=2)
//						{shuju_change=1;	L_FINISH = L_int;}
//						T_06S=0;
//					}
				}
				else {shuju_change=1;	Mode_p_work();
					//L_FINISH = array[2]*10;
				//T_06S=0;
					}
				
			//	shuju_bianliang_chongzhi();
   
//        printf("sum = %g\n",sum);//这边打印也可以使用printf("%f\n",sum);这种打印默认出来的数是小数点后6位
//        printf("length of array[4] = %d\n",length);
//        printf("average = %g\n",average);
//        printf("var = %g\n",var);
//        printf("standard = %g\n",standard);
				
				
				
			}
			
			
//			if(shuju_change==1)
//			{
//				if(MODE_Static==2)
//				{}
////				else if(MODE_Static==1)
////				{Mode_s_work();}
//			}
			
			EA=1;

    }
void shuju_bianliang_chongzhi(void)
{
	sum=0.0;
	standard=0.0;
	var=0.0;
	average=0.0;shuju_change=0;
}


/*********************************************给出距离算出功率*************************************/
//void L_mm_TO_P_mw(uint L_mm_TO_P)
//{
//	uint L_mm_TO_P_mw_FLAG=0;
//		double BEISHU=0.000;
//	double L_CM;
//	double P_mW_50CM=8.36;	
//	double P_mW_100CM=3.2;	
//	double P_mW_1500CM=0.930;	
//	double P_mW_200CM=0.520;	
//L_CM=L_mm_TO_P/10;
//	
//	if(L_CM<50	&& L_CM>=10) L_mm_TO_P_mw_FLAG=1;
//	else if(L_CM>=50 && L_CM<100 )	L_mm_TO_P_mw_FLAG=2;
//		else if(L_CM>=100 && L_CM<150 )	L_mm_TO_P_mw_FLAG=3;
//			else if(L_CM>=150	&& L_CM<=400)	L_mm_TO_P_mw_FLAG=4;
//	else if(L_CM<=10)	L_mm_TO_P_mw_FLAG=6;
//	else if(L_CM>=400)L_mm_TO_P_mw_FLAG=7;
//				else L_mm_TO_P_mw_FLAG=5;
//	
//	
//	switch(L_mm_TO_P_mw_FLAG)
//	{
//		case 1:
//		{
//			BEISHU=50.00/L_CM;
//			BEISHU=BEISHU*BEISHU;
//			change_P_mW_doing=P_mW_50CM*BEISHU;
//			break;
//		}
//		case 2:
//		{	
//			double P_uW=0.0;
//			double L_M=0.0;
//			L_M = L_mm_TO_P/1000.0;
//			L_M=L_M*(1.92);
//			P_uW=1.0/(exp(L_M));
//			P_uW=21.84*P_uW;		/***60cm 6.67**/
//			change_P_mW_doing=P_uW;

//			break;
//		}
//				case 3:
//		{

//			double P_uW=0.0;
//			double L_M=0.0;
//			L_M = L_mm_TO_P/1000.0;
//			L_M=L_M*(2.47);
//			P_uW=1.0/(exp(L_M));
//			P_uW=37.89*P_uW;		
//			change_P_mW_doing=P_uW;
//			break;
//		}
//				case 4:
//		{
//	
//			BEISHU=L_CM/150.0;
//			BEISHU=BEISHU*BEISHU;
//			change_P_mW_doing=P_mW_1500CM/BEISHU;
//			break;
//		}
//				case 5:
//		{
//			
//	
//			break;
//		}
//		
//						case 6:
//		{
//			
//	change_P_mW_doing=200;
//			break;
//		}
//		
//						case 7:
//		{
//				change_P_mW_doing=0.13;
//	
//			break;
//		}
//		default:
//		{
//			L_mm_TO_P_mw_FLAG=0x0a;
//		}
//	}
//		change_P_mW_First=change_P_mW_doing/1000;		//uw变mw
//	//change_P_mW=((log(change_P_mW))+2)*13;	//实时曲线 W119,H98,Y_Cectral 291,VD50,纵轴放大倍数256，间距1数据源通道0
//	change_P_mW=log10(change_P_mW_doing);
//	P_FINISH=change_P_mW_First;
//}
/**********************************
change_P_mW_First 是动态的真实mW
change_P_mW	动态真实的log后的uW
***********************************/
void L_mm_TO_P_mw(uint L_mm_TO_P)
{
	uint L_mm_TO_P_mw_FLAG=0;
		double BEISHU=0.00000;
	double L_CM;
//	double P_mW_50CM=8.36;	
//	double P_mW_100CM=3.2;	
//	double P_mW_1500CM=0.930;	
//	double P_mW_200CM=0.520;

		double mihanshu=0.0;
L_CM=L_mm_TO_P/10;
	
	if(L_CM<=10&& L_CM>=5) L_mm_TO_P_mw_FLAG=1;
		else 	if(L_CM<=20&& L_CM>10) L_mm_TO_P_mw_FLAG=2;
			else 	if(L_CM<=100&& L_CM>20) L_mm_TO_P_mw_FLAG=3;
			else 	if(L_CM<=300&& L_CM>100) L_mm_TO_P_mw_FLAG=4;
				else L_mm_TO_P_mw_FLAG=5;
	
	/***66300.18x^-1.86****/
	switch(L_mm_TO_P_mw_FLAG)
	{
		case 1:
		{
//			mihanshu = pow((double)(L_CM),1.86);
//			BEISHU = 66300.18/mihanshu;
//			change_P_mW_doing=BEISHU;
			mihanshu = pow((double)(L_CM),-1.6);
			BEISHU = 36527.86*mihanshu;
			change_P_mW_doing=BEISHU;
			break;
		}
		
		
				case 2:
		{
			mihanshu = pow((double)(L_CM),-1.85);
			BEISHU = 65617.44*mihanshu;
			change_P_mW_doing=BEISHU;
			break;
		}
		
				case 3:
		{
			mihanshu = pow((double)(L_CM),-1.81);
			BEISHU = 57622.25*mihanshu;
			change_P_mW_doing=BEISHU;
			break;
		}
		
				case 4:
		{
			mihanshu = pow((double)(L_CM),-2);
			BEISHU = 139224.07*mihanshu;
			change_P_mW_doing=BEISHU;
			break;
		}
		
		
		default:
		{
			L_mm_TO_P_mw_FLAG=0x0a;
		}
	}
		if(W_S[0]!=2&&MODE==1)change_P_mW_doing=change_P_mW_doing*0.6;
		change_P_mW_First=change_P_mW_doing/1000;		//uw变mw
	//if(MODE==2)	P_FINISH=change_P_mW_doing/1000.000;		//变成uw
	//change_P_mW=((log(change_P_mW))+2)*13;	//实时曲线 W119,H98,Y_Cectral 291,VD50,纵轴放大倍数256，间距1数据源通道0
	change_P_mW=log10(change_P_mW_doing);
	P_FINISH=change_P_mW_First;//uw变mw
}


/*********************状态灯*****************
说明：1）在UI上显示，若没有正在进行的任务则显示空白
	若开始启动程序但是UV为开，即显示绿点
	若UV开则显示红点

	2）指示灯：若启动程序则显示绿灯，若UV开显示红灯 两个兼容

	3）激光：除了首页、设置界面、连接界面，其他界面开始前都开启，
	若屏幕背光打开，则关闭激光
*******************************************/
//bit jiguang_flag=0	//0表示未开启即要关闭激光 1表示开启激光
u8 start_first_flag=0;	//只有复位和自动完成的情况下 会置位
void LED_jiguang_Check()
{
	u8 ui[2];

	static u8 work_uv[2]={0,0x2};
	static u8 work_start[2]={0,0x1};
	static bit shandong_flag=0;
	EA=0;
	if(Page_s[1]!=Page_home	&&Page_s[1]!=Page_lianji	&&Page_s[1]!=Page_shezhi	)
	{
		//UI
		ReadDGUS(0x3010,(uint8_t *)&ui,sizeof(ui));
		if(UV_St==1&&ui[1]==0x01&&Page_s[1]==Page_chengxuhua_JUMP)start_first_flag=1;
		if(UV_St==0&&ui[1]==0x01	)	//uv work
		{
			start_first_flag=1;
			WriteDGUS(0x1700,(uint8_t *)&work_uv,sizeof(work_uv));
			LED_B=1; LED_G=0;
		}
		else if(UV_St==1&&ui[1]==0x01)	{LED_B=0; LED_G=1;WriteDGUS(0x1700,(uint8_t *)&work_start,sizeof(work_start));}
		else if(ui[1]==0x00&&UV_St==1&&start_first_flag==0){LED_B=0; LED_G=0;WriteDGUS(0x1700,(uint8_t *)&icon_exit,sizeof(icon_exit));}
		else if(start_first_flag==1&&ui[1]==0x00)
		{
			if(shandong_flag==0)
			{shandong_flag=~shandong_flag;
		LED_B=0; LED_G=1;WriteDGUS(0x1700,(uint8_t *)&work_start,sizeof(work_start));
			}
			else {shandong_flag=~shandong_flag;LED_B=0; LED_G=0;WriteDGUS(0x1700,(uint8_t *)&icon_exit,sizeof(icon_exit));}
		}
		//else {}
	//	if(UV_St==1)LED_B=0;
		
//		
//		if(ui[1]==0x01)	//开启
//		{
//			LED_G=1;
//		}
//		else LED_G=0;
		
		
		if(Screen_Touch_flag==0 && Screen_Time_500ms<=60)	//30s
		{
			JIGUANG=1;
		}else JIGUANG=0;
			
	}
	else {JIGUANG=0;LED_B=0; LED_G=0;UV=0;}
	EA=1;
}

/********************脉冲***********************
说明：t不间断照射时间		on上升沿 	off下降沿   T总共需要时间   T1周期1 T2

1.t%on!=0		则T=(t/on)*(on+off)+t%on
2.t%on==0 	T=(t/on-1)*(on+off)+on
例 不间断照射10s 上升沿1s 下降沿100s ,计算 
***********************************************/
uint maichong_suanfa(uint T,u8 on,u8 off)	//输入 不间断照射的时间
{
	uint T2;
	uint T1;
	uint T3;
//EA=0;
	if(MODE==2	||	MODE==4)		
	{
		T2=T%on;
		T3=T/on;
		if(T2!=0)
		{
			T1=T3*(on+off);
			if(T1/(on+off)!=T3){yichu_T=1;return 1;}
			T1+=T2;yichu_T=0;
			//判断溢出 
			
			//if((T1+T2)>=0xef10){yichu_T=1;return 0;}


			return T1;
		}
		else
		{
			
			
			T1=(T3-1)*(on+off);
			if(T1/(on+off)!=(T3-1)){yichu_T=1;return 1;}
			
			T1+=on;yichu_T=0;
			
			//if((T1+on)>=0xef10){yichu_T=1;return 0;}
			
			return T1;
		}
		
	}
	return T;
//EA=1;
}


uint mode4_maichong(uint T,uint on,uint off)
{
	
	uint t1;
	uint t2;
	
	t1=T/(on+off);
	
	if(t1==0)//不到一个周期
	{
		if(T>=on)	return on;
		else return T;
	}
	else 
	{
		t2=t1-(t1*(on+off));
		if(t2>=on)	return (t1*on+on);
			else return (t1*on+t2);
	}
	return 0;
}


void yichu_run()
{
	
				if(yichu_T==1)	//6个全部消失 出现error
		{
			error_flag=1;
			WriteDGUS(0x1720,(uint8_t *)&(exit_sp),sizeof(exit_sp));
			WriteDGUS(0x4630,(uint8_t *)&(exit_sp),sizeof(exit_sp));
			WriteDGUS(0x4640,(uint8_t *)&(exit_sp),sizeof(exit_sp));
			
			WriteDGUS(0x4650,(uint8_t *)&(exit_sp),sizeof(exit_sp));
			WriteDGUS(0x4660,(uint8_t *)&(exit_sp),sizeof(exit_sp));
			WriteDGUS(0x1820,(uint8_t *)&(exit_sp),sizeof(exit_sp));
			
			WriteDGUS(0x4620,(uint8_t *)&(attend_sp_error),sizeof(attend_sp_error));
		}
		
		else 
		{WriteDGUS(0x1820,(uint8_t *)&(exit_sp),sizeof(exit_sp));}

}

void yichu_set()
{
	
	if(error_flag==1&&yichu_T==0)
{
	error_flag=0;
			WriteDGUS(0x1720,(uint8_t *)&(attend_sp_0),sizeof(attend_sp_0));
			WriteDGUS(0x4630,(uint8_t *)&(attend_sp_min_0),sizeof(attend_sp_min_0));
			WriteDGUS(0x4640,(uint8_t *)&(attend_sp_sec_0),sizeof(attend_sp_sec_0));
			
			WriteDGUS(0x4650,(uint8_t *)&(attend_sp_sec_1),sizeof(attend_sp_sec_1));
			WriteDGUS(0x4660,(uint8_t *)&(attend_sp_min_1),sizeof(attend_sp_min_1));
			WriteDGUS(0x1820,(uint8_t *)&(attend_sp_1),sizeof(attend_sp_1));
			
			WriteDGUS(0x4620,(uint8_t *)&(exit_sp),sizeof(exit_sp));
}
}