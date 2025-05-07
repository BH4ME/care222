#include "sys_params.h"
#include "norflash.h"
#include <string.h>
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include "cmd.h"
#include "uart4.h"
#include "handle.h"
#include "mode_4.h"
#include "T5LUart4.h"
/*****************************************************************
��д����λ��дʱ��


*******************************************************************/

u16 Adress_Change_0;
u16 Nor_Adress[2];

void norflash_check_Adress(void)
{
	EA=0;
	if(Adress_Change_0>=0x4000	||	Adress_Change_0<=0x28)
	{
		Adress_Change_0=0x28;
	}
	EA=1;
	
}
/************************************************��ȡnorflah 0x00�е�ַ*******************************/
void Init_Adress(void)
{
	uint8_t Close[8]={0xAA,0,0,0,0,0,0,0xff};
		uint8_t Open[8]={0xAA,0,0,0,0,0,0,0xBB};
	
		
	uchar dat[64]={0};
	u16 p_set[2]={0};
//	uchar dat_txst_1[4]={0x4,0x2,0,0};
	//uchar dat_txst[4]={0x0,0x20,0,0};
	EA=0;
	
	
set_shoudong_Init();
set_zhiliao_Init();
	
			ReadNorFlash_Str(Nor_P_set,(uint8_t *)&(p_set),2);	
			dynamic_static_flag=p_set[0];
			WriteDGUS(0x1280, (uint8_t *)&dynamic_static_flag,sizeof(dynamic_static_flag));		//��������Ӧ

		
		
	ReadNorFlash_Str(Nor_Next_Adress,(uint8_t*)&Nor_Adress,2);
	Adress_Change_0=Nor_Adress[1];		/****��ȡ�ϸ�norflash��ַ*****/
	DelayMs(5);
	if(Adress_Change_0<=0x28||Adress_Change_0>=0x4000)
	{
		Adress_Change_0=0x28;Nor_Adress[0]=0x00;Nor_Adress[1]=0x28;
		WriteNorFlash_Str(Nor_Next_Adress,(uint8_t*)&Nor_Adress,2);	
		DelayMs(5);
	}

	
	ReadNorFlash_Dgus(Nor_Guanji_Adress,Dgus_Buff_Reserve,4);		/***��ȡ��һ�ιػ���ʱ��****/
	DelayMs(5);
	WriteNorFlash_Str((Adress_Change_0),(uint8_t*)&Close,4);/***д��flash�ػ�У����****/
	DelayMs(5);
	Adress_Change_0=0x4+Adress_Change_0;	norflash_check_Adress();
	WriteNorFlash_Dgus(Adress_Change_0,Dgus_Buff_Reserve,4);/***д���ϴιػ���ʱ��***/
	DelayMs(5);
	Adress_Change_0=Adress_Change_0+0x4;	norflash_check_Adress();
		//

	//printf("%d\n",Adress);
	WriteNorFlash_Str(Adress_Change_0,(uint8_t*)&Open,4);		/****д�뱾�ο�����У����*****/
	DelayMs(5);
	Adress_Change_0=Adress_Change_0+0x4;	norflash_check_Adress();
	WriteNorFlash_Dgus(Adress_Change_0,RTC_Adress,4);					/***д�뱾�ο�����ʱ��****/
	DelayMs(5);
	Adress_Change_0=Adress_Change_0+0x4;	norflash_check_Adress();
Nor_Adress[0]=0x00;Nor_Adress[1]=Adress_Change_0;
	WriteNorFlash_Str(Nor_Next_Adress,(uint8_t*)&Nor_Adress,2);		/******���µ�ַλ*******/
	DelayMs(5);
	
	EA=1;
}
/*********************************************************************************************************/



void norflash_Write_Open_UV(void)
{
	uint8_t bianliang_Write[8]={0};
	uint8_t T[2];

	EA=0;

		T[0]=T_XIANSHI>>8;
		T[1]=T_XIANSHI&0x00ff;


	bianliang_Write[0]=0xAA;
	bianliang_Write[1]=Page_s[1];

	bianliang_Write[7]=0xCC;		/*****0x02����*****/	

	WriteNorFlash_Str(Adress_Change_0,(uint8_t*)&bianliang_Write,4);
	Adress_Change_0=Adress_Change_0+0x4;	norflash_check_Adress();
	WriteNorFlash_Dgus(Adress_Change_0,RTC_Adress,4);
	Adress_Change_0=Adress_Change_0+0x4;norflash_check_Adress();	
	Nor_Adress[0]=0x00;Nor_Adress[1]=Adress_Change_0;
	WriteNorFlash_Str(Nor_Next_Adress,(uint8_t*)&Nor_Adress,2);
	


EA=1;	
	
	
}

