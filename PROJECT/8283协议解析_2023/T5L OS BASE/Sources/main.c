/******************************************************************************
* �� �� ��   : main.c
* �� �� ��   : V1.0
* ��    ��   : pinot
* ��������   : 2019��11��01��
* ��������   : ������������Ͳ�����ʼ������ѭ������Ҫ���ܺ�����ڡ�
* �޸���ʷ   :
* ��    ��   :
* ��    ��   :
* �޸�����   :2023.2.8 ���ܽ�ģʽ 1�������κμ� ����͹�����ʾ���ݡ�2��ʱ���Զ������󣬷�������ʱ��������ͷ��������߱����ϴβ��䣬3����λ��ʱ��������ͷ�������λ������������Ȼ����
							2021.2.9 ����ҳ�� ���Ӱ���Ч������ʼ����ͣ����ȡ��ʱ���������Ϊ���ڽ���ʱ�䡢ʣ��ʱ�䡢��ʱ�䣬

******************************************************************************/



/*****************************************************************************
ϵͳ��*/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "T5LOS8051.H"
#include "sys.h"
#include "handle.h"
#include <stdio.h>
#include "math.h"
#include "adc.h"
#include "sys_params.h"
#include "uart4.h"
#include	"start_win.h"
#include "column_chart.h"
#include "mode_4.h"
#include "T5LUart4.h"
#include "string.h"

u8 test_error[9]={1,2,3,4,5,6,7,8,9};
uint UART5_FLAG =0;
//״̬��   =0  �ȴ���һ��5A   =1  �ȴ��ڶ���5A =>3 �ڼ�������
uchar DATA15=00;   //У���
uchar GY53_Data[16];//���ܵ�����������
uchar Dat;
uint32_t L[3]={0,0,0};
//uint L_1=11111;
uint L_int;
uint MODE=0;
uint W_P_uJ[5]={0};
uint W_S[5]={0};
bit OPEN_P=0X00;				
bit OPEN_S=0X00;		

uint b=0;
uint c=0;
uint hour=0;
//#define ZIWAIXIAN_Status_Ad 0x3600
//uint Status_Flag=0; /*****0���*****1�Ƶ�*****2�̵�********/

uint norflash_Guanji_flag_5s = 0;
u8 test=1;

//uint nor_UV_flag[3]={0};
//uint Buttom_Write[4]={0};
//uint buttom_flag=0;

uint Status_Current=0;	
uint Status[5]={0,1,2,4,8};	
uint MODE_Static=0;


u8 Page_flag=0;


bit First_Init=1;	//���ֳ�ʼ��
bit Read_buff=1;
u8 Read_First_Distance[2];
u16 L_sp_ad=0x4900; //����ָ��
u16 L_vp_ad=0x2500;
bit L_change=0;

