/******************************************************************************
* �� �� ��   : handle.c
* �� �� ��   : V1.0
* ��    ��   : pinot
* ��������   : 2019��11��27��
* ��������   : �߼�����
* �޸���ʷ   :
* ��    ��   :
* ��    ��   :
* �޸�����   :
******************************************************************************/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "handle.h"
#include "mode_4.h"

/*****************************************************************************
ȫ�ֱ���*/
////�ڲ�RAM
uint8_t value0F00[4] = {0};
//uint8_t xp_status=0;
u16 BUff_main[2]=0;
//�ⲿRAM
//typedef struct dlt_645_8283
//{
//    uint8_t st_byte1;//֡��ʼ��
//    uint8_t st_byte2;//֡��ʼ��
//    uint8_t lenth;//����
//    uint8_t func;//����
//    uint8_t addr1;//��ַ
//    uint8_t addr2;//��ַ	
//    uint8_t edata[250];//��������
//}dl_t_645_8283_body;
//dl_t_645_8283_body xdata s_dl_t_645_8283_body={0};


/*****************************************************************************
�жϰ���ֵ�Ƿ����ϴ�*/
int8_t GetValue0F00(void)
{
	uint8_t cleanData[4]={0};
	ReadDGUS(DHW_0F00,value0F00,sizeof(value0F00));
	if(0x5A == value0F00[0])
	{
		WriteDGUS(DHW_0F00,cleanData,sizeof(cleanData));
		return 1;
	}
	return 0;
}
/*****************************************************************************
���ڳ��δ�ӡ*/

/*****************************************************************************
��תָ��ҳ��*/
void JumpPage(uint16_t pageId)
{
	uint8_t temp[4]={0x5A,0x01,0,0};
	temp[2]=(uint8_t)(pageId>>8);
	temp[3]=pageId;
	EA=0;
  WriteDGUS(DHW_SPAGE,temp,sizeof(temp));
	do
	{
    DelayMs(5);
	  ReadDGUS(DHW_SPAGE,temp,1);
	}while(temp[0]!=0);
	//EA=1;
}





uint control_zhiliao_flag=0;		//�ж�����ģʽ mj on off ����״̬
uint control_shoudong_flag=0;		//�ж��ֶ�ģʽ time on off ����״̬
u8 icon_attend[2]={0x00,0x03};
u8 icon_exit[2]={0x00,0x00};
u8 control_chengxu_flag=0;
	u8 Ad_chengxu,yushu_chengxu,chushu_chengxu;
u8 chengxu_1_24_lock(u8 buff)	//�����������ڷ�Χ��0~99������ʱ�䷶ΧӦ����1��~24
{
	u8 buffer;
	u8 reset[2]={0x00,0x00};
	buffer=buff;
	if(	control_chengxu_flag==1)		//Сʱ1~24 
	{
			if(buff>=0x19	&&	buff<=0x62)		//���ڵ���25
			{
				buffer=0x00;
				WriteDGUS(0x2550,(uint8_t *)&reset,sizeof(reset));
				return buffer;
			}
			else if(buff==0x63)
			{
				buffer=0x18;
				reset[1]=buffer;
				WriteDGUS(0x2550,(uint8_t *)&reset,sizeof(reset));
				return buffer;
			}
		return buffer;		
	}
	else if(control_chengxu_flag==2)		//����0~60
	{
			if(buff>=0x3d	&&	buff<=0x62)		//���ڵ���61
			{
				buffer=0x00;
				WriteDGUS(0x2550,(uint8_t *)&reset,sizeof(reset));
				return buffer;
			}
			else if(buff==0x63)
			{
				buffer=0x3c;
				reset[1]=buffer;
				WriteDGUS(0x2550,(uint8_t *)&reset,sizeof(reset));
				return buffer;
			}
		return buffer;	
	}
	else if(yushu_chengxu==4&&control_chengxu_flag==3)	//����on ��Χ1~99
	{

			if(buff==0x0)
			{
				buffer=0x1;
				reset[1]=buffer;
				WriteDGUS(0x2550,(uint8_t *)&reset,sizeof(reset));
				return buffer;
			}
		return buffer;			
	}
	
	return buffer;

}

u8 chengxu_start =0;
u8 chengxu_stop =0;
u8 chengxu_start_1 =0;
u8 mode_4_flag_start=0;	//0δ���� 1����


