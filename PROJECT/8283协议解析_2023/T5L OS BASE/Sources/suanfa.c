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
bit L_ARRAY_FINISH=0;		/****0δ���*****/

double sum = 0;//���
uint array[4] = {0.0,0.0,0.0,0.0};
int length = 0;//���鳤��
double average = 0;//��ƽ����
double var = 0.0; //�󷽲�
double standard = 0; //���׼��


bit shuju_change=0;		/****0δ�������******/

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


//���
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


	if(L_i>=3) {L_i=0;}		/***ȡ0~4����������������鸸����****/
	else {L_i++;L_3FLAG++;}
//	EA=1;

}

///****************array cm**********���ݴ���****�������ó��������ж��Ƿ���Ҫ����ʱ��***********/
void shujuchuli(void)
    {
			int a=0;
			int i=0;
			int j=0;
			double average_jiaoyan=0.0;
			double var_jiaoyan=0.0;
//			EA=0;
			if(L_ARRAY_FINISH==1)				/****�������ݵ�ARRAY��� �������ݷ����Ƿ����ʱ��****/
			{
			//shuju_bianliang_chongzhi();
        for ( i = 0; i <= 2;i++)
        {
            sum += array[i];//���
        }
 
        length = 3;//�����鳤��
        average = sum / length;//��ƽ��ֵ
 
        for ( j = 0; j <= 2;j++)
        {
            var += pow(array[j]-average,2)/length;//�󷽲�
        }
    
        //standard = pow(var,0.5);//���׼��
				//a=var;
				
				if(var<=3)
				{
					shuju_change=0;			/*********���ݲ��仯������Ҫ������Ӧ******/
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
   
//        printf("sum = %g\n",sum);//��ߴ�ӡҲ����ʹ��printf("%f\n",sum);���ִ�ӡĬ�ϳ���������С�����6λ
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


/*********************************************���������������*************************************/
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
//		change_P_mW_First=change_P_mW_doing/1000;		//uw��mw
//	//change_P_mW=((log(change_P_mW))+2)*13;	//ʵʱ���� W119,H98,Y_Cectral 291,VD50,����Ŵ���256�����1����Դͨ��0
//	change_P_mW=log10(change_P_mW_doing);
//	P_FINISH=change_P_mW_First;
//}
/**********************************
change_P_mW_First �Ƕ�̬����ʵmW
change_P_mW	��̬��ʵ��log���uW
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
		change_P_mW_First=change_P_mW_doing/1000;		//uw��mw
	//if(MODE==2)	P_FINISH=change_P_mW_doing/1000.000;		//���uw
	//change_P_mW=((log(change_P_mW))+2)*13;	//ʵʱ���� W119,H98,Y_Cectral 291,VD50,����Ŵ���256�����1����Դͨ��0
	change_P_mW=log10(change_P_mW_doing);
	P_FINISH=change_P_mW_First;//uw��mw
}


/*********************״̬��*****************
˵����1����UI����ʾ����û�����ڽ��е���������ʾ�հ�
	����ʼ����������UVΪ��������ʾ�̵�
	��UV������ʾ���

	2��ָʾ�ƣ���������������ʾ�̵ƣ���UV����ʾ��� ��������

	3�����⣺������ҳ�����ý��桢���ӽ��棬�������濪ʼǰ��������
	����Ļ����򿪣���رռ���
*******************************************/
//bit jiguang_flag=0	//0��ʾδ������Ҫ�رռ��� 1��ʾ��������
u8 start_first_flag=0;	//ֻ�и�λ���Զ���ɵ������ ����λ
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
//		if(ui[1]==0x01)	//����
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

/********************����***********************
˵����t���������ʱ��		on������ 	off�½���   T�ܹ���Ҫʱ��   T1����1 T2

1.t%on!=0		��T=(t/on)*(on+off)+t%on
2.t%on==0 	T=(t/on-1)*(on+off)+on
�� ���������10s ������1s �½���100s ,���� 
***********************************************/
uint maichong_suanfa(uint T,u8 on,u8 off)	//���� ����������ʱ��
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
			//�ж���� 
			
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
	
	if(t1==0)//����һ������
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
	
				if(yichu_T==1)	//6��ȫ����ʧ ����error
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