uint l_mode2;
/***************************������********************/
void UART5_isr()
{

	uchar Dat = SBUF3_RX;    						//������ε�����	
	//Uart2SendByte((uint8_t *)&L);
	//DAT_W_EEPROM = EEPROM_Read(0);
	//UART_Send_byte(1,KEY_FLAG);						//�ô���1�Ѵ���2�����ݷ�����
	switch(UART5_FLAG)
	{
		case 0:
		{
			if(Dat == 0x57)     						//�жϵ�һ��5A
			{
				GY53_Data[UART5_FLAG] = Dat;   //������
				UART5_FLAG=1;      					 //״̬��1
			}
			break;
		}
		case 1:
		{
			if(Dat == 0x00)    							//�жϵڶ���5A
			{
				GY53_Data[UART5_FLAG] = Dat;   //������
				UART5_FLAG=2;      						 //״̬��2
			}
			else
			{
				UART5_FLAG = 0;       				//״̬��0
			}
			break;
		}
		default:
		{
					GY53_Data[UART5_FLAG] = Dat;
					UART5_FLAG++;
					if(UART5_FLAG>0x0F) 									//00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 
			{

					UART5_FLAG = 0;       								//״̬��0				
					DATA15 = GY53_Data[0X00]+GY53_Data[0X01]+GY53_Data[0X02]+GY53_Data[0X03]+GY53_Data[0X04]+GY53_Data[0X05]+GY53_Data[0X06]+GY53_Data[0X07]+GY53_Data[0X08]+GY53_Data[0X09]+GY53_Data[0X0A]+GY53_Data[0X0B]+GY53_Data[0X0C]+GY53_Data[0X0D]+GY53_Data[0X0E]; //����У���

				
				if(DATA15 == GY53_Data[0X0F])  					//����ʱ��ɹ�
				{
				//L[0] = ((GY53_Data[0X0A]<<16 | GY53_Data[0X09]<<8) | GY53_Data[0X08]); 
					
					/*************���״̬λ�Ƿ�Ϊ0********************/
					if(GY53_Data[0X0b]==0)
					{

					L_int=((GY53_Data[0X0A]<<16 | GY53_Data[0X09]<<8) | GY53_Data[0X08]);

						if(L_change==1)
						{
							L_change=0;
							if(MODE==1)WriteDGUS(0x4913,(uint8_t *)&(colour_w),sizeof(colour_w));
								else
							WriteDGUS(L_sp_ad+0x3,(uint8_t *)&(colour_w),sizeof(colour_w));
						}
						
					}
					else 	{L_change=1;
					if(MODE==1)WriteDGUS(0x4913,(uint8_t *)&(colour_y),sizeof(colour_y));
					else WriteDGUS(L_sp_ad+0x3,(uint8_t *)&(colour_y),sizeof(colour_y));}	
					//(GY53_Data[0X0A]     GY53_Data[0X09]      GY53_Data[0X08]   //1000=xxx.m

				}
			}
			break;
		}
	}
}













/***************************����ϴ�����*************************/

void shangchuan(void)
{

	uint T_doing_min_int=0;
	uint T_first_min_int=0;
	uint T_doing_sec_int=0;
	uint T_first_sec_int=0;
	uint T_first_h_int=0;
	u16 baifenbi_T=0;
	float I=0.0;

	T_doing_min_int=T_doing/60;
	T_doing_sec_int=T_doing%60;
	
	if(MODE==2&&dynamic_static_flag)
	{
	T_first_min_int=T_dynamic/60;
	T_first_sec_int=T_dynamic%60;
	}
	else
	{
	T_first_min_int=T_All/60;
	T_first_sec_int=T_All%60;
	}

	EA=0;

	//����ʣ��ʱ���ɷ���Сʱ
		c=T_XIANSHI/60;
		b=T_XIANSHI%60;

	
	if(dynamic_static_flag&&MODE==2	)	//����ģʽ�� ��̬ʱ�� ���İٷֱȵ��㷨
	{
		baifenbi_T=(float)(T_doing)/(float)(T_dynamic)*100;
	}	else 	baifenbi_T=(float)(T_doing)/(float)(T_All)*100;

	
	
	if(baifenbi_T>100||baifenbi_T==99)	//�趨�ٷֱ�
	{
		baifenbi_T=100;
	}

	
	if(MODE==2)		//ת����Сʱ ����ģʽ���ֶ�ģʽ֧��Сʱ
	{
		if(c>=60){hour=c/60;c=c%60;WriteDGUS(0x1720,(uint8_t *)&attend_sp_0,sizeof(attend_sp_0));	WriteDGUS(0x1710,(uint8_t *)&(hour),sizeof(hour));	}
		else 
		{WriteDGUS(0x1720,(uint8_t *)&(exit_sp),sizeof(exit_sp));}
			
		if(T_first_min_int>=60)		
		{
			T_first_h_int=T_first_min_int/60;T_first_min_int=T_first_min_int%60;WriteDGUS(0x1820,(uint8_t *)&(attend_sp_1),sizeof(attend_sp_1));	WriteDGUS(0x1810,(uint8_t *)&(T_first_h_int),sizeof(T_first_h_int));
		}		else 
		{WriteDGUS(0x1820,(uint8_t *)&(exit_sp),sizeof(exit_sp));}
	}
	
	
	WriteDGUS(T_XIANSHI_1,(uint8_t *)&c,sizeof(c));		/***ʣ��ʱ��*����****/
	WriteDGUS(0x2805,(uint8_t *)&b,sizeof(b));		/***ʣ��ʱ��*��****/

	
	WriteDGUS(0x2830,(uint8_t *)&(baifenbi_T),sizeof(baifenbi_T));		

	if(MODE==4){	WriteDGUS(0x5000,(uint8_t *)&(mj_mode4),sizeof(mj_mode4));}		/******MODE4������****/

	WriteDGUS(0x2820,(uint8_t *)&(T_first_min_int),sizeof(T_first_min_int));		/***��ʱ��*����****/
	WriteDGUS(0x2825,(uint8_t *)&(T_first_sec_int),sizeof(T_first_sec_int));		/***��ʱ��*��****/
	
	if(MODE==1){
		WriteDGUS(0x2840,(uint8_t *)&(D),sizeof(D));							//ֱ��
		if(W_S[0]==0x01	&&	MODE==1){		WriteDGUS(0x2850,(uint8_t *)&(Volume),sizeof(Volume));}	//���ܽ�Ϊ���
		else {WriteDGUS(0x2850,(uint8_t *)&(Square),sizeof(Square));}		//���	
	}
		if(Page_s[1]==Page_shezhi)		//д��ADC
		{
			I=(adc_10-adc_08)/0.5*1.02;
			WriteDGUS(ADC_08_AD,(uint8_t *)&I,sizeof(I));
			WriteDGUS(ADC_09_AD,(uint8_t *)&adc_09,sizeof(adc_09));
			WriteDGUS(ADC_10_AD,(uint8_t *)&adc_10,sizeof(adc_10));
		}

		//if(W_S[0]==0x03&&	MODE==1){change_P_mW_doing=change_P_mW_doing/2.00;}
		WriteDGUS(0x2600,(uint8_t *)&change_P_mW_doing,sizeof(change_P_mW_doing));		/*********����****************/
		WriteDGUS(0x2700,(uint8_t *)&W_mJ_doing,sizeof(W_mJ_doing));

EA=1;

}					

