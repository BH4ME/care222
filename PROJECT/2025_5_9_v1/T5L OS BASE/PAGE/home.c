#include  "home.h"
#include "dgus.h"
#include "qrj.h"
#include "sz.h"

#include <math.h>

#include "start_win.h"
uint8_t CurrentPageNum[2]={0};	//CurrentPageNum[1]�ǵ�ǰҳ��

float L_M = 0.0;//���� m
//u16 Time_static_16;	//��̬��ʱ���ɾ���ó���
void Prepare_qrj_zx_qy();	//һֱ���е� �����ĸ�״̬������ǰ����ģʽ

/*********************************************************************************
����ģʽ���������ֱ��	���������ģʽ���ķ�֮һ��ֱ��������ģʽ�Ķ���֮һ
����ģʽ���������ֱ��	
���ܽ�ģʽ�������
********************************************************************************/
float Tanx=0.4557;
float pai=3.1415;
float Tan_zhongxin=0.04366094;
float zhongxin_35mm=0.4008159;	//=0.0175m/tan2.5

float L_m_D_V_S=0.00;				//���� ��λ:M
float Square=0.00;			//��� ��λ��M^2
float D=0.00;					//ֱ�� ��λ��M
float Volume=0.00;					//���ܽ���� ��λ��M^3


//��ʼ������ҳ��
void HOME_INIT()
{
	QRJ_INIT();
	SZ_INIT();
	
}



u8 last_page;
//������������ѭ��ִ�е�ǰҳ��ĳ���
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
			
			Draw_chart();		//������ͼ
			
			
			QRJ_Home();
			Prepare_qrj_zx_qy();
			
			break;
		}
			
		case Page_zhongxin:
		{
			
			ZX_Home();
			Prepare_qrj_zx_qy();			
			
			
			break;
		}
		
		case Page_quyu:
		{
			QY_Home();
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
			SZ_Home();
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
���� ���� Ϊ���ʴ�С
�β�Ϊ����
*******************************************************/
//double change_P_uW=0.0;	//lg change_P_uW_doing
double change_P_uW_doing=0.0;	//���ݾ������Ƴ��Ĺ���
//double change_P_mW_First=0.0;

void L_mm_TO_P_uw(u16 L_mm_TO_P,u8 current_page)		//��ʽ��������
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
		change_P_uW_doing=change_P_uW_doing*0.6;	//���ܽ���������Ҫ��0.6
	}
	//if(W_S[0]!=2&&MODE==1)change_P_mW_doing=change_P_mW_doing*0.6;	//���ܽ���������Ҫ��0.6
//	change_P_mW_First=change_P_mW_doing/1000;		//uw��mw
//	change_P_uW=log10(change_P_uW_doing);	//���ں��ڻ�ͼ�жϵ�ʱ��
//	P_FINISH=change_P_mW_First;//uw��mw

}




u16 baifenbi_T=0;
//���롢���ʡ������ֱ�������
void Prepare_qrj_zx_qy()	//һֱ���е� �����ĸ�״̬������ǰ����ģʽ
{

			L_mm_TO_P_uw(L_int,CurrentPageNum[1]);	//���ݾ���õ����� 
			WriteDGUS(0x2600,(uint8_t *)&change_P_uW_doing,sizeof(change_P_uW_doing));		/*********����****************/
			L_M	=	(float)(L_int/1000.00f);	//m����/*********����****************/
				
	if(CurrentPageNum[1]==Page_qirongjiao)
	{
			L_m_D_V_S	=	 (float)(L_int/1000.000)+zhongxin_35mm;
			D	=	Tanx*2*L_m_D_V_S;
			Square	=	(float)(pow(L_m_D_V_S*Tanx,2)*pai);	
			Volume	=	Square*L_m_D_V_S/3.00;
			WriteDGUS(0x2850,(uint8_t *)&(Volume),sizeof(Volume));	//���ܽ�Ϊ���
		
				//	baifenbi_T = ((mJ_doing*1000.00) / qirongjiao_j ) ;
	}
	else if(CurrentPageNum[1]==Page_zhongxin)
	{
					
			L_m_D_V_S	=	 (float)(L_int/1000.000)+zhongxin_35mm;
			D	=	Tan_zhongxin*2*L_m_D_V_S;
			Square	=	(float)(pow(L_m_D_V_S*Tan_zhongxin,2)*pai);	
			
			WriteDGUS(0x2850,(uint8_t *)&(Square),sizeof(Square));//���	
		
			//				baifenbi_T = ((mJ_doing*1000.00) / qirongjiao_j ) ;
	}
	else if(CurrentPageNum[1]==Page_quyu)
	{
			L_m_D_V_S	=	 (float)(L_int/1000.000)+zhongxin_35mm;
			D	=	Tanx*2*L_m_D_V_S;
			Square	=	(float)(pow(L_m_D_V_S*Tanx,2)*pai);	
			
			WriteDGUS(0x2850,(uint8_t *)&(Square),sizeof(Square));//���	
		
						//	baifenbi_T = ((mJ_doing*1000.00) / qirongjiao_j ) ;
	}
	
			WriteDGUS(0x2500, (uint8_t *)&L_M,sizeof(L_M));	
			WriteDGUS(0x2840,(uint8_t *)&(D),sizeof(D));			//ֱ��

			//WriteDGUS(0x2830,(uint8_t *)&(baifenbi_T),sizeof(baifenbi_T));		//�ٷֱ�
}




