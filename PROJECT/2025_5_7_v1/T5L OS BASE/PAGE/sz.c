#include "sz.h"
#include "timer.h"
#include "adc.h"


void 	SZ_prepare()
{
	if(ADC_100ms >= 2)	//200ms
	{
		T_100MS_ADC_isr();
		ADC_100ms=0;
	}
	
}





void SZ_Home()
{
	
		
		if(last_page!=CurrentPageNum[1])	//��һ�ν���ҳ��
	{
		last_page=CurrentPageNum[1];	
		current_st = 0x01;
	}
	
	
	switch(current_st)		//��handle.c���������м��
	{
		case 0:		//������ʼ��״̬
		{
			current_st = 0x01;	//��ʼ���ɹ�����ת����һ�γ�ʼ��̬

		}
		
		case 1:		//��һ�γ�ʼ��̬
		{
			current_st = 0x02;	//��һ�γ�ʼ���ɹ�����ת������̬
		}
		
		case 2:		//����̬
		{
			
			SZ_prepare();
			break;
		}

		
		default :
		{
			current_st = 0x02;
		//	SZ_prepare();
			break;
		}
	}
	
	
}