void Mode_1_run()	//ģʽһ Ϊ���ܽ�ģʽ������ģʽ������ģʽ
{
	EA=0;
		if(W_S[2]==0x01)
		{
			WriteDGUS(0x3010,(uint8_t *)&W_S[3],sizeof(W_S[3]));
			W_S[1]=0X00;
			W_S[2]=0x00;
			W_mJ_XIANSHI=0;
		}
	if(W_S[1]==0x01)
	{
		if(OPEN_S==0)
		{
			MODE_Static=1;
			//UV=1;
		//W_FINISH=W_P_uJ[0];	/*****mJ*****/		
		ZIWAIXIAN_S_FINISH_FLAG=1;
			OPEN_S=1;
		}
	}
	else if(W_S[1]==0x00) {ZIWAIXIAN_S_FINISH_FLAG=0;OPEN_S=0;UV=0;
	
			if(MODE_Static==0)	//δ������ʾʣ��ʱ��
	{
				Static_T_Current_mode1();
	}}
	
	W_S[0]=BUff_main[0];
	
	//EA=1;
}


void Mode_2_run()	//ģʽ��Ϊ����ģʽ
{
	EA=0;
		if(W_P_uJ[2]==0x01)
		{
			WriteDGUS(0x3010,(uint8_t *)&W_P_uJ[3],sizeof(W_P_uJ[3]));
			W_P_uJ[1]=0X00;
			W_P_uJ[2]=0x00;
		}
	if(W_P_uJ[1]==0x01)
	{
		ZIWAIXIAN_FINISH_FLAG=1;		
		MODE_Static=2;
		if(OPEN_P==0)
		{	
//		ReadDGUS(0x2410,(uint8_t *)&PWM_ON,sizeof(PWM_ON));		//����on
//		ReadDGUS(0x2420,(uint8_t *)&PWM_OFF,sizeof(PWM_OFF));
		//UV=1;

			OPEN_P=1;
		}
	}
	else if(W_P_uJ[1]==0x00) {
		ZIWAIXIAN_FINISH_FLAG=0;		
		ReadDGUS(W_S_Address,(uint8_t *)&W_P_uJ[0],sizeof(W_P_uJ[0]));
		W_FINISH=W_P_uJ[0];	/*****mJ*****/	
		ReadDGUS(0x2410,(uint8_t *)&PWM_ON,sizeof(PWM_ON));		//����on
		ReadDGUS(0x2420,(uint8_t *)&PWM_OFF,sizeof(PWM_OFF));
	UV=0;
			if(MODE_Static==0)	//δ������ʾʣ��ʱ��
	{

		Static_T_Current_mode2();
	}
	
	}


//	EA=1;
}