/*****************************************************************************
������ѹ֮��������*/
u8 uart4_open_buff[2];
u16 colour_y=0xf800;
u16 colour_w=0xffff;

void TouchHandle(void)
{
	u8 buff_chengxuhua[2];
	//u8 Current_page[2];
	static u16 before_ad;
	static u16 current_ad;
//	uint16_t LED_Buff[4]={0x0000,0,0x00,0x00};
	//uint baifenbi[5]={0};
	//uint8_t temp_nor[2]={0,0x8};
//	uint8_t temp[16]={0};
		uint8_t data_read[4]={0};
//	uint16_t flag=0;
//	uint16_t iconid=0;
	uint16_t getDar=0;
		EA=0;
  getDar = value0F00[1]<<8|value0F00[2];
//	LOG((uint8_t *)&getDar,sizeof(getDar));
//	
//	s_dl_t_645_8283_body.st_byte1=0x5A;
//	s_dl_t_645_8283_body.st_byte2=0xA5;
//	s_dl_t_645_8283_body.lenth=value0F00[3]*2+3;	
//	s_dl_t_645_8283_body.func=0x83;	
//	s_dl_t_645_8283_body.addr1=value0F00[1];
//  s_dl_t_645_8283_body.addr2=value0F00[2];

//	ReadDGUS(getDar,(uint8_t *)&s_dl_t_645_8283_body.edata[0],value0F00[3]*2);
//	
	
		
	 if (getDar==0x3010)		//������ͣ��ʼ
	{
		
		ReadDGUS(0x3010,(uint8_t *)&BUff_main[1],sizeof(BUff_main[1]));
		if(BUff_main[1]==0x01)		//��ʼ
		{
			norflash_Open_UV_Flag=1;
			if(MODE==1){W_S[1]=0x01;UV=1;}
			else if(MODE==2){W_P_uJ[1]=0x01;reset_mode2=1;			JumpPage(Page_zhiliao_JUMP);}
			else if(MODE==3){
				chengxu_start=1; 
				JumpPage(Page_chengxuhua_JUMP);
			if(chengxu_run_flag==0)chengxu_start_1=1;		}								//����ģʽ
			else if(MODE==4){reset_mode4=1;start_mode4=1;mode_4_Init();}
				//	ResMODEt_Draw_juxing();Draw_Reset();
		}
		else if(BUff_main[1]==0x00)	//��ͣ
		{
			UV=0;UE=0;norflash_Stop_UV_Flag=1;
					if(MODE==1){W_S[1]=0x00;}
					else if(MODE==2){W_P_uJ[1]=0x00;}
					else if(MODE == 3){
					chengxu_stop=1;chengxu_start=0;}
					else if(MODE==4)start_mode4=0;
				//	UV=0;
		}			
	}
	
	else if(getDar==0x3020)
	{

		if(MODE==1){S_bianliangqingling();MODE_1_Finish=0;		W_mJ_doing=0;
		W_mJ_XIANSHI=0;}
			else if(MODE==2){if(reset_mode2)CLOSE_W_bianliangchongzhi();MODE_2_Finish=0;}

			else if(MODE==3)
			{
				JumpPage(Page_chengxuhua);
				chengxu_start=0;
				WriteDGUS(0x3010,(uint8_t *)&icon_exit,sizeof(icon_exit));
				chengxu_start_1=0;
				chengxu_run_flag=0;
			}
			else if(MODE==4)
			{if(reset_mode4)mode_4_reset();					JumpPage(Page_shoudong);	MODE_4_Finish=0;}

	//	Off_On_pwm_flag=0;
		UV=0;
			//UE=0;
		//Reset_flag=0;	//��ʼʱ��ͬ��
		Draw_Reset();
//		norflash_Close_UV_Flag=1;

	//	T_doing=0;
		MODE_Static=0;
//		start_first_flag=0;

	}
	else
	{
		
		/**************************ģʽ1*****************************************/
//	if(Page_s[1]==Page_zhongxin||Page_qirongjiao||Page_quyu)
//	{
//		if(getDar==0x2403)
//		{
//			Unlock_current_page_zengliang(0x04,0x02,0x05,0x02);
//			WriteDGUS(0x5105,(uint8_t *)&(icon_attend),sizeof(icon_attend));		
//		}
//		else
//			{		
//				WriteDGUS(0x5105,(uint8_t *)&(icon_exit),sizeof(icon_exit));
//				Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
//			}
//	
//	}
	/******************************************************************����ģʽ�� ��������*****************************************/
	
	//ReadDGUS(0x14,(uint8_t *)&Page_s,sizeof(Page_s));
	 if(Page_s[1]==Page_zhiliao)
	{
		//if(ZIWAIXIAN_FINISH_FLAG)
					if(getDar==0x2403)		//����ģʽ mj��������
			{
			//ReadDGUS(S_LED_0_Address,(uint8_t *)&LED_Buff[0],sizeof(LED_Buff[0]));
				
									WriteDGUS(0x2425,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x06,0x02,0x07,0x02);
									WriteDGUS(0x2415,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
				
					WriteDGUS(0x2440,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
					WriteDGUS(0x2450,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
					WriteDGUS(0x2460,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
					control_zhiliao_flag=1;
					Unlock_current_page_zengliang(0x02,0x02,0x03,0x02);
					WriteDGUS(0x2405,(uint8_t *)&(icon_attend),sizeof(icon_attend));	
			}
			else if(getDar==0x2413)		//����ģʽ�� ����on����
				
			{
									WriteDGUS(0x2440,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
					WriteDGUS(0x2450,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
					WriteDGUS(0x2460,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
									WriteDGUS(0x2425,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x06,0x02,0x07,0x02);
									WriteDGUS(0x2405,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x02,0x02,0x03,0x02);
				
					control_zhiliao_flag=2;
					Unlock_current_page_zengliang(0x04,0x02,0x05,0x02);
					WriteDGUS(0x2415,(uint8_t *)&(icon_attend),sizeof(icon_attend));	
			}
			else if(getDar==0x2423)		//����ģʽ�� ����off����
			{					WriteDGUS(0x2440,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
					WriteDGUS(0x2450,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
					WriteDGUS(0x2460,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
									WriteDGUS(0x2405,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x02,0x02,0x03,0x02);
									WriteDGUS(0x2415,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
					control_zhiliao_flag=3;
					Unlock_current_page_zengliang(0x06,0x02,0x07,0x02);
					WriteDGUS(0x2425,(uint8_t *)&(icon_attend),sizeof(icon_attend));	
			}
			else if(getDar==0x3010)		//������ģʽ�°��¿�ʼ����
			{
			//	WriteDGUS(0x2440,(uint8_t *)&(icon_attend),sizeof(icon_attend));	
				zhiliao_read_norflash();
			}
				
			else if(Page_s[1]==Page_zhiliao	)	//����ģʽ�� ��ⰴ����	��ס�ϴε��������� �����ϴε�����ͼ��
			{
				switch(control_zhiliao_flag)
				{
					case 0:	{break;}
					
					case 1:	
						{	
							control_zhiliao_flag=0;	
							WriteDGUS(0x2405,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x02,0x02,0x03,0x02);
						}
						
									case 2:	
						{	
							control_zhiliao_flag=0;	
							WriteDGUS(0x2415,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
						}
						
									case 3:	
						{	
							control_zhiliao_flag=0;	
							WriteDGUS(0x2425,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x06,0x02,0x07,0x02);
						}
									default :{break;}
						
				}
				
			 if(getDar==0x2440)	//Ƥ�������� ͷ֡0xf0
			{
				 zhiliao_Choose_Mode_1();
			}
			else if(getDar==0x2450)	//����������	ͷ֡0xf1
			{
				 zhiliao_Choose_Mode_2();				
			}
			else if(getDar==0x2460)	//����������	ͷ֡0xf2
			{
				 zhiliao_Choose_Mode_3();				
			}				
			
		}

	}
	/*******************************************************************************************************/	
		
	/*************************************����ģʽ***************************************/
	else 	if(Page_s[1]==Page_chengxuhua)
	{

		/**********��ַ�㷨
		��0x2560��0x257d֮�䣨���漰���ڣ��ܹ���5�����ݱ��0~4��ÿһ����6�����ݱ����0~6��Сʱ1������1��Сʱ2������2��on��off��
		1��ȡ��ַ-0x2560=x
		2��x/6=�ڼ������ݣ�0~4��
		3��x%5=�ڼ������ݣ�0~5��	u8 Ad_chengxu,yushu_chengxu,chushu_chengxu;
		*************************/
		if(getDar>=0x2510	&&	getDar<=0x252d)
		{
			//�ı���ɫ ѡ��
			current_ad=((getDar-0x2510)*0x10)+0x4700;
			WriteDGUS(before_ad+0x3,(uint8_t *)&(colour_w),sizeof(colour_w));
			WriteDGUS(current_ad+0x3,(uint8_t *)&(colour_y),sizeof(colour_y));	
			before_ad=current_ad;	
			
					//Unlock_current_page_zengliang(0x02,0x02,0x03,0x02);
					WriteDGUS(0x2555,(uint8_t *)&(icon_attend),sizeof(icon_attend));	
			Ad_chengxu=getDar-0x2510;	//��Χ��[0~1d]
			yushu_chengxu=Ad_chengxu%6;	//��Χ[0~5]
			
			if(yushu_chengxu==0||yushu_chengxu==2)	//Сʱ
			{
				control_chengxu_flag=1;
			}
			else if(yushu_chengxu==1||yushu_chengxu==3)	//����
			{
				control_chengxu_flag=2;
			}
//			else if(yushu_chengxu==4||yushu_chengxu==5)	//����
//			{
//				control_chengxu_flag=3;
//			}else control_chengxu_flag=0;
			else if(yushu_chengxu==4||yushu_chengxu==5)	//����
			{
				control_chengxu_flag=3;
			}else control_chengxu_flag=0;
			
			
			
			ReadDGUS(0x2560+Ad_chengxu,(uint8_t *)&buff_chengxuhua,sizeof(buff_chengxuhua));		//��ȡ��ǰ���� 
			WriteDGUS(0x2550,(uint8_t *)&(buff_chengxuhua),sizeof(buff_chengxuhua));//���ոն�ȡ������д�룬�����û��ڴ˻����ϼ����޸���ֵ

			
		}
		else if(getDar>=0x257E	&&	getDar<=0X2582)		//�������� ͬһ��ַ
		{
							WriteDGUS(0x2555,(uint8_t *)&icon_exit,sizeof(icon_exit));
				//Lock_current_page_zengliang(0x02,0x02,0x03,0x02);
		}
		else if(getDar==0x2550)		//��������
		{
					ReadDGUS(0x2550,(uint8_t *)&buff_chengxuhua,sizeof(buff_chengxuhua));
					buff_chengxuhua[1]=chengxu_1_24_lock(buff_chengxuhua[1]);				//��ȡ��ǰ������Χ���ж��Ƿ񳬳���Χ
					WriteDGUS(0x2560+Ad_chengxu,(uint8_t *)&(buff_chengxuhua),sizeof(buff_chengxuhua));
		}
		
		else 	//�ڳ��򻯽��水�³���������36�����������Ѿ��ϴ��Ĵ���
			{
				
				//WriteDGUS(before_ad+0x3,(uint8_t *)&(colour_w),sizeof(colour_w));
			}

			}

	//		/***********************����ҳ��*****************************/u16 char_hide=0xff00;
//u16 char_attend_rtc=0x1300;
				else 	if(Page_s[1]==Page_shezhi)
	{
		if(getDar==0x1440	||	getDar==0x1441)	//��Ļ����
		{
			set_page_write();
		}
			else if(getDar==0x1400)		//rtc set finish
	{
		RTC_SET_IN();	
	}
	else if(getDar==0x1410)		//����Ԥ��д�� flash
	{
		zhiliao_SET();
	}
	else if(getDar==0x1420)		//�����Զ����� 
	{
		Sudoopen_set_handle();
	}
	else if(getDar==0x1430)					//�ָ���������
	{
	 reset_data();
	}
	else if(getDar==0x1280)
	{
		
		ReadDGUS(0x1280,(uint8_t *)&(dynamic_static_flag),sizeof(dynamic_static_flag));		//�ж��Ǿ�̬ʱ�� ���Ƕ�̬ʱ��
		WriteNorFlash_Str(Nor_P_set,(uint8_t *)&(dynamic_static_flag),2);	
		
	}
//	else if(getDar==0x2420)		//��������ģʽ�� ��ʽ
//	{
//		WriteDGUS(0x1470,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
//	}
	
	
//		{
//			WriteDGUS(0xfe00,(uint8_t *)&char_hide,sizeof(char_hide));		//vp 0xfe00
//			set_rtc_finish=0;
	}
	
	/****************************************�ֶ�ģʽ*********************************************/
		else if(Page_s[1]==Page_shoudong)
	{
					if(getDar==0x1505)		//�ֶ�ģʽ ���Ӱ�������
			{
				WriteDGUS(0x1465,(uint8_t *)&(icon_exit),sizeof(icon_exit));		//������������ͼ������ ������ס
				Lock_current_page_zengliang(0x06,0x02,0x07,0x02);
				WriteDGUS(0x1470,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
				Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
				control_shoudong_flag=1;
				Unlock_current_page_zengliang(0x02,0x02,0x03,0x02);
				WriteDGUS(0x1460,(uint8_t *)&(icon_attend),sizeof(icon_attend));	
			}
			else if(getDar==0x1510)		//�ֶ�ģʽ�� ����on����
				
			{
				WriteDGUS(0x1470,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
				Lock_current_page_zengliang(0x06,0x02,0x07,0x02);
				WriteDGUS(0X1460,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
				Lock_current_page_zengliang(0x02,0x02,0x03,0x02);

				control_shoudong_flag=2;
				Unlock_current_page_zengliang(0x04,0x02,0x05,0x02);
				WriteDGUS(0x1465,(uint8_t *)&(icon_attend),sizeof(icon_attend));	
			}
			else if(getDar==0x1515)		//���ֶ�ģʽ�� ����off����
			{		
				WriteDGUS(0x1465,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
				Lock_current_page_zengliang(0x02,0x02,0x03,0x02);
				WriteDGUS(0x1460,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
				Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
				control_shoudong_flag=3;
				Unlock_current_page_zengliang(0x06,0x02,0x07,0x02);
				WriteDGUS(0x1470,(uint8_t *)&(icon_attend),sizeof(icon_attend));	
			}

				
			else if(Page_s[1]==Page_shoudong	)	//�ֶ�ģʽ�� ��ⰴ����	��ס�ϴε��������� �����ϴε�����ͼ��
			{
				
				 if(getDar==0x3010)		//���ֶ�ģʽ�°��¿�ʼ����
			{
			//	mode_4_Init();
			//	shoudong_read_norflash();
			}
				switch(control_shoudong_flag)
				{
					case 0:	{break;}
					
					case 1:	
						{	
							control_shoudong_flag=0;	
							WriteDGUS(0x1460,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x02,0x02,0x03,0x02);
						}
						
									case 2:	
						{	
							control_shoudong_flag=0;	
							WriteDGUS(0x1465,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
						}
						
									case 3:	
						{	
							control_shoudong_flag=0;	
							WriteDGUS(0x1470,(uint8_t *)&(icon_exit),sizeof(icon_exit));	
							Lock_current_page_zengliang(0x06,0x02,0x07,0x02);
						}
									default :{break;}
						
				}
				
			
		}

	}
	/************��������***********/
	else 	if(Page_s[1]==Page_lianji)
	{
		
		
			 if(getDar==0x4600)
		{
			ReadDGUS(0x4600,(uint8_t *)&uart4_open_buff,sizeof(uart4_open_buff));

					if(uart4_open_buff[1]) 
			{
						Lock_current_page();	EA=0;ET2=0;EX1=0;ET0=0;EX0=0;ES3R=0;ES3T=0;ECAN=0;IEN2=0;ES2R=1;ES2T=1;EA=1;
						Uart4_Open();
			}
			
		}
}
	}
EA=1;
}
///**********ģ����Ļ���***********/
void Analog_click(u16 x,u16 y)
{
	static u16 buff_analog[4]={0x5aa5,0x0004,0,0};
	
	buff_analog[2]=x;
	buff_analog[3]=y;
	
	
	WriteDGUS(0xd4,(uint8_t *)&buff_analog,sizeof(buff_analog));	
}
/*****************************************************************************
������*/
void HandleProc(void)
{
	//EA=0;
		if(GetValue0F00())
		{
			WriteDGUS(0x5200, (uint8_t *)&test_error[4],sizeof(test_error[4]));		
		  TouchHandle();
			WriteDGUS(0x5200, (uint8_t *)&test_error[5],sizeof(test_error[5]));		

		}
	
	//EA=1;	
}