void norflash_Write_Stop_UV(void)
{
			uint8_t bianliang_Write[8]={0};

	EA=0;

			bianliang_Write[0]=0xAA;
			bianliang_Write[1]=Page_s[1];

			bianliang_Write[7]=0xDD;			/*******0x04��ͣ******/

				
				
			WriteNorFlash_Str(Adress_Change_0,(uint8_t*)&bianliang_Write,4);
			Adress_Change_0=Adress_Change_0+0x4;	norflash_check_Adress();				
			WriteNorFlash_Dgus(Adress_Change_0,RTC_Adress,4);
			Adress_Change_0=Adress_Change_0+0x4;norflash_check_Adress();
				

	Nor_Adress[0]=0x00;Nor_Adress[1]=Adress_Change_0;
			WriteNorFlash_Str(Nor_Next_Adress,(uint8_t*)&Nor_Adress,2);

EA=1;	

	
}


void norflash_Write_Close_UV(void)
{
			uint8_t bianliang_Write[8]={0};

	EA=0;

			bianliang_Write[0]=0xAA;
			bianliang_Write[1]=0x00;
			bianliang_Write[2]=0x00;
			bianliang_Write[3]=0x00;
			bianliang_Write[4]=0x00;
			bianliang_Write[5]=0x00;
			bianliang_Write[6]=0x00;
			bianliang_Write[7]=0xEE;			/*******0x08�ر�******/

				
			WriteNorFlash_Str(Adress_Change_0,(uint8_t*)&bianliang_Write,4);
			Adress_Change_0=Adress_Change_0+0x4;	norflash_check_Adress();				
				
			WriteNorFlash_Dgus(Adress_Change_0,RTC_Adress,4);
			Adress_Change_0=Adress_Change_0+0x4;norflash_check_Adress();
				
	Nor_Adress[0]=0x00;Nor_Adress[1]=Adress_Change_0;
			WriteNorFlash_Str(Nor_Next_Adress,(uint8_t*)&Nor_Adress,2);
			

EA=1;	
	
	
}


void norflash_Write_guanji(void)			/******5sһѭ��******/
{
	
	
	EA=0;

	
	
	WriteNorFlash_Dgus(Nor_Guanji_Adress,RTC_Adress,4);

	EA=1;
}




void Uart4_Open(void)
{
	u8 flag=1;

	DelayMs(5);
	Uart4Init();
	printf("connect");
	
			DelayMs(5);
		printf("\n\n\n\r	���������� �������£� \n\n\n\n");		DelayMs(5);
	printf("\n\r		1)UV TEST		˵�������Դ���			 \n\n\n\n");
						DelayMs(5);		printf("\n\r		2)UV READ		˵������ȡFlashROM��0x28~0x4000��		 \n\n\n\n");
						DelayMs(5);		printf("\n\r		3)UV CLEAN		˵����Clean Flash Rom				\n\n\n\n");
//	printf("		��0x28��			 \n\n\n\n");
						DelayMs(5);		printf("\n\r		4)UV ADRESS		˵�����鿴��ǰд���ַ			 \n\n\n\n");
				DelayMs(5);		printf("\n\r		ע�����ÿ�η�ָ����Ҫ�ӻس����У�\\r\\n��			 \n\n\n\n");
	while(flag)
	{
		DelayMs(5);
		//printf("\n����ָ��");
		DelayMs(5);	//printf("\n	test	%bx",0x000005);printf("\n	test	%bx",0x123456);
				  if( rec_ok )
        {
            rec_ok = 0;
            uartRecCnt = 0;
					//	printf("1111");
            match_cmd( uartRecStr );
            memset( uartRecStr, 0, sizeof( uartRecStr ) );		//��ձ������� ��Ҫ���ͷ�ļ� string.h
        }	
		
				ReadDGUS(0x4600,(uint8_t *)&uart4_open_buff,sizeof(uart4_open_buff));
				if(uart4_open_buff[1]==0)
				{
					printf("exit");		
					Unlock_current_page();
					flag=0;
				}

	}
	
		
	
	
	
	

	ES0=0;ET0=1;ET1=1;ES3R=1;ES3T=1;ET2=1;

	EA=1;
	
}