float L_Float = 0.0;
u8 first_page_zhiliao=0;
u8 first_page_shoudong=0;
u8 first_page_lianji=0;
void Switch_page()	//�жϵ�ǰҳ��
{
EA=0;
		switch(Page_s[1])
	{
		case Page_home:	
		{first_page_zhiliao=0;	
			first_page_shoudong=0;
			first_page_lianji=0;
							  ES2T=0;
					ES2R=0;
			//��ҳ
			MODE=0;
			break;
		}
		
		case Page_qirongjiao:
		{
			
			MODE=1;BUff_main[0]=0x01;Mode_1_run();
			break;
		}
		
		
		case Page_zhongxin:
		{
			MODE=1;BUff_main[0]=0x02;Mode_1_run();
			break;
		}
		
				
		case Page_quyu:
		{
			MODE=1;BUff_main[0]=0x03;Mode_1_run();
			break;
		}
		
				
		case Page_chengxuhua:
		{
			first_page_zhiliao=0;	MODE=3;
			if(chengxu_start==1)	//��ʼ������	
			{
				
				//chengxu_Run();
				
			}
			break;
		}
		
				
		case Page_zhiliao:
		{
			//EA=0;
				MODE = 2;
			Mode_2_run();

			if(first_page_zhiliao==0)		//ÿ�ν���ҳ���ʼ�� ÿ�ν�������ģʽֻ����һ��
			{
	
				first_page_zhiliao=1;		
				Lock_current_page_zengliang(0x02,0x02,0x03,0x02);		//��������������
				Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
				Lock_current_page_zengliang(0x06,0x02,0x07,0x02);
				WriteDGUS(0x2405,(uint8_t *)&icon_exit,sizeof(icon_exit));
				WriteDGUS(0x2415,(uint8_t *)&icon_exit,sizeof(icon_exit));
				WriteDGUS(0x2425,(uint8_t *)&icon_exit,sizeof(icon_exit));
			}
			first_page_shoudong=0;
		//	EA=1;
			break;
		}
		
				
		case Page_shoudong:
		{		

			first_page_zhiliao=0;first_page_lianji=0;	
			MODE = 4;
			mode_4_run();
			
			if(first_page_shoudong==0)		//ÿ�ν���ҳ���ʼ�� ÿ�ν�������ģʽֻ����һ��
			{		
					ES2T=0;
					ES2R=0;
				first_page_shoudong=1;		
				Lock_current_page_zengliang(0x02,0x02,0x03,0x02);		//��������������
				Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
				Lock_current_page_zengliang(0x06,0x02,0x07,0x02);
				WriteDGUS(0x1460,(uint8_t *)&icon_exit,sizeof(icon_exit));
				WriteDGUS(0x1465,(uint8_t *)&icon_exit,sizeof(icon_exit));
				WriteDGUS(0x1470,(uint8_t *)&icon_exit,sizeof(icon_exit));
			}

			break;
		}
		
				
		case Page_lianji:
		{
			first_page_shoudong=0;
			
			if(first_page_lianji==0)
			{
				  ES2T=1;
					ES2R=1;
				lianji_run();
				first_page_lianji=1;
			}
				ReadDGUS(0x4600,(uint8_t *)&uart4_open_buff,sizeof(uart4_open_buff));
			//	ReadDGUS(0x1280,(uint8_t *)&(dynamic_static_flag),sizeof(dynamic_static_flag));		//�ж��Ǿ�̬ʱ�� ���Ƕ�̬ʱ��
				T5L_HMIslvSub(); //���ܣ����߸��´���������
			break;
		}
		
				
		case Page_shezhi:
		{
			first_page_lianji=0;	
			ES2T=0;
			ES2R=0;
			break;
		}
		
				case Page_chengxuhua_JUMP:
		{
			first_page_zhiliao=0;	MODE=3;
//			if(chengxu_start==1)	//��ʼ������	
//			{
//				//chengxu_start=0;
//				chengxu_Run();
//				
//			}
			break;
		}
				
				case Page_zhiliao_JUMP:
		{
			MODE = 2;
			Mode_2_run();
		}
		default:
		{
			
			break;
		}
		

		
		
		
	}
	EA=1;
}


