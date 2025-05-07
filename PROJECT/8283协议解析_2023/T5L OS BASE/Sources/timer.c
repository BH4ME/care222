/******************************************************************************
* �� �� ��   : timer.c
* �� �� ��   : V1.0
* ��    ��   : pinot
* ��������   : 2019��11��18��
* ��������   : ��ʱ��Ӧ��
* �޸���ʷ   :
* ��    ��   :
* ��    ��   :
* �޸�����   :
******************************************************************************/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "timer.h"
#include "rtc_sys.h"
#include "mode_3.h"
#include "handle.h"
#include "mode_4.h"
#include "T5LUart4.h"
/*****************************************************************************
ȫ�ֱ���*/
uint16_t Wait_Count_2=0;
uint16_t Wait_Count=0;
uint16_t TOF_suanfa_Count=0;
uint16_t T_1S=0;
uint16_t T_1S_S=0;
uint T_100MS_ADC=0;
uint norflash_Close_UV_Flag=0;
uint norflash_Open_UV_Flag=0;
uint norflash_Stop_UV_Flag=2;
//uint Work_Time=0;
uint Draw_1s=0;
uint Draw_1ms=0;
uint D_1s=0;
uint Screen_Time_500ms=0;
uint mode_4_500ms;
uint mode4_1s_time;
uint uv_f;
uint t2;
//uint jiguang_time_1s=0;
//uint t1_300;
uint t15ms=0;
/*****************************************************************************
��ʱ��0************1ms******/
void T0_Init(void)
{
  TMOD|=0x01;
  TH0=T1MS>>8;        //1ms��ʱ��
  TL0=T1MS;
  ET0=1;              //������ʱ��0�ж�
  TR0=1;              //������ʱ��
}
void T0_ISR_PC(void)    interrupt 1
{
  EA=0;
  TH0=T1MS>>8;
  TL0=T1MS;
  Wait_Count++;
	SysTick_RTC++;
//	t1_300++;
//	if(t1_300>=300)
//	{
//		t1_300=0;
//		t1_isr();
//	}

//	
	// HandleProc();	
	
  if(Wait_Count>500)		/********0.5s*******/
  {
    Wait_Count=0;
		Wait_Count_2=1;
		if(Screen_Touch_flag==0)
		{
		Screen_Time_500ms++;
		}
//		jiguang_time_1s++;
  }




/********************************���������������Ӧ����***********************/
	if(ZIWAIXIAN_FINISH_FLAG ^  ZIWAIXIAN_S_FINISH_FLAG	^(chengxu_start)^(mode_4_flag_start&&start_mode4))
	{
		//Reset_flag=1;	//����ʱ��ͬ��
		Draw_1ms++;
				WriteDGUS(0x5200, (uint8_t *)&test_error[6],sizeof(test_error[6]));	

		/**************�㷨******************/
//	if(dynamic_static_flag && ZIWAIXIAN_FINISH_FLAG)
//	{
//			TOF_suanfa_Count++;
//	  if(TOF_suanfa_Count>400)		/********0.2s*******/
//  {
//		
//    TOF_suanfa_Count=0;		

//		if(Off_On_pwm_flag==0)		//�½��ص�ʱ�� ������ʱ��任
//		{
//		}

//  }
//}
	/*******************����ģʽ******************/
				if(chengxu_start)
			{
				D_1s++;
				if(D_1s>=1000){D_1s=0;Draw_1s++;mode_3_isr();}
			}
		/***********�ֶ�ģʽ***************/
		else if(mode_4_flag_start&&start_mode4)
		{
			mode4_1s_time++;
			if(mode4_1s_time>=1000)
			{
				mode4_isr();
				mode4_1s_time=0;
			}
		}
	
	
		
		/**********����ģʽ****************/
	 else if(ZIWAIXIAN_FINISH_FLAG==1)
		{
			//a=T_1S;
			T_1S++;
			First_shuju();
				if(T_1S>1000)
				{
					//Work_Time++;
					T_1S=0;
					TIM0_1S_isr();
				}
			
		}


/***************���ܽ�ģʽ���������䡢��������****************/
		else if(ZIWAIXIAN_S_FINISH_FLAG==1)
		{
			T_1S_S++;
			First_S_shuju();
			if(T_1S_S>1000)
				{
					T_1S_S=0;
					//Work_Time++;
					TIM0_1S_S_isr();
				}
			
		}else {T_1S_S=0;}

		
				WriteDGUS(0x5200, (uint8_t *)&test_error[7],sizeof(test_error[7]));	
}
else{TOF_suanfa_Count=0;}

  EA=1;
}
/*****************************************************************************
��ʱ��2*/
void T2_Init(void)
{
  T2CON=0x70;
  TH2=0x00;
  TL2=0x00;
  TRL2H=0xBC;
  TRL2L=0xCD;        //1ms�Ķ�ʱ��
  IEN0|=0x20;        //������ʱ��2
  TR2=0x01;
}
void T2_ISR_PC(void)    interrupt 5
{
  TF2=0;    
	EA=0;
	uv_f++;
	t2++;
	
	if(Page_s[1]==Page_lianji)
	{
		t15ms++;
	if(t15ms>=5)
	{
		t15ms=0;

				if (HmiSlvCmd.Status==1){
		HmiSlvCmd.Delay++; 
		if (HmiSlvCmd.Delay>0) HmiSlvCmd.Status=2; 
	}
	if (HmiSlvCmd.Step>0 || HmiSlvCmd.Status>2) //ModBus �������ݳ�ʱ�ж�
	{
		HmiSlvCmd.TxTimeOut++; //��ʱ���շ���ʱ�� 100ms
		if (HmiSlvCmd.TxTimeOut>20)
		{
			HmiSlvCmd.TxTimeOut=0;
			HmiSlvCmd.Step=0;
			HmiSlvCmd.Status=0;
		}
	}
	else HmiSlvCmd.TxTimeOut=0;
}
	}
	else 	if(Page_s[1]==Page_shezhi)
	{
					T_100MS_ADC++;
				if(T_100MS_ADC>300)
				{
					T_100MS_ADC=0;
					T_100MS_ADC_isr();
				}
	}
	

	if(t2>=500)
	{
		t2_isr();

		t2=0;
	}
	/************adc**************/
//			if(Page_s[1]==Page_shezhi)
//			{			
//				T_100MS_ADC++;
//				if(T_100MS_ADC>300)
//				{
//					T_100MS_ADC=0;
//					T_100MS_ADC_isr();
//				}
//			}	

	/*****************�������Լ캯����ֹ����ʧ��*****************/
	if(uv_f>=100)
	{
				uv_f=0;
		if(UV==1)
			{
							if(UV^UV_St==0)
				{
					UV=0;UV_FLAG=1;
				}
				else if(UV_FLAG==1){UV_FLAG=0;UV=1;}
				
				}	
			}

//	
EA=1;
}
/*****************************************************************************
��ʱ��������*/
void InitTimer(void)
{
  T0_Init();
  T2_Init();
}