/****************************��Ļ���ȳ�ʼ��*******************
��Ļ�������ȡ��������ȡ�����ʱ��
***************************************************************/
u8 Screen_Light[4];
//0������Ļ���� 1�������� 2-3ʱ�� min


u8 Screen_Touch_flag=0;	//0��ʾ��ǰ������״̬	1��ʾ�Ѿ�Ϣ��
u16 Screen_set_Time_min;

void Screen_Init()		//��ʼ��
{
	ReadNorFlash_Str(Nor_set_light_Ad,(uint8_t*)&Screen_Light,2);
	WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	
	WriteDGUS(0x1440,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	
	Screen_set_Time_min=Screen_Light[2]<<8|Screen_Light[3];	
}

/***********************�û�д��ʱ��****************/
void set_page_write()	//handl set�������²�ִ��
{
	ReadDGUS(0x1440,(uint8_t *)&Screen_Light,sizeof(Screen_Light));
	WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));
	WriteNorFlash_Str(Nor_set_light_Ad,(uint8_t*)&Screen_Light,2);
	Screen_set_Time_min=Screen_Light[2]<<8|Screen_Light[3];
}

void Screen_review()	//���������Ϣ��ʱ���Ƿ񵽴�
{
	static u8 flag;
	static u8 clean=0xf0;
	ReadDGUS(0x16,(uint8_t *)&flag,sizeof(flag));
	if(flag==0x5a	&&	Screen_Touch_flag==0)
	{
		WriteDGUS(0x16,(uint8_t *)&clean,sizeof(clean));	
		Screen_Time_500ms=0;
	}
	else if(Screen_Time_500ms>=(Screen_set_Time_min*120)	&& Screen_Touch_flag==0)	//ҪϢ��
	{
		Screen_Touch_flag=1;
		WriteDGUS(0x82,(uint8_t *)&Screen_Light[1],sizeof(Screen_Light[1]));		
	}
	else if(Screen_Touch_flag==1	&&	flag==0x5a)	//Ϣ������
	{
		WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));
		Screen_Touch_flag=0;
		Screen_Time_500ms=0;		
	}
	
}


/**************************�����Զ�����*************************
_ _ _	=���0x0f ff ÿ��ȡ4bit,����510	�����Զ���������ģʽ�����Զ�ѡ��Ƥ�������� �����Զ�����

bit2 [0,7]	ҳ��
bit1 [1,4]	���뵱ǰҳ���ģʽ
bit0 [0,1]	0���Զ����У�1�Զ�����
***************************************************************/
 u8 sudo_buff[4];

	
static struct{
	u8 Head_sudo;
	u8 page;
	u8 mode;
	u8 status;
}sudoopen_set_nor;
void open_sudo_Init()		//������ʼ��
{
	u16 buff_user[4];
	//static

	ReadNorFlash_Dgus(Nor_set_sudoopen_initial_Ad,0x1325,2);		
	ReadNorFlash_Str(Nor_set_sudoopen_Ad,(uint8_t*)&sudo_buff,2);	
	//WriteDGUS(0x3220,(uint8_t *)&sudo_buff,sizeof(sudo_buff));
		if(sudo_buff[0]==0xf5)	
	{
		
	//	JumpPage((u16 *)&sudo_buff[1]);
		switch(sudo_buff[1])
		{
			case 0:{JumpPage(Page_home);	break;}
			
			case 1:{JumpPage(Page_qirongjiao);	break;	}
				
			case 2:	{JumpPage(Page_zhongxin);	break;}
			
			case 3:	{JumpPage(Page_quyu);	break;}

			case 4:	{JumpPage(Page_chengxuhua);	break;}

			case 5:	{JumpPage(Page_zhiliao);		
			if(sudo_buff[2]==1)
			{
				ReadNorFlash_Str(Nor_zhiliao_user_Ad,(uint8_t*)&buff_user,4);	
				if(buff_user[0]==0xf6)
				{	
					WriteDGUS(0x2400, (uint8_t *)&buff_user[1],sizeof(buff_user[1]));	
					WriteDGUS(0x2410, (uint8_t *)&buff_user[2],sizeof(buff_user[2]));	
					WriteDGUS(0x2420, (uint8_t *)&buff_user[3],sizeof(buff_user[3]));		
				}
				
			}
			else if(sudo_buff[2]==2){Analog_click(0x01f4,0x0064);}
						else if(sudo_buff[2]==3){Analog_click(0X01f4,0X008c);}
									else if(sudo_buff[2]==4){Analog_click(0X01F4,0X00B1);}
								
				//DelayMs(500);
			//if(sudo_buff[3]){Analog_click(0x02ad,0x0082);}
									break;}

			case 6:	{JumpPage(Page_shoudong);	break;}

			case 7:	{JumpPage(Page_lianji);	break;}
				
			default :	{break;}
		}	
//		
//	//	if(sudo_buff[1]!=5)
//				set_zhiliao_Init();

	}
	else JumpPage(Page_home);
	
}