u8 UV_FLAG=0;
void 		t2_isr()
{
	/**************�ж��Ƿ�ͨ��ʱ��ļ�����������䣬Ϊ�������ҳ���¸�ֵ��׼��********/
		if(MODE_1_Finish==1	||	MODE_2_Finish==1	||	MODE_4_Finish)		//mode1ͨ��ʱ��ı仯����������	
	{
		if(Page_s[1]!=Page_Lasttime)			//ҳ��仯
		{
		//	W_S[1]=0x01;W_P_uJ[1]=0x01;
			if(MODE_1_Finish==1){
//				Reset_flag=0;	//��ʼʱ��ͬ��
			Draw_Reset();
			//MODE_4_Finish=0;
			W_mJ_XIANSHI=0;
			W_mJ_doing=0;
			T_doing=0;
			MODE_Static=0;			//���� ��ʾδ��������ʱ�Ķ�̬����
			MODE_1_Finish=0;		//����
		//	MODE_2_Finish=0;
				}
				else if(MODE_2_Finish==1&&MODE!=2){			
					//Reset_flag=0;	//��ʼʱ��ͬ��
			Draw_Reset();
	//		MODE_4_Finish=0;
		//	W_mJ_XIANSHI=0;
	//		W_mJ_doing=0;
			T_doing=0;
			MODE_Static=0;			//���� ��ʾδ��������ʱ�Ķ�̬����
	//		MODE_1_Finish=0;		//����
			MODE_2_Finish=0;}
					else if(MODE_4_Finish==1&&MODE!=4){		
//						Reset_flag=0;	//��ʼʱ��ͬ��
			Draw_Reset();
			MODE_4_Finish=0;
		//	W_mJ_XIANSHI=0;
		//	W_mJ_doing=0;
			T_doing=0;
			MODE_Static=0;			//���� ��ʾδ��������ʱ�Ķ�̬����
			//MODE_1_Finish=0;		//����
		//	MODE_2_Finish=0;
					}
//			Reset_flag=0;	//��ʼʱ��ͬ��
//			Draw_Reset();
//			MODE_4_Finish=0;
//			W_mJ_XIANSHI=0;
//			W_mJ_doing=0;
//			T_doing=0;
//			MODE_Static=0;			//���� ��ʾδ��������ʱ�Ķ�̬����
//			MODE_1_Finish=0;		//����
//			MODE_2_Finish=0;
		}
	}
	

		//LED_S();
	/**************************************д��flash��־λ***************************************/		
						if(norflash_Open_UV_Flag==1)	//����
			{
				norflash_Open_UV_Flag=0;
				norflash_Write_Open_UV();

			}
			
			else if(norflash_Close_UV_Flag==1)//��ͣ
			{

				norflash_Close_UV_Flag=0;
				norflash_Write_Close_UV();

			}
					else if(norflash_Stop_UV_Flag==1)//��λ
			{
				norflash_Stop_UV_Flag=0;
					norflash_Write_Stop_UV();
				}
			
				else{
	norflash_Guanji_flag_5s++;	//10sд��һ�ιػ�ʱ��
	if(norflash_Guanji_flag_5s>=20)
	{
		norflash_Guanji_flag_5s=0;
		norflash_Write_guanji();
		
	}
}
	

			if(Screen_set_Time_min!=0)
		{
			Screen_review();
		}
	/**********��ʼ��***********/
			if(test<=24)
		{
			test++;
		}
					if(test==12)
	{
	
			Init_Adress();//flash��ַ��ʼ�� ����ȴ�3s�� ��Ϊ��ʱRTC��ʼ����ɣ�д�뿪����ʱ��Ϊ��Чֵ
		open_sudo_Init();		//�����Զ�������ʼ��
		
	}
	else if(test==20){sudo_click_start();}//ģ���� ��ʼ����
}
/*****************************************************************************
������*/

void main(void)
{

			InitCPU();
			Init_Rtc();	//rtc
			JumpPage(0x14);	//��ת��logo
			WriteDGUS(0x4620,(uint8_t *)&(exit_sp),sizeof(exit_sp));
			WriteDGUS(0x4873,(uint8_t *)&(colour_w),sizeof(colour_w));//����ģʽ ѡ�б�ɫ��ʼ��
		
			Screen_Init();		//��Ļ�����ʼ��
			led_register();//���������ʼ��
		
			T5L_HMIslvInit(); //�������س���Э���ʼ��

			chengxu_Init();//����ģʽ��ʼ�� ��flash��ȡ���ݲ���д��
			//ReadDGUS(0x1280,(uint8_t *)&(dynamic_static_flag),sizeof(dynamic_static_flag));		//�ж��Ǿ�̬ʱ�� ���Ƕ�̬ʱ��
			//open_sudo_Init();
	
	//Ԥ�� �����ܽ�ģʽ���ְ���
			WriteDGUS(0x5105,(uint8_t *)&(icon_attend),sizeof(icon_attend));
	//Ԥѡ ��������
			WriteDGUS(0x5106,(uint8_t *)&(icon_attend),sizeof(icon_attend));
	//Ԥѡ ��������
				WriteDGUS(0x5107,(uint8_t *)&(icon_attend),sizeof(icon_attend));
	EA=1;
  while(1)
  {
		zhexian_Draw();	//���߼��
	
		if(Wait_Count_2==1)//500msѭ��һ��
		{
			
			ReadDGUS(0x14,(uint8_t *)&Page_s,sizeof(Page_s));
			LED_jiguang_Check();
			Wait_Count_2=0;
			
					WriteDGUS(0x5200, (uint8_t *)&test_error[0],sizeof(test_error[0]));		
		
				if(Off_On_pwm_flag==0	||	UV_St==0)		//��PWM��Ӱ�� 1ʱ����Ϣ״̬ 0Ϊ����״̬
			{
				EA=0;
				L_mm_TO_P_mw(L_int);l_mode2=L_int/10;
				EA=1;
			}		
			else if(MODE!=1	||	UV_St==1){ change_P_mW=-1;	change_P_mW_doing=0.0;	}		
			
			Switch_page();
			if(MODE!=3){shangchuan();}		

			if(BUff_main[1]==0){UV=0;}//STOP

			
			L_Float	=	(float)(L_int/1000.00f);	//m����
			WriteDGUS(TOF_Address_H, (uint8_t *)&L_Float,sizeof(L_Float));	
			
								WriteDGUS(0x5200, (uint8_t *)&test_error[1],sizeof(test_error[1]));		
			EA=1;
			/*************************************ҳ��仯���ú���************************************
	˵��������ͨ��ʱ��仯���������������£����з�ҳ�Ĺ��ܻᵼ�µ�ǰ���״̬�µ�����ȫ������
	*******************************************************************************************/

//		if(MODE_Static==0)	//δ������ʾʣ��ʱ��
////	{
//	//	if(Reset_flag==0)		//�����¸�λ����ͬ��ʣ��ʱ�����ʱ��
//		{
//			//EA=0;
//			  Static_T_Current_mode4();
//				Static_T_Current_mode2();
//				Static_T_Current_mode1();
//		//	EA=1;
//		}
//	}
/**************************************************************************************************************/		

}	
  	Clock();
    HandleProc();		



  }
}
