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

		}
		
		case 1:		//第一次初始化态
		{
			current_st = 0x02;	//第一次初始化成功，跳转到就绪态
		}
		
		case 2:		//就绪态
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