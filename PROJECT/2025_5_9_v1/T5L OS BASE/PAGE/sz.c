#include "sz.h"
#include "timer.h"
#include "adc.h"
#include "interactive.h"
#include "norflash.h"



void 	SZ_prepare()
{
	if(ADC_100ms >= 2)	//200ms
	{
		T_100MS_ADC_isr();
		ADC_100ms=0;
	}
	
}


/********************************************************************
治疗预设
皮肤面照射 创伤面照射 烧伤面照射 三个模式在norflash中各有一个地址，分开存储，以原始数据存储
整体在norflash中还有一个地址，已结构体的形式存储每一个前面有标志位，存储以u8的形式
整体在norflash中还有一个地址，以原始数据存储，四个 u16，最前面有u16的标志位

*************************************************************************/


void zhiliao_date_INIT()	//从NORFLASH读取后，写入DGUS和屏幕对应的值
{
		u16 buff_user[4];
	//	u8 check[6];
//	u8 Nor_buff[12];
//	u8 H[3];
//	u8 M[3];
//	u8 L[3];
	
	 /***********固定头*************/
	zhiliao_set_nor.Head_set_H=0XF0;
	zhiliao_set_nor.Head_set_M=0XF1;
	zhiliao_set_nor.Head_set_L=0XF2;
	
	ReadNorFlash_Dgus(Nor_zhiliao_set_Ad,SET_zhiliao_moshi_Adress,6);	//读取到虚拟地址 0x3200 处理后的数据
	ReadNorFlash_Str(Nor_zhiliao_set_Ad,(uint8_t *)&zhiliao_set_nor,6);	//读取到结构体 
	
	ReadNorFlash_Str(Nor_zhiliao_user_Ad,(uint8_t*)&buff_user,4);			//加载上次 手动录入三个变量
	if(buff_user[0]==0x00f6)
	{
		WriteDGUS(0x2400, (uint8_t *)&buff_user[1],sizeof(buff_user[1]));	
		WriteDGUS(0x2410, (uint8_t *)&buff_user[2],sizeof(buff_user[2]));	
		WriteDGUS(0x2420, (uint8_t *)&buff_user[3],sizeof(buff_user[3]));	
		//WriteDGUS(0x1700, (uint8_t *)&buff_user,sizeof(buff_user));	
	}				
	ReadNorFlash_Dgus(Nor_zhiliao_set_initial_3_Ad, 0X1320,2);		//设置页面的地址 经过算法处理完的u8
	ReadNorFlash_Dgus(Nor_zhiliao_set_initial_2_Ad, 0X1450,2);	
	ReadNorFlash_Dgus(Nor_zhiliao_set_initial_1_Ad, 0X1310,2);
	
}

void zhiliao_check()	//检查输入的治疗模式下三个模式的值是否符合规范
{
	
	
	
}


void zhiliao_SET()		//放在设置页面 handle中
{
	
	u8 MJ_H[4];	//2023 02 18  年月日
								//0	 1	2	 3
	u8 MJ_M[4];	//00	12 45 23	时分秒
								//0		1	 2	3
	u8	MJ_L[4];
 u8 MJ_H_M_L[12]={0};
//u8 Nor_buff[10];	//0xf7
	u32 MJ_H_HEX,MJ_L_HEX,MJ_M_HEX;
	
// /***********固定头*************/
//zhiliao_set_nor.Head_set_H=0XF0;
//zhiliao_set_nor.Head_set_M=0XF1;
//zhiliao_set_nor.Head_set_L=0XF2;

	ReadDGUS(0X1310,(uint8_t *)&MJ_H,sizeof(MJ_H));
	ReadDGUS(0X1450,(uint8_t *)&MJ_M,sizeof(MJ_M));
	ReadDGUS(0X1320,(uint8_t *)&MJ_L,sizeof(MJ_L));
// Nor_buff[0]=0xf7;			//头枕=帧0xf7
// Nor_buff[1]=MJ_H[1];
// Nor_buff[2]=MJ_H[2];
// Nor_buff[3]=MJ_H[3];
// Nor_buff[4]=MJ_M[1];
// Nor_buff[5]=MJ_M[2];
// Nor_buff[6]=MJ_M[3];
// Nor_buff[7]=MJ_L[1];
// Nor_buff[8]=MJ_L[2];
// Nor_buff[9]=MJ_L[3];
		
//WriteNorFlash_Str(Nor_zhiliao_set_initial_1_Ad, (uint8_t *)&Nor_buff,6);		//写入norflash中未经过算法处理的源码
	WriteNorFlash_Dgus(Nor_zhiliao_set_initial_3_Ad, 0X1320,2);	
	WriteNorFlash_Dgus(Nor_zhiliao_set_initial_2_Ad, 0X1450,2);	
	WriteNorFlash_Dgus(Nor_zhiliao_set_initial_1_Ad, 0X1310,2);

	MJ_H_HEX=(u32)(MJ_H[0])<<24	|	(u32)(MJ_H[1])<<16	| (u32)(MJ_H[2])<<8	| (u32)(MJ_H[3]);
	MJ_M_HEX=(u32)(MJ_M[0])<<24	|	(u32)(MJ_M[1])<<16	| (u32)(MJ_M[2])<<8	| (u32)(MJ_M[3]);
	MJ_L_HEX=(u32)(MJ_L[0])<<24	|	(u32)(MJ_L[1])<<16	| (u32)(MJ_L[2])<<8	| (u32)(MJ_L[3]);
	
	
	zhiliao_set_nor.H_mj=(MJ_H_HEX/10000)%1000;
//	if(zhiliao_set_nor.H_mj==0 || zhiliao_set_nor.H_mj>=100)	//不合法
//	{
//		zhiliao_set_nor.H_mj=1;
//		
//	}
	zhiliao_set_nor.H_on=(MJ_H_HEX/100)%100;
	zhiliao_set_nor.H_off=MJ_H_HEX%100;
	
	zhiliao_set_nor.M_mj=(MJ_M_HEX/10000)%1000;
	zhiliao_set_nor.M_on=(MJ_M_HEX/100)%100;
	zhiliao_set_nor.M_off=MJ_M_HEX%100;

	
	zhiliao_set_nor.L_mj=(MJ_L_HEX/10000)%1000;
	zhiliao_set_nor.L_on=(MJ_L_HEX/100)%100;
	zhiliao_set_nor.L_off=MJ_L_HEX%100;

	WriteNorFlash_Str(Nor_zhiliao_set_Ad, (uint8_t *)&zhiliao_set_nor,6);	
//	WriteDGUS(0X3200, (uint8_t *)&zhiliao_set_nor,sizeof(zhiliao_set_nor));			//测试
}




void SZ_INIT()	//放在HOME_INIT用于开机初始化 只进行一次
{
	zhiliao_date_INIT();	//从norflash读取到治疗的三个模式的数据，并且写入到设置页面
	
	
	
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