void Sudoopen_set_handle()		//set���水�´�����	�Զ�����
{
	u8 read_buff[2];
	u16 concordance;
	ReadDGUS(0x1325,(uint8_t *)&read_buff,sizeof(read_buff));
	{		
		concordance=(u16)(read_buff[0])<<8	|	(u16)(read_buff[1]);	
		sudoopen_set_nor.Head_sudo=0xf5;
		sudoopen_set_nor.status=concordance%10;
		sudoopen_set_nor.mode=(concordance/10)%10;
		sudoopen_set_nor.page=(concordance/100)%10;
		WriteNorFlash_Str(Nor_set_sudoopen_Ad,(uint8_t*)&sudoopen_set_nor,2);	
		WriteNorFlash_Dgus(Nor_set_sudoopen_initial_Ad,0x1325,2);		

		//	WriteDGUS(0x3210,(uint8_t *)&read_buff,sizeof(read_buff));
	}
	
	
}
void sudo_click_start()
{
	if(sudo_buff[3])Analog_click(0x02ad,0x0082);
}
/****************�������� ��Norflash�ж�ȡ���� Ȼ��д�뵽ָ����Dgus��ַ*********************
Dgus:��ַ���� 0x1601~0x1603 ��Ϊʱ���ַHEX��ʾ 0x1604��Ϊ���ݵ�ַ ASCII�ı���ʾ
			�Դ��������� 0x1611~0x1613 0x1614 �ౣ���ֽ�0x1600�Է�������;

Norflash:��ַ���� �ȶ�ȡNor_Next_Adress 0x0	��ַ�жϵ�ǰд���ĵ�ַÿ����ǰ��0x8��ʱ���ں�ռ��4�ĸ��֣�
			������ǰռ��4���֣����ֽ�0XAA��Ϊͷ֡�����ֽڣ�����0xcc ��ͣ0xdd ��λ0xee �ػ�0xdd��

��������ȶ�ȡnext_adress�鿴��ǰ��ַ�Ƿ������ݣ��еĻ���˵�������Ѿ����и����ˣ���ȡ-0x8��ַ���С��0x28�����Ѿ�����
					���ȡ0x2000�����û�и���������ѭ���������һ�εĲ�����¼Ϊ1���Դ�����
*********************************************************************************************/

#define Status_turn_on 0xbfaabbfa	//����
#define Status_turn_off 0xb9d8bbfa 	//�ػ�
#define Status_open_uv 0xbfaac6f4	//����
#define UV_ziwai 0xD7CFCDE2		//����
#define UV_xiandeng 0xCFDFB5C6	//�ߵ�
#define Status_close_uv 0xB8B4CEBB	//��λ
#define Status_stop_uv 0xD4DDCDA3	//��ͣ

#define open_qrj_0 0xC6F8C8DC
#define open_qrj_1 0xBDBAC4A3
#define open_qrj_2 0xCABDffff

#define open_zx_0 0xD6D0D0C4
#define open_zx_1 0xC4A3CABD

#define open_qy_0 0xC7F8D3F2
#define open_qy_1 0xC4A3CABD

#define open_cxh_0 0xB3CCD0F2
#define open_cxh_1 0xBBAFC4A3
#define open_cxh_2 0xCABDffff

#define open_zl_0 0xD6CEC1C6
#define open_zl_1 0xC4A3CABD

#define open_sd_0 0xCAD6B6Af
#define open_sd_1 0xC4A3CABD



static struct{
	u8 Y;
	u8 M;//�̶�Ϊ0x00
	u8 D;
	u8 H;
	u8 Min;
	u8 Sec;
//	{
	
	u32 Status;
	u32 Status_0;
		u32 Status_1;
		u32 Status_2;
	//u16	mode;	
	//}Content[5];
	u16 end;
}Link_Norflash;