float mJ_doing=0.0;	//�������mj
u32 time_100ms=0;	//100ms���жϱ�־λ
u32 time_doing=0;//�Ѿ������ʱ��
//���ܽ������ġ����� run����

void QRJ_ZX__QY_RUN(Callback_u16 callback_u16)						//����QRJ_run
{
	u16 ResTime_sec;
  u16 ResTime_min;	
	static u32 t_1=0;
if(time_100ms)			//100ms���ǲ����ý����ʽ �� ��ʱ�����趨100ms������1ms
{
		time_100ms=0;
		//uJ_doing+=143;
		//mJ_doing=mJ_doing+(change_P_uW_doing/1000);	//ÿ0.1���ۼӵ�ǰ��mj������
		mJ_doing=mJ_doing+(change_P_uW_doing/10000.0f);	
		t_1++;
		if(t_1>=10)	////1s
		{
			t_1=0;
	//		mJ_doing=mJ_doing+(change_P_uW_doing/1000);
		//	time_100ms=0;
			time_doing++; //����ʱ��	
			//TIME_static--;
		}
		callback_u16(L_int);
	 // QRJ_mm_to_s(L_int);	//���ݾ���͹��ʵõ�ʱ��
	
		ResTime_min=(u16)(TIME_static)/60;
		ResTime_sec=(u16)(TIME_static)%60;

		WriteDGUS(0x2800,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***ʣ��ʱ��*����****/
		WriteDGUS(0x2805,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***ʣ��ʱ��*��****/
		
		//��ʱ�� ����ʣ��ʱ������Ѿ�������ʱ��
		ResTime_min=(u16)(TIME_static+time_doing)/60;
		ResTime_sec=(u16)(TIME_static+time_doing)%60;
		WriteDGUS(0x2825,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***��ʱ��*��****/
		WriteDGUS(0x2820,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***��ʱ��*����****/
		

		WriteDGUS(0x2700,(uint8_t *)&mJ_doing,sizeof(mJ_doing));		/***������mj***/
		
		if(mJ_doing*1000.0f > qirongjiao_j){mJ_doing=qirongjiao_j/1000.0f;}	//��ֹ�������
		baifenbi_T = ((mJ_doing*1000.00*100) / qirongjiao_j ) ;
		WriteDGUS(0x2830,(uint8_t *)&(baifenbi_T),sizeof(baifenbi_T));		//�ٷֱ�
		
	}	
}





/********************************************��ȡ��ǰҳ�沢����*************************************/
/****************************************
����ҳ�� 00
���ؼ� 01
�������ƣ�logֵ�� 04
*****************************************/

void Lock_current_page_zengliang(u8 id_1,u8 daima_1);
void Unlock_current_page_zengliang(u8 id_1,u8 daima_1);
u8 st_current[2]={0};
static struct{
	u16 Head ;
	u8 page_0;//�̶�Ϊ0x00
	u8 page_1;
	
	u8 touch_id;
	u8 touch_daima;

	u16 mode;

}PageLock;


void PageLock_Init()	//��ʼ��
{
	PageLock.Head=0x5aa5;
  PageLock.page_0=0x00;
}



	u16 Current_Lock=0;		//0��ʾδ����
	u16 mode_close = 0x0000;
	u16 mode_open = 0x0001;

	u16 ControlPageLock_Adress = 0xb0;

	u8 PageSliding = 0x0b;
	u8 Returnkey = 0x05;
	//u8 zengliang=0x03;


void Lock_current_page()
{
	if(CurrentPageNum[1] == Page_qirongjiao)
	{
		Lock_current_page_zengliang(0x04,0x02);	//��ס�������� logֵ
		//WriteDGUS(0x5105,(uint8_t *)&(icon[0]),sizeof(icon[0]));//����ͼ��
		WriteDGUS(qrj_log_icon,(uint8_t *)&(icon[0]),sizeof(icon[0]));	//����logͼ��
	}
	else if(CurrentPageNum[1] == Page_qirongjiao)
	{
		Lock_current_page_zengliang(0x04,0x02);	//��ס�������� logֵ
		WriteDGUS(zx_log_icon,(uint8_t *)&(icon[0]),sizeof(icon[0]));	//����logͼ��
	}
	else if(CurrentPageNum[1] == Page_qirongjiao)
	{
		Lock_current_page_zengliang(0x04,0x02);	//��ס�������� logֵ
		WriteDGUS(qy_log_icon,(uint8_t *)&(icon[0]),sizeof(icon[0]));	//����logͼ��
	}
//	if(MODE==1)				
//	{
//				if(W_S[0]==1)//���ܽ�
//		{

//		Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
//		}
//		else if(W_S[0]==2)//zx
//		{
//		WriteDGUS(0x5106,(uint8_t *)&(icon_exit),sizeof(icon_exit));
//		Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
//		}
//				else if(W_S[0]==3)//qy
//		{
//					WriteDGUS(0x5107,(uint8_t *)&(icon_exit),sizeof(icon_exit));
//		Lock_current_page_zengliang(0x04,0x02,0x05,0x02);

//		}
//	}
	
	//ReadDGUS(ReadCurrentPage_Adress,(uint8_t *)&Current_page,sizeof(Current_page));	//����ֵ 00 06
	PageLock.page_1=CurrentPageNum[1];
	PageLock.touch_id=0x00;			//������ҳ����Ϊ 0λ��
	PageLock.touch_daima=PageSliding;
	PageLock.mode=mode_close;

	WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
	while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&st_current,sizeof(st_current));
		if(st_current[0]==st_current[1])
		{break;}
	}
	
	PageLock.touch_id=0x01;		//���ؼ� ����Ϊ1λ��
	PageLock.touch_daima=Returnkey;
	WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
	while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&st_current,sizeof(st_current));
		if(st_current[0]==st_current[1])
		{break;}
	}
	Current_Lock=1;

}

