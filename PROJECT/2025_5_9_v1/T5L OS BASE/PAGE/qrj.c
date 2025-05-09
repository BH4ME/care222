#include  "qrj.h"
#include "dgus.h"
#include "home.h"
//���ܽ�����
double TIME_static=0.0;
void QRJ_mm_to_s(u16 L_T);

u16 icon[4]={0x00,0x01,0x02,0x03};

u8 current_st;
//��ʼ��0x00����һ�ν������0x01������̬0x02����һ�ν������ֻ����һ�κ�δִ�����񣩣�����̬0x03��ʼ��
//����̬��ͣ0x04������̬����0x05,��λ0x06
/*******************************************************
//������ʼ��QRJ_Init ֻ����һ��
��һ�ν�����棨�ӱ��ҳ����ת�����ģ������ߵ����λ���� QRJ_Reset ֻ����һ��
�ڵ�ǰҳ�浫δ��������δ��ʼ��δ������ͣ״̬QRJ_prepare
���������У����ڿ�ʼ����ͣ״̬�����������Ϊ��ʼ״̬��QRJ_run ��ֻ�а�������ʱ���ִ��
******************************************************/

void QRJ_done()	//�����������
{
	WriteDGUS(0x3010,(uint8_t *)&icon[0],sizeof(icon[0]));	//ͼ�� �俪ʼ
}


void QRJ_run()	//���²��� ��������		
{
	//QRJ_ZX__QY_RUN();
  QRJ_ZX__QY_RUN(QRJ_mm_to_s);	

	//���� �������
	if(mJ_doing*1000 >= qirongjiao_j  ||  TIME_static<=0  ||  baifenbi_T >=100)
	{
		
	
		
		current_st = 0x05;	//״̬λ����Ϊ ��Ȼ���	
	}
	
}



void QRJ_INIT()		//������ʼ��
{
	current_st = 0x00;
	WriteDGUS(0x3010,(uint8_t *)&icon[0],sizeof(icon[0]));
}



//u8 last_page;
//��Ҫ���ܽ������ص�Ԫ
void 	QRJ_Set()				//ÿ�ν���ҳ���ʼ��
{

}


void 	QRJ_Reset()				//���¸�λ�� ֻ����һ�� 
{
	
	mJ_doing = 0;	//���������� 
	WriteDGUS(0x2700,(uint8_t *)&mJ_doing,sizeof(mJ_doing));		/***������mj***/
	
	baifenbi_T = 0;	//��ɰٷֱ�Ϊ0
	WriteDGUS(0x2830,(uint8_t *)&(baifenbi_T),sizeof(baifenbi_T));		
	
	time_doing=0;	//�ѽ��� ��ʱ����Ϊ0
	
	WriteDGUS(0x3010,(uint8_t *)&icon[0],sizeof(icon[0]));	//ͼ�� �俪ʼ
	 
	
}
	
//		L_int   mm

float qirongjiao_j=0.0;

void QRJ_prepare()	//�ڵ�ǰҳ�� ��δ����
{
	u16 ResTime_sec;
  u16 ResTime_min;	
	u16 QRJ_mj;	


	ReadDGUS(0x5100,(uint8_t *)&QRJ_mj,sizeof(QRJ_mj));  	//��ȡɱ��logֵ ��Χ1~5
	switch(QRJ_mj)
	{
		case 1:
		{
			qirongjiao_j	=	500;			//��λ��uj ������mj
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
	
	QRJ_mm_to_s(L_int);	//���ݾ���͹��ʵõ�ʱ��
	
	ResTime_min=(u16)(TIME_static)/60;
	ResTime_sec=(u16)(TIME_static)%60;
	WriteDGUS(0x2820,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***��ʱ��*����****/
	WriteDGUS(0x2800,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***ʣ��ʱ��*����****/
	WriteDGUS(0x2805,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***ʣ��ʱ��*��****/
	WriteDGUS(0x2825,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***��ʱ��*��****/

}
	
	
	
/******************����������ʱ�� mm->s*********/


void QRJ_mm_to_s(u16 L_T)	
{
	u8 L_T_S_FLAG;
	double L_CM;
	L_CM=L_T/10.0;
	if(L_CM<400	&& L_CM>5) L_T_S_FLAG=1;		//��5cm~4m��Χ
				else L_T_S_FLAG=5;

	if(current_st!=0x03)	//����������̬
	{
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
	
	else	//����̬
		
	{
		
		switch(L_T_S_FLAG)
	{
		case 1:
		{
			//TIME_static=((float)(qirongjiao_j *100- uJ_doing))	/ (14.3f*100) ;
			TIME_static=((float)(qirongjiao_j - mJ_doing*1000))	/ (change_P_uW_doing) ;
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





void QRJ_stop()
{
	
}





void QRJ_Home()	//ѭ������
{
	
		if(last_page!=CurrentPageNum[1])	//��һ�ν���ҳ��
	{
		last_page=CurrentPageNum[1];	
		current_st = 0x01;
	}
//	else 
//	{
//		
//	}	
	
	switch(current_st)		//��handle.c���������м��
	{
		case 0:		//������ʼ��״̬
		{
			current_st = 0x01;	//��ʼ���ɹ�����ת����һ�γ�ʼ��̬
		//	break;
		}
		
		case 1:		//��һ�γ�ʼ��̬
		{
			current_st = 0x02;	//��һ�γ�ʼ���ɹ�����ת������̬
			QRJ_Set();
			
		//	break;
		}
		
		case 2:		//����̬
		{
			QRJ_prepare();
			break;
		}
		
				case 3:		//����̬��ʼ
		{
			UV=1;
			QRJ_run();
			break;
		}
		
				case 4:		//����̬��ͣ
		{
			UV=0;
			QRJ_stop();
			break;
		}
		
				case 5:		//����̬���� ��Ϊ��Ȼ������������λ
		{
			UV=0;
			QRJ_done();	
			break;
		}
				case 6:		//����̬��λ ֻ����һ�� Ȼ����ת����һ�γ�ʼ��
		{
			UV=0;
			QRJ_Reset();	//ֻ����һ��
			Unlock_current_page();//����
			
	//		WriteDGUS(0x5105,(uint8_t *)&(icon_exit),sizeof(icon_exit));
			current_st = 0x01;	// ��λ ����һ�γ�ʼ��		
			break;
		}
		
		default :
		{
			break;
		}
		
	}
	
	
}