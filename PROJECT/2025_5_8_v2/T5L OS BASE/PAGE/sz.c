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
����Ԥ��
Ƥ�������� ���������� ���������� ����ģʽ��norflash�и���һ����ַ���ֿ��洢����ԭʼ���ݴ洢
������norflash�л���һ����ַ���ѽṹ�����ʽ�洢ÿһ��ǰ���б�־λ���洢��u8����ʽ
������norflash�л���һ����ַ����ԭʼ���ݴ洢���ĸ� u16����ǰ����u16�ı�־λ

*************************************************************************/


void zhiliao_date_INIT()	//��NORFLASH��ȡ��д��DGUS����Ļ��Ӧ��ֵ
{
		u16 buff_user[4];
	//	u8 check[6];
//	u8 Nor_buff[12];
//	u8 H[3];
//	u8 M[3];
//	u8 L[3];
	
	 /***********�̶�ͷ*************/
	zhiliao_set_nor.Head_set_H=0XF0;
	zhiliao_set_nor.Head_set_M=0XF1;
	zhiliao_set_nor.Head_set_L=0XF2;
	
	ReadNorFlash_Dgus(Nor_zhiliao_set_Ad,SET_zhiliao_moshi_Adress,6);	//��ȡ�������ַ 0x3200 ����������
	ReadNorFlash_Str(Nor_zhiliao_set_Ad,(uint8_t *)&zhiliao_set_nor,6);	//��ȡ���ṹ�� 
	
	ReadNorFlash_Str(Nor_zhiliao_user_Ad,(uint8_t*)&buff_user,4);			//�����ϴ� �ֶ�¼����������
	if(buff_user[0]==0x00f6)
	{
		WriteDGUS(0x2400, (uint8_t *)&buff_user[1],sizeof(buff_user[1]));	
		WriteDGUS(0x2410, (uint8_t *)&buff_user[2],sizeof(buff_user[2]));	
		WriteDGUS(0x2420, (uint8_t *)&buff_user[3],sizeof(buff_user[3]));	
		//WriteDGUS(0x1700, (uint8_t *)&buff_user,sizeof(buff_user));	
	}				
	ReadNorFlash_Dgus(Nor_zhiliao_set_initial_3_Ad, 0X1320,2);		//����ҳ��ĵ�ַ �����㷨�������u8
	ReadNorFlash_Dgus(Nor_zhiliao_set_initial_2_Ad, 0X1450,2);	
	ReadNorFlash_Dgus(Nor_zhiliao_set_initial_1_Ad, 0X1310,2);
	
}

void zhiliao_check()	//������������ģʽ������ģʽ��ֵ�Ƿ���Ϲ淶
{
	
	
	
}


void zhiliao_SET()		//��������ҳ�� handle��
{
	
	u8 MJ_H[4];	//2023 02 18  ������
								//0	 1	2	 3
	u8 MJ_M[4];	//00	12 45 23	ʱ����
								//0		1	 2	3
	u8	MJ_L[4];
 u8 MJ_H_M_L[12]={0};
//u8 Nor_buff[10];	//0xf7
	u32 MJ_H_HEX,MJ_L_HEX,MJ_M_HEX;
	
// /***********�̶�ͷ*************/
//zhiliao_set_nor.Head_set_H=0XF0;
//zhiliao_set_nor.Head_set_M=0XF1;
//zhiliao_set_nor.Head_set_L=0XF2;

	ReadDGUS(0X1310,(uint8_t *)&MJ_H,sizeof(MJ_H));
	ReadDGUS(0X1450,(uint8_t *)&MJ_M,sizeof(MJ_M));
	ReadDGUS(0X1320,(uint8_t *)&MJ_L,sizeof(MJ_L));
// Nor_buff[0]=0xf7;			//ͷ��=֡0xf7
// Nor_buff[1]=MJ_H[1];
// Nor_buff[2]=MJ_H[2];
// Nor_buff[3]=MJ_H[3];
// Nor_buff[4]=MJ_M[1];
// Nor_buff[5]=MJ_M[2];
// Nor_buff[6]=MJ_M[3];
// Nor_buff[7]=MJ_L[1];
// Nor_buff[8]=MJ_L[2];
// Nor_buff[9]=MJ_L[3];
		
//WriteNorFlash_Str(Nor_zhiliao_set_initial_1_Ad, (uint8_t *)&Nor_buff,6);		//д��norflash��δ�����㷨�����Դ��
	WriteNorFlash_Dgus(Nor_zhiliao_set_initial_3_Ad, 0X1320,2);	
	WriteNorFlash_Dgus(Nor_zhiliao_set_initial_2_Ad, 0X1450,2);	
	WriteNorFlash_Dgus(Nor_zhiliao_set_initial_1_Ad, 0X1310,2);

	MJ_H_HEX=(u32)(MJ_H[0])<<24	|	(u32)(MJ_H[1])<<16	| (u32)(MJ_H[2])<<8	| (u32)(MJ_H[3]);
	MJ_M_HEX=(u32)(MJ_M[0])<<24	|	(u32)(MJ_M[1])<<16	| (u32)(MJ_M[2])<<8	| (u32)(MJ_M[3]);
	MJ_L_HEX=(u32)(MJ_L[0])<<24	|	(u32)(MJ_L[1])<<16	| (u32)(MJ_L[2])<<8	| (u32)(MJ_L[3]);
	
	
	zhiliao_set_nor.H_mj=(MJ_H_HEX/10000)%1000;
//	if(zhiliao_set_nor.H_mj==0 || zhiliao_set_nor.H_mj>=100)	//���Ϸ�
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
//	WriteDGUS(0X3200, (uint8_t *)&zhiliao_set_nor,sizeof(zhiliao_set_nor));			//����
}




void SZ_INIT()	//����HOME_INIT���ڿ�����ʼ�� ֻ����һ��
{
	zhiliao_date_INIT();	//��norflash��ȡ�����Ƶ�����ģʽ�����ݣ�����д�뵽����ҳ��
	
	
	
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