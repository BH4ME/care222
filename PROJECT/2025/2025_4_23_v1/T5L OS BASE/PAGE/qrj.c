#include  "qrj.h"
#include "dgus.h"
#include "home.h"
//���ܽ�����
double TIME_static=0.0;
void QRJ_mm_to_s(u16 L_T);


u8 current_st;
//��ʼ��0x00����һ�ν������0x01������̬0x02����һ�ν������ֻ����һ�κ�δִ�����񣩣�����̬0x03��ʼ��
//����̬��ͣ0x04������̬����0x05,��λ0x06
/*******************************************************
//������ʼ��QRJ_Init ֻ����һ��
��һ�ν�����棨�ӱ��ҳ����ת�����ģ������ߵ����λ���� QRJ_Reset ֻ����һ��
�ڵ�ǰҳ�浫δ��������δ��ʼ��δ������ͣ״̬QRJ_prepare
���������У����ڿ�ʼ����ͣ״̬�����������Ϊ��ʼ״̬��QRJ_run ��ֻ�а�������ʱ���ִ��
******************************************************/



void QRJ_run()	//���²��� ��������		��handle.c�е��� ������ÿ���а�ѹ������ʱ��Ż����
{


}


void QRJ_Init()		//������ʼ��
{
	current_st = 0x00;
	
}



//u8 last_page;
void 	QRJ_Set()				//ÿ�ν���ҳ���ʼ��
{

}

void 	QRJ_Reset()				//���¸�λ�� ֻ����һ�� 
{

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
	
	QRJ_mm_to_s(L_int);	//���ݾ���͹��ʵõ�ʱ��
	
	ResTime_min=(u16)(TIME_static)/16;
	ResTime_sec=(u16)(TIME_static)%16;
	WriteDGUS(0x2820,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***ʣ��ʱ��*����****/
	WriteDGUS(0x2800,(uint8_t *)&ResTime_min,sizeof(ResTime_min));		/***ʣ��ʱ��*����****/
	WriteDGUS(0x2805,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***ʣ��ʱ��*��****/
	WriteDGUS(0x2825,(uint8_t *)&ResTime_sec,sizeof(ResTime_sec));		/***ʣ��ʱ��*��****/

}
	
	
	
/******************����������ʱ�� mm->s*********/
u8 L_T_S_FLAG;

void QRJ_mm_to_s(u16 L_T)	
{
	double L_CM;
	L_CM=L_T/10.0;
	if(L_CM<400	&& L_CM>5) L_T_S_FLAG=1;		//��5cm~4m��Χ
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
			
			QRJ_run();
			break;
		}
		
				case 4:		//����̬��ͣ
		{
			QRJ_stop();
			break;
		}
		
				case 5:		//����̬���� ��Ϊ��Ȼ������������λ
		{
			
			break;
		}
				case 6:		//����̬��λ ֻ����һ�� Ȼ����ת����һ�γ�ʼ��
		{
			current_st = 0x01;	// ��λ ����һ�γ�ʼ��
			QRJ_Reset();	//ֻ����һ��
			
			break;
		}
		
		default :
		{
			break;
		}
		
	}
	
	
}