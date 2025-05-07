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
先写数据位再写时间


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
/************************************************读取norflah 0x00中地址*******************************/
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
			WriteDGUS(0x1280, (uint8_t *)&dynamic_static_flag,sizeof(dynamic_static_flag));		//功率自适应

		
		
	ReadNorFlash_Str(Nor_Next_Adress,(uint8_t*)&Nor_Adress,2);
	Adress_Change_0=Nor_Adress[1];		/****读取上个norflash地址*****/
	DelayMs(5);
	if(Adress_Change_0<=0x28||Adress_Change_0>=0x4000)
	{
		Adress_Change_0=0x28;Nor_Adress[0]=0x00;Nor_Adress[1]=0x28;
		WriteNorFlash_Str(Nor_Next_Adress,(uint8_t*)&Nor_Adress,2);	
		DelayMs(5);
	}

	
	ReadNorFlash_Dgus(Nor_Guanji_Adress,Dgus_Buff_Reserve,4);		/***读取上一次关机的时间****/
	DelayMs(5);
	WriteNorFlash_Str((Adress_Change_0),(uint8_t*)&Close,4);/***写入flash关机校验码****/
	DelayMs(5);
	Adress_Change_0=0x4+Adress_Change_0;	norflash_check_Adress();
	WriteNorFlash_Dgus(Adress_Change_0,Dgus_Buff_Reserve,4);/***写入上次关机的时间***/
	DelayMs(5);
	Adress_Change_0=Adress_Change_0+0x4;	norflash_check_Adress();
		//

	//printf("%d\n",Adress);
	WriteNorFlash_Str(Adress_Change_0,(uint8_t*)&Open,4);		/****写入本次开机的校验码*****/
	DelayMs(5);
	Adress_Change_0=Adress_Change_0+0x4;	norflash_check_Adress();
	WriteNorFlash_Dgus(Adress_Change_0,RTC_Adress,4);					/***写入本次开机的时间****/
	DelayMs(5);
	Adress_Change_0=Adress_Change_0+0x4;	norflash_check_Adress();
Nor_Adress[0]=0x00;Nor_Adress[1]=Adress_Change_0;
	WriteNorFlash_Str(Nor_Next_Adress,(uint8_t*)&Nor_Adress,2);		/******更新地址位*******/
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

	bianliang_Write[7]=0xCC;		/*****0x02开启*****/	

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

			bianliang_Write[7]=0xDD;			/*******0x04暂停******/

				
				
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
			bianliang_Write[7]=0xEE;			/*******0x08关闭******/

				
			WriteNorFlash_Str(Adress_Change_0,(uint8_t*)&bianliang_Write,4);
			Adress_Change_0=Adress_Change_0+0x4;	norflash_check_Adress();				
				
			WriteNorFlash_Dgus(Adress_Change_0,RTC_Adress,4);
			Adress_Change_0=Adress_Change_0+0x4;norflash_check_Adress();
				
	Nor_Adress[0]=0x00;Nor_Adress[1]=Adress_Change_0;
			WriteNorFlash_Str(Nor_Next_Adress,(uint8_t*)&Nor_Adress,2);
			

EA=1;	
	
	
}


void norflash_Write_guanji(void)			/******5s一循环******/
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
		printf("\n\n\n\r	串口已连接 命令如下： \n\n\n\n");		DelayMs(5);
	printf("\n\r		1)UV TEST		说明：测试串口			 \n\n\n\n");
						DelayMs(5);		printf("\n\r		2)UV READ		说明：读取FlashROM（0x28~0x4000）		 \n\n\n\n");
						DelayMs(5);		printf("\n\r		3)UV CLEAN		说明：Clean Flash Rom				\n\n\n\n");
//	printf("		（0x28）			 \n\n\n\n");
						DelayMs(5);		printf("\n\r		4)UV ADRESS		说明：查看当前写入地址			 \n\n\n\n");
				DelayMs(5);		printf("\n\r		注意事项：每次法指令需要加回车或换行（\\r\\n）			 \n\n\n\n");
	while(flag)
	{
		DelayMs(5);
		//printf("\n接收指令");
		DelayMs(5);	//printf("\n	test	%bx",0x000005);printf("\n	test	%bx",0x123456);
				  if( rec_ok )
        {
            rec_ok = 0;
            uartRecCnt = 0;
					//	printf("1111");
            match_cmd( uartRecStr );
            memset( uartRecStr, 0, sizeof( uartRecStr ) );		//清空备份数组 需要添加头文件 string.h
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

/****************************屏幕亮度初始化*******************
屏幕正常亮度、待机亮度、待机时间
***************************************************************/
u8 Screen_Light[4];
//0中是屏幕亮度 1待机亮度 2-3时间 min


u8 Screen_Touch_flag=0;	//0表示当前是正常状态	1表示已经息屏
u16 Screen_set_Time_min;

void Screen_Init()		//初始化
{
	ReadNorFlash_Str(Nor_set_light_Ad,(uint8_t*)&Screen_Light,2);
	WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	
	WriteDGUS(0x1440,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	
	Screen_set_Time_min=Screen_Light[2]<<8|Screen_Light[3];	
}

/***********************用户写入时间****************/
void set_page_write()	//handl set界面摁下才执行
{
	ReadDGUS(0x1440,(uint8_t *)&Screen_Light,sizeof(Screen_Light));
	WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));
	WriteNorFlash_Str(Nor_set_light_Ad,(uint8_t*)&Screen_Light,2);
	Screen_set_Time_min=Screen_Light[2]<<8|Screen_Light[3];
}

void Screen_review()	//主函数检测息屏时间是否到达
{
	static u8 flag;
	static u8 clean=0xf0;
	ReadDGUS(0x16,(uint8_t *)&flag,sizeof(flag));
	if(flag==0x5a	&&	Screen_Touch_flag==0)
	{
		WriteDGUS(0x16,(uint8_t *)&clean,sizeof(clean));	
		Screen_Time_500ms=0;
	}
	else if(Screen_Time_500ms>=(Screen_set_Time_min*120)	&& Screen_Touch_flag==0)	//要息屏
	{
		Screen_Touch_flag=1;
		WriteDGUS(0x82,(uint8_t *)&Screen_Light[1],sizeof(Screen_Light[1]));		
	}
	else if(Screen_Touch_flag==1	&&	flag==0x5a)	//息屏后触摸
	{
		WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));
		Screen_Touch_flag=0;
		Screen_Time_500ms=0;		
	}
	
}


/**************************开机自动启动*************************
_ _ _	=最大0x0f ff 每次取4bit,例：510	开机自动进入治疗模式并且自动选择皮肤面照射 但不自动进行

bit2 [0,7]	页面
bit1 [1,4]	进入当前页面的模式
bit0 [0,1]	0不自动进行，1自动进行
***************************************************************/
 u8 sudo_buff[4];

	
static struct{
	u8 Head_sudo;
	u8 page;
	u8 mode;
	u8 status;
}sudoopen_set_nor;
void open_sudo_Init()		//开机初始化
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



void Sudoopen_set_handle()		//set界面按下处理函数	自动启动
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
/****************联机界面 从Norflash中读取数据 然后写入到指定的Dgus地址*********************
Dgus:地址分配 0x1601~0x1603 作为时间地址HEX显示 0x1604作为数据地址 ASCII文本显示
			以此往后类推 0x1611~0x1613 0x1614 多保留字节0x1600以防其他用途

Norflash:地址分配 先读取Nor_Next_Adress 0x0	地址判断当前写到的地址每次往前退0x8。时间在后占用4四个字，
			内容在前占用4个字（高字节0XAA作为头帧，低字节：开机0xcc 暂停0xdd 复位0xee 关机0xdd）

运算规则：先读取next_adress查看当前地址是否有数据，有的话就说明现在已经进行覆盖了，读取-0x8地址如果小于0x28并且已经覆盖
					则读取0x2000，如果没有覆盖则跳出循环，将最近一次的操作记录为1号以此类推
*********************************************************************************************/