void Unlock_current_page()
{

	if(Current_Lock==1)	//���ж��Ƿ�����
	{
			if(CurrentPageNum[1] == Page_qirongjiao	|| CurrentPageNum[1] == Page_zhongxin	||CurrentPageNum[1] == Page_quyu)
		{
		//WriteDGUS(0x5105,(uint8_t *)&(icon_attend),sizeof(icon_attend));
			Unlock_current_page_zengliang(0x04,0x02);
		}
//			if(MODE==1)				
//	{
//		if(W_S[0]==1)//���ܽ�
//		{
//		WriteDGUS(0x5105,(uint8_t *)&(icon_attend),sizeof(icon_attend));
//		Unlock_current_page_zengliang(0x04,0x02,0x05,0x02);
//		}
//		else if(W_S[0]==2)//zx
//		{
//					WriteDGUS(0x5106,(uint8_t *)&(icon_attend),sizeof(icon_attend));
//				Unlock_current_page_zengliang(0x04,0x02,0x05,0x02);

//		}
//				else if(W_S[0]==3)//qy
//		{
//		WriteDGUS(0x5107,(uint8_t *)&(icon_attend),sizeof(icon_attend));
//		Unlock_current_page_zengliang(0x04,0x02,0x05,0x02);
//		}
//	}
	

			PageLock.page_1=CurrentPageNum[1];	//�������ؼ�
			PageLock.mode=mode_open;
			PageLock.touch_daima=Returnkey;	
			PageLock.touch_id=0x01;	
			WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
		while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&st_current,sizeof(st_current));
		if(st_current[0]==st_current[1])
		{break;}
	}
			PageLock.touch_daima=PageSliding;		//��������ģ��
			PageLock.touch_id=0x00;		
			WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
	while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&st_current,sizeof(st_current));
		if(st_current[0]==st_current[1])
		{break;}
	}
			Current_Lock=0;

	}
	

}

//��ס���� 
void Lock_current_page_zengliang(u8 id_1,u8 daima_1)
{

	PageLock.page_1=CurrentPageNum[1];
	PageLock.touch_id=id_1;			//����log mj�Ĵ���������ס
	PageLock.touch_daima=daima_1;
	PageLock.mode=mode_close;

	WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
	while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&st_current,sizeof(st_current));
		if(st_current[0]==st_current[1])
		{break;}
	}
}



//��������
void Unlock_current_page_zengliang(u8 id_1,u8 daima_1)
{
	
	PageLock.page_1=CurrentPageNum[1];
	PageLock.touch_id=id_1;		//���ؼ� ����Ϊ1λ��
	PageLock.touch_daima=daima_1;
	PageLock.mode=mode_open;
	WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
		while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&st_current,sizeof(st_current));
		if(st_current[0]==st_current[1])
		{break;}
	}
}