void lianji_run()	//������������� �˺���ֻ����һ�� 
{
	u8 buff_nor[16];	//����+ʱ��
	u8 i;
	u16 Adress;
	EA=0;
	//u8 check[4];
	//u8 return_nor=0;	//0��ʾδ���� 1��ʾ������
	Link_Norflash.end	=	0xffff;
	Adress	=	Adress_Change_0;
	
//	ReadNorFlash_Str(Nor_Next_Adress,(uint8_t*)&check,2);
//	if(check[0]==0xaa)	//��ʼ������
//	{
//		return_nor=1;
//	}

	for(i=0;i<=9;i++)
	{
		if(Adress>0x28)
		{	
			Adress=Adress-0x8;
		}
		else break;
//		else if(return_nor==1)
//		{
//			Adress=0x2000;
//		}
//		else break;

		ReadNorFlash_Str(Adress,(uint8_t*)&buff_nor,8);	
		if(buff_nor[0]==0xaa)		//������
		{
			
				Link_Norflash.Y=buff_nor[8];
				Link_Norflash.M=buff_nor[9];
				Link_Norflash.D=buff_nor[10];
				Link_Norflash.H=buff_nor[12];
				Link_Norflash.Min=buff_nor[13];
				Link_Norflash.Sec=buff_nor[14];
				
						if(buff_nor[7]==0xff){Link_Norflash.Status=Status_turn_off;	Link_Norflash.Status_0=0XFFFFFFFF;}
			else if(buff_nor[7]==0xbb){Link_Norflash.Status=Status_turn_on;Link_Norflash.Status_0=0XFFFFFFFF;}
			else if(buff_nor[7]==0xcc)
				{
					switch(buff_nor[1])
					{
						case Page_qirongjiao:
						{Link_Norflash.Status=Status_open_uv;
							Link_Norflash.Status_0=open_qrj_0;
							Link_Norflash.Status_1=open_qrj_1;
							Link_Norflash.Status_2=open_qrj_2;
							break;
						}
						case Page_zhongxin:
						{					Link_Norflash.Status=Status_open_uv;		Link_Norflash.Status_0=open_zx_0;	Link_Norflash.Status_1=open_zx_1;Link_Norflash.Status_2=0XFFFFFFFF;break;}
							
						case Page_quyu:
						{					Link_Norflash.Status=Status_open_uv;		Link_Norflash.Status_0=open_qy_0;Link_Norflash.Status_1=open_qy_1;Link_Norflash.Status_2=0XFFFFFFFF;
						break;}
							
						case Page_chengxuhua:
						{	
							
							Link_Norflash.Status=Status_open_uv;
							Link_Norflash.Status_0=open_cxh_0;
							Link_Norflash.Status_1=open_cxh_1;
							Link_Norflash.Status_2=open_cxh_2;
						break;}
							
						case Page_zhiliao:
						{						Link_Norflash.Status=Status_open_uv;	Link_Norflash.Status_0=open_zl_0;Link_Norflash.Status_1=open_zl_1;Link_Norflash.Status_2=0XFFFFFFFF;	break;}
													case Page_zhiliao_JUMP:
						{						Link_Norflash.Status=Status_open_uv;	Link_Norflash.Status_0=open_zl_0;Link_Norflash.Status_1=open_zl_1;Link_Norflash.Status_2=0XFFFFFFFF;	break;}
							
						case Page_shoudong:
						{						Link_Norflash.Status=Status_open_uv;	Link_Norflash.Status_0=open_sd_0;Link_Norflash.Status_1=open_sd_1;	Link_Norflash.Status_2=0XFFFFFFFF;break;}
							
						case Page_chengxuhua_JUMP:
							{				Link_Norflash.Status=Status_open_uv;		Link_Norflash.Status_0=open_cxh_0;
							Link_Norflash.Status_1=open_cxh_1;
							Link_Norflash.Status_2=open_cxh_2;	break;}
								
							default :
							{break;}
					}
		//	Link_Norflash.Status=Status_open_uv;
			}
			else if(buff_nor[7]==0xee){Link_Norflash.Status=Status_close_uv;Link_Norflash.Status_0=0XFFFFFFFF;}
			else if(buff_nor[7]==0xdd){Link_Norflash.Status=Status_stop_uv;Link_Norflash.Status_0=0XFFFFFFFF;}
			
			WriteDGUS(0x1601+(i*0x10),(uint8_t *)&Link_Norflash,sizeof(Link_Norflash));		
		}
	//	else break;
	}
	EA=1;
}

//void lianji_Init()
//{
//	
//	//Link_Norflash.Content[0].Status=Status_turn_on;
//	//Link_Norflash.Content[1].Status=Status_turn_on;
//	//Link_Norflash.Content[2].Status=Status_turn_on;
//	//Link_Norflash.Content[3].Status=Status_turn_on;
//	//Link_Norflash.Content[4].Status=Status_turn_on;	
//	
//}