#define Status_turn_on 0xbfaabbfa	//开机
#define Status_turn_off 0xb9d8bbfa 	//关机
#define Status_open_uv 0xbfaac6f4	//开启
#define UV_ziwai 0xD7CFCDE2		//紫外
#define UV_xiandeng 0xCFDFB5C6	//线灯
#define Status_close_uv 0xB8B4CEBB	//复位
#define Status_stop_uv 0xD4DDCDA3	//暂停

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
	u8 M;//固定为0x00
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

void lianji_run()	//检测在联机界面 此函数只运行一次 
{
	u8 buff_nor[16];	//内容+时间
	u8 i;
	u16 Adress;
	EA=0;
	//u8 check[4];
	//u8 return_nor=0;	//0表示未覆盖 1表示覆盖了
	Link_Norflash.end	=	0xffff;
	Adress	=	Adress_Change_0;
	
//	ReadNorFlash_Str(Nor_Next_Adress,(uint8_t*)&check,2);
//	if(check[0]==0xaa)	//开始覆盖了
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
		if(buff_nor[0]==0xaa)		//有数据
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
	u16 buff_user[4]={0x00f6,0xa,0x5,0x2};//手动 10min 5on 2off
	
	u16 buff_zhiliao[4]={0x00f6,0x1,0x5,0x2};//治疗 1mj 5on 2off
	
	u16 buff_chengxuhua_0[18]={0x0008,0x0000,0x0009,0x0,0x000a,0x000a,0x0009,0x0,0x000a,0x0,0x000a,0x000a,0x000a,0x0,0x000b,0x0,0x000a,0x000a};//程序化 
	u16 buff_chengxuhua_1[17]={0x000e,0x0,0x000f,0x0,0x000a,0x000a,0x000f,0x0,0x0010,0x0,0x000a,0x000a,0x007f,0x007e,0x0041,0x002b,0x0054};//程序化 
	
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
	
	/******手动**********/


	
		WriteDGUS(0x1490, (uint8_t *)&buff_user[1],sizeof(buff_user[1]));	
		WriteDGUS(0x1495, (uint8_t *)&buff_user[2],sizeof(buff_user[2]));	
		WriteDGUS(0x1500, (uint8_t *)&buff_user[3],sizeof(buff_user[3]));	
		
		WriteNorFlash_Str(Nor_shoudong_user_Ad, (uint8_t *)&buff_user,4);	//写入norflash
		
		
		/********治疗**********/
		WriteDGUS(0x2400, (uint8_t *)&buff_zhiliao[1],sizeof(buff_zhiliao[1]));		//毫焦
		WriteDGUS(0x2410, (uint8_t *)&buff_zhiliao[2],sizeof(buff_zhiliao[2]));		//脉冲on
		WriteDGUS(0x2420, (uint8_t *)&buff_zhiliao[3],sizeof(buff_zhiliao[3]));		//脉冲off
		WriteNorFlash_Str(Nor_zhiliao_user_Ad, (uint8_t *)&buff_zhiliao,4);	
		
		
		/********程序化*******/
		WriteDGUS(0x2560, (uint8_t *)&buff_chengxuhua_0,sizeof(buff_chengxuhua_0));		//脉冲on
		WriteDGUS(0x2572, (uint8_t *)&buff_chengxuhua_1,sizeof(buff_chengxuhua_1));		//脉冲off
		chengxu_Updae();
		
		
		WriteDGUS(0x1280, (uint8_t *)&icon_exit,sizeof(icon_exit));		//功率自适应
		
		ReadDGUS(0x1280,(uint8_t *)&(dynamic_static_flag),sizeof(dynamic_static_flag));	
		P_SET[0]=dynamic_static_flag;
		WriteNorFlash_Str(Nor_P_set,(uint8_t *)&(P_SET),2);	
		
}