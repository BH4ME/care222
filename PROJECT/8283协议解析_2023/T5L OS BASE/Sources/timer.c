/******************************************************************************
* 文 件 名   : timer.c
* 版 本 号   : V1.0
* 作    者   : pinot
* 生成日期   : 2019年11月18日
* 功能描述   : 定时器应用
* 修改历史   :
* 日    期   :
* 作    者   :
* 修改内容   :
******************************************************************************/

/*****************************************************************************
自定义头文件*/
#include "timer.h"
#include "rtc_sys.h"
#include "mode_3.h"
#include "handle.h"
#include "mode_4.h"
#include "T5LUart4.h"
/*****************************************************************************
全局变量*/
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
定时器0************1ms******/
void T0_Init(void)
{
  TMOD|=0x01;
  TH0=T1MS>>8;        //1ms定时器
  TL0=T1MS;
  ET0=1;              //开启定时器0中断
  TR0=1;              //开启定时器
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




/********************************当开启照射后进入对应函数***********************/
	if(ZIWAIXIAN_FINISH_FLAG ^  ZIWAIXIAN_S_FINISH_FLAG	^(chengxu_start)^(mode_4_flag_start&&start_mode4))
	{
		//Reset_flag=1;	//结束时间同步
		Draw_1ms++;
				WriteDGUS(0x5200, (uint8_t *)&test_error[6],sizeof(test_error[6]));	

		/**************算法******************/
//	if(dynamic_static_flag && ZIWAIXIAN_FINISH_FLAG)
//	{
//			TOF_suanfa_Count++;
//	  if(TOF_suanfa_Count>400)		/********0.2s*******/
//  {
//		
//    TOF_suanfa_Count=0;		

//		if(Off_On_pwm_flag==0)		//下降沿的时候 不进行时间变换
//		{
//		}

//  }
//}
	/*******************程序化模式******************/
				if(chengxu_start)
			{
				D_1s++;
				if(D_1s>=1000){D_1s=0;Draw_1s++;mode_3_isr();}
			}
		/***********手动模式***************/
		else if(mode_4_flag_start&&start_mode4)
		{
			mode4_1s_time++;
			if(mode4_1s_time>=1000)
			{
				mode4_isr();
				mode4_1s_time=0;
			}
		}
	
	
		
		/**********治疗模式****************/
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


/***************气溶胶模式、区域照射、中心照射****************/
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
定时器2*/
void T2_Init(void)
{
  T2CON=0x70;
  TH2=0x00;
  TL2=0x00;
  TRL2H=0xBC;
  TRL2L=0xCD;        //1ms的定时器
  IEN0|=0x20;        //开启定时器2
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
	if (HmiSlvCmd.Step>0 || HmiSlvCmd.Status>2) //ModBus 接收数据超时判断
	{
		HmiSlvCmd.TxTimeOut++; //超时接收发送时间 100ms
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

	/*****************紫外线自检函数防止启动失败*****************/
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
定时器总配置*/
void InitTimer(void)
{
  T0_Init();
  T2_Init();
}