void reset_data()
{
	uchar RTC_H[4]={0x01,0x34,0xb0,0x9d};	//2023 03 01
	u8 RTC_L[4]={0x00,0x01,0x44,0x38};//08:30:00
	
	u8 MJ_H[4]={0x00,0x00,0x59,0xf6};//02 30 30
	u8 MJ_M[4]={0x00,0x00,0xc7,0x42};//05 10 10
	u8 MJ_L[4]={0x00,0x01,0x88,0x99};//10 05 05
	
	u8 read_buff[2]={0};	//000
	u16 buff_user[4]={0x00f6,0xa,0x5,0x2};//�ֶ� 10min 5on 2off
	
	u16 buff_zhiliao[4]={0x00f6,0x1,0x5,0x2};//���� 1mj 5on 2off
	
	u16 buff_chengxuhua_0[18]={0x0008,0x0000,0x0009,0x0,0x000a,0x000a,0x0009,0x0,0x000a,0x0,0x000a,0x000a,0x000a,0x0,0x000b,0x0,0x000a,0x000a};//���� 
	u16 buff_chengxuhua_1[17]={0x000e,0x0,0x000f,0x0,0x000a,0x000a,0x000f,0x0,0x0010,0x0,0x000a,0x000a,0x007f,0x007e,0x0041,0x002b,0x0054};//���� 
	
	u16 P_SET[2]={0};
	Screen_Light[0]=0x64;//100%
	Screen_Light[1]=0xf;//15%
	Screen_Light[2]=0x0;//15%
	Screen_Light[3]=0xf;//15min
	
	WriteDGUS(0X1300,(uint8_t *)&RTC_H,sizeof(RTC_H));
	WriteDGUS(0X1305,(uint8_t *)&RTC_L,sizeof(RTC_L));
	RTC_SET_IN();
	
	WriteDGUS(0X1310,(uint8_t *)&MJ_H,sizeof(MJ_H));
	WriteDGUS(0X1450,(uint8_t *)&MJ_M,sizeof(MJ_M));
	WriteDGUS(0X1320,(uint8_t *)&MJ_L,sizeof(MJ_L));
	zhiliao_SET();
	
	WriteDGUS(0x1325,(uint8_t *)&read_buff,sizeof(read_buff));
	Sudoopen_set_handle();
	
	WriteDGUS(0x1440,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	
	WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));
	WriteNorFlash_Str(Nor_set_light_Ad,(uint8_t*)&Screen_Light,2);
	Screen_set_Time_min=Screen_Light[2]<<8|Screen_Light[3];
	
	/******�ֶ�**********/


	
		WriteDGUS(0x1490, (uint8_t *)&buff_user[1],sizeof(buff_user[1]));	
		WriteDGUS(0x1495, (uint8_t *)&buff_user[2],sizeof(buff_user[2]));	
		WriteDGUS(0x1500, (uint8_t *)&buff_user[3],sizeof(buff_user[3]));	
		
		WriteNorFlash_Str(Nor_shoudong_user_Ad, (uint8_t *)&buff_user,4);	//д��norflash
		
		
		/********����**********/
		WriteDGUS(0x2400, (uint8_t *)&buff_zhiliao[1],sizeof(buff_zhiliao[1]));		//����
		WriteDGUS(0x2410, (uint8_t *)&buff_zhiliao[2],sizeof(buff_zhiliao[2]));		//����on
		WriteDGUS(0x2420, (uint8_t *)&buff_zhiliao[3],sizeof(buff_zhiliao[3]));		//����off
		WriteNorFlash_Str(Nor_zhiliao_user_Ad, (uint8_t *)&buff_zhiliao,4);	
		
		
		/********����*******/
		WriteDGUS(0x2560, (uint8_t *)&buff_chengxuhua_0,sizeof(buff_chengxuhua_0));		//����on
		WriteDGUS(0x2572, (uint8_t *)&buff_chengxuhua_1,sizeof(buff_chengxuhua_1));		//����off
		chengxu_Updae();
		
		
		WriteDGUS(0x1280, (uint8_t *)&icon_exit,sizeof(icon_exit));		//��������Ӧ
		
		ReadDGUS(0x1280,(uint8_t *)&(dynamic_static_flag),sizeof(dynamic_static_flag));	
		P_SET[0]=dynamic_static_flag;
		WriteNorFlash_Str(Nor_P_set,(uint8_t *)&(P_SET),2);	
		
}