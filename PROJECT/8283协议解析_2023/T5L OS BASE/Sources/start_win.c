#include "start_win.h"
#include "column_chart.h"
#include <stdio.h>
#include "uart4.h"
#include <math.h>
#include "handle.h"
//#define START_WIN_CHART_VP		0x4000		//����ͼ�οؼ���VP��ַ
//#define COLUMN_CHART_HEIGHT		295				//��״ͼ�ĸ߶�


//bit is_anim = 1;
//bit style = 0;
//u16 color = 0xf800;
//uint16_t init_val[3];
//	uint16_t MAX;
//COLUMN_CHART chart;
//u16 start_win_get_val(s16 index);

//u16 x1_0=0x85;
//u16 y1_0=0x364;

uint Draw_W_Flag=0;		//0���������ߣ�1��������
uint First_Draw_Flag=0;		//ֻ����һ��


	u16 Y_zuobiao;
	u16 X_zuobiao;
	
	float S_Write;		


u16 Adress_Now;

double P_mW_First=0.0;
double W_mJ_zong=0.0;
float W_mJ_doing=0.0;
double T_First=0.0; //��ʼʱ�䲻���
uint T_doing=0.0; //�ѽ��е�ʱ��
uint T_All=0;
u8 Page_Lasttime=0;
u8 Page_s[2]={0};
void X_Y_Calculate();
void Draw_juxing();
void Draw_juxing_zuobiao();
void S_D_Surfaces_All();
void Unlock_current_page_zengliang(u8 id_1,u8 daima_1,u8 id_2,u8 daima_2);
void Lock_current_page_zengliang(u8 id_1,u8 daima_1,u8 id_2,u8 daima_2);
u8 second_firstdraw_flag=0;	//�ڶ��λ�ͼ��flag
uint Y_zuobiao_static=0;
u16 chart_num =0;
void sys_write_chart_zhexian_Two(u16 x1,u16 y1,u16 x2,u16 y2)		//����2����
{
	#define CHART_ADDR		0xb000	//��̬���߹�������Ӧ��ϵͳ�����ӿڵ�ַ
	//#define CHART_NUM			0xff				//�߶�������,����ֻд��chart_idָ��������ͨ��,��1��
	//#define POINT_NUM			1  			//һ����д����ٸ����ݵ�
	#define colour 0Xffff
	u8 chart_cmd[16] = {0x00,0x02};
	chart_cmd[2]=(u8)(chart_num>>8);
	chart_cmd[3]=(u8)(chart_num&0xff);
	chart_cmd[4] = (u8)(colour>>8);//����ͨ��id
	chart_cmd[5] = (u8)(colour&0xff);//һ����д����ٸ����ݵ�
	chart_cmd[6] = (u8)(x1>>8);//x1
	chart_cmd[7] = (u8)(x1&0xff);
	chart_cmd[8] = (u8)(y1>>8);//y1
	chart_cmd[9] = (u8)(y1&0xff);
	chart_cmd[10] = (u8)(x2>>8);//x2
	chart_cmd[11] = (u8)(x2&0xff);
	chart_cmd[12] = (u8)(y2>>8);//y2
	chart_cmd[13] = (u8)(y2&0xff);
		chart_cmd[14] = (u8)0xff;//y2
	chart_cmd[15] = (u8)0x00;			//0xa007 
//	y1=y2;
//	x1=x2;
	Adress_Now=CHART_ADDR+0x7;chart_num++;
	//��������
	WriteDGUS(CHART_ADDR,chart_cmd,sizeof(chart_cmd));
}
void sys_write_chart_zhexian_Two_Cankao(u16 x1,u16 y1,u16 x2,u16 y2)		//����2����
{
	#define CHART_ADDR_cankao		0x5500	//��̬���߹�������Ӧ��ϵͳ�����ӿڵ�ַ
	#define CHART_NUM_cankao			0xff				//�߶�������,����ֻд��chart_idָ��������ͨ��,��1��
	//#define POINT_NUM			1  			//һ����д����ٸ����ݵ�
	#define colour_cankao 0X8410
	u8 chart_cmd[16] = {0x00,0x02,0x0f,CHART_NUM_cankao};
	chart_cmd[4] = (u8)(colour_cankao>>8);//����ͨ��id
	chart_cmd[5] = (u8)(colour_cankao&0xff);//һ����д����ٸ����ݵ�
	chart_cmd[6] = (u8)(x1>>8);//x1
	chart_cmd[7] = (u8)(x1&0xff);
	chart_cmd[8] = (u8)(y1>>8);//y1
	chart_cmd[9] = (u8)(y1&0xff);
	chart_cmd[10] = (u8)(x2>>8);//x2
	chart_cmd[11] = (u8)(x2&0xff);
	chart_cmd[12] = (u8)(y2>>8);//y2
	chart_cmd[13] = (u8)(y2&0xff);
	chart_cmd[14] = (u8)0xff;//y2
	chart_cmd[15] = (u8)0x00;			//0xa007 
//	y1=y2;
//	x1=x2;
	//Adress_Now=0xa007;
	//��������
	WriteDGUS(CHART_ADDR_cankao,chart_cmd,sizeof(chart_cmd));
}
#define tubiao_chang 580	//ͼ��ĳ���
#define tubiao_kuan 120	//ͼ��Ŀ��
#define tubiao_zuoshang_x 10	//ͼ�������x����
#define tubiao_zuoshang_y 346	//ͼ�������y����
#define tubiao_0zuobiao_x 10 //0������x
#define tubiao_0zuobiao_y 426 //0������y
//�߶�6�� 2.5 -1
//#define tubiao_chang 570	//ͼ��ĳ���
//#define tubiao_kuan 112	//ͼ��Ŀ��
//#define tubiao_zuoshang_x 17	//ͼ�������x����
//#define tubiao_zuoshang_y 350	//ͼ�������y����
//#define tubiao_0zuobiao_x 17 //0������x
//#define tubiao_0zuobiao_y 430 //0������y
void Write_New(u16 x1,u16 y1)//ÿ��д3��
{

		u8 chart_cmd[6] = {0};
		u8 chart[2]={0};
		EA=0;	
	chart_cmd[0] = (u8)(x1>>8);//x1
	chart_cmd[1] = (u8)(x1&0xff);
	chart_cmd[2] = (u8)(y1>>8);//y1
	chart_cmd[3] = (u8)(y1&0xff);		//0xa007 
	chart_cmd[4] = (u8)0xff;
	chart_cmd[5] = (u8)0x00;			//0xa007 
		
	chart[0]=(u8)(chart_num>>8);
	chart[1]=(u8)(chart_num&0xff);
//		if(Adress_Now>=0xb000)
//		{
//			Adress_Now=0x0a007;
//		}
		/******************�ж�xy�������Ƿ񳬳����ֵ**********************************/
		if(x1>=(tubiao_chang+tubiao_zuoshang_x))	//�����Ҳ೤��ֵ
		{
			x1=tubiao_chang+tubiao_zuoshang_x;
		}
		else if(x1<tubiao_zuoshang_x){x1=tubiao_zuoshang_x;}
		
		if(y1>=(tubiao_zuoshang_y+tubiao_kuan))
		{
			y1=tubiao_zuoshang_y+tubiao_kuan-2;
		}
		else if(y1<tubiao_zuoshang_y){y1=tubiao_zuoshang_y+2;}
		
		/******************************************************************************/
		WriteDGUS(CHART_ADDR+0x1,chart,sizeof(chart));	//����������
		WriteDGUS(Adress_Now,chart_cmd,sizeof(chart_cmd));
				
		Adress_Now=Adress_Now+0x2;chart_num++;
		EA=1;
}

	u16 X_zuobiao_second;
//u8 second_flag=0;		//û�õĵ��ڶ������ߵ�ʱ��Ϊ0 �õ��ڶ������ߵ�ʱ��Ϊ1
/*******************************д��ڶ������� ��ʱ��䳤��ʱ��**************************/
u16 chart_num_2 =0;
u16 Adress_Now_2;
void sys_write_chart_zhexian_Two_2(u16 x1,u16 y1,u16 x2,u16 y2)		//����2����
{
	#define CHART_ADDR_2		0x8000	//��̬���߹�������Ӧ��ϵͳ�����ӿڵ�ַ
	//#define CHART_NUM			0xff				//�߶�������,����ֻд��chart_idָ��������ͨ��,��1��
	//#define POINT_NUM			1  			//һ����д����ٸ����ݵ�
	#define colour_2 0Xffe0
	
	u8 chart_cmd[16] = {0x00,0x02};
	EA=0;
	chart_num_2=1;
	chart_cmd[2]=(u8)(chart_num_2>>8);
	chart_cmd[3]=(u8)(chart_num_2&0xff);
	chart_cmd[4] = (u8)(colour_2>>8);//����ͨ��id
	chart_cmd[5] = (u8)(colour_2&0xff);//һ����д����ٸ����ݵ�
	chart_cmd[6] = (u8)(x1>>8);//x1
	chart_cmd[7] = (u8)(x1&0xff);
	chart_cmd[8] = (u8)(y1>>8);//y1
	chart_cmd[9] = (u8)(y1&0xff);
	chart_cmd[10] = (u8)(x2>>8);//x2
	chart_cmd[11] = (u8)(x2&0xff);
	chart_cmd[12] = (u8)(y2>>8);//y2
	chart_cmd[13] = (u8)(y2&0xff);
		chart_cmd[14] = (u8)0xff;//y2
	chart_cmd[15] = (u8)0x00;			//0xa007 
//	y1=y2;
//	x1=x2;
	X_zuobiao_second=tubiao_zuoshang_x+0x2;
	Adress_Now_2=CHART_ADDR_2+0x7;chart_num_2++;
	//��������
	WriteDGUS(CHART_ADDR_2,chart_cmd,sizeof(chart_cmd));
	EA=1;
}
void Write_New_2(u16 x1,u16 y1)//ÿ��д3��
{

		u8 chart_cmd[6] = {0};
		u8 chart[2]={0};
		EA=0;	
	chart_cmd[0] = (u8)(x1>>8);//x1
	chart_cmd[1] = (u8)(x1&0xff);
	chart_cmd[2] = (u8)(y1>>8);//y1
	chart_cmd[3] = (u8)(y1&0xff);		//0xa007 
	chart_cmd[4] = (u8)0xff;
	chart_cmd[5] = (u8)0x00;			//0xa007 
		
	chart[0]=(u8)(chart_num_2>>8);
	chart[1]=(u8)(chart_num_2&0xff);
//		if(Adress_Now>=0xb000)
//		{
//			Adress_Now=0x0a007;
//		}
		/******************�ж�xy�������Ƿ񳬳����ֵ**********************************/
		if(x1>=(tubiao_chang+tubiao_zuoshang_x))	//�����Ҳ೤��ֵ
		{
			x1=tubiao_chang+tubiao_zuoshang_x;
		}
		else if(x1<tubiao_zuoshang_x){x1=tubiao_zuoshang_x;}
		
		if(y1>=(tubiao_zuoshang_y+tubiao_kuan))
		{
			y1=tubiao_zuoshang_y+tubiao_kuan-2;
		}
		else if(y1<tubiao_zuoshang_y){y1=tubiao_zuoshang_y+2;}
		
		/******************************************************************************/
		WriteDGUS(CHART_ADDR_2+0x1,chart,sizeof(chart));	//����������
		WriteDGUS(Adress_Now_2,chart_cmd,sizeof(chart_cmd));
				
		Adress_Now_2=Adress_Now_2+0x2;chart_num_2++;
		EA=1;
}

/**********************************************************************************************/
void X_Y_huoqu()
{
	EA=0;
	/*******************************����Y������ֵΪ����**********************************/
				if(change_P_mW>=0)
			{
				Y_zuobiao	=	tubiao_0zuobiao_y-(change_P_mW/2.5*(tubiao_0zuobiao_y-tubiao_zuoshang_y));
			}
			else 
				{

					Y_zuobiao	=	(tubiao_0zuobiao_y)+(fabs(change_P_mW)/1*(tubiao_zuoshang_y+tubiao_kuan-tubiao_0zuobiao_y));	
					if(change_P_mW==-1)
					{Y_zuobiao=Y_zuobiao-2;}
				}
				
			if(Y_zuobiao==Y_zuobiao_static)Y_zuobiao=Y_zuobiao+1;
	/****************************************����X������ֵΪ����*************************************/			

					if(Draw_1ms>=S_Write)
					{	
						
						Draw_1ms=0;
						
						if(X_zuobiao<=tubiao_zuoshang_x+tubiao_chang-0x2)	//ͼ�� 1
						{
							X_zuobiao=2+X_zuobiao;					
							Write_New(X_zuobiao,Y_zuobiao);
						}
						else	if(dynamic_static_flag	&&	T_All!=T_dynamic)	//ͼ��	2
						{
							//second_firstdraw_flag=1;
							if(second_firstdraw_flag==0)
							{
								
								sys_write_chart_zhexian_Two_2(tubiao_0zuobiao_x,tubiao_0zuobiao_y,tubiao_0zuobiao_x,tubiao_0zuobiao_y);
								second_firstdraw_flag=1;
							}
							X_zuobiao_second=X_zuobiao_second+2;
							Write_New_2(X_zuobiao_second,Y_zuobiao);
							
							
						}
						
						
					}
EA=1;

}


/***************����ģʽ��ʱ�䴦�ڶ�̬**����������****************/
u16 Adress_Now_dynamic;

u16 dynamic_X_zuobiao;
u16 dynamic_Y_zuobiao;
void dynamic_draw_chart_Init()
{
	#define Dynamic_chart_adress_vp 0x1907
	u8 buff_dynamic[2]={0x00,0x02};
	//���������������ܵ����ߴ�1*1�����2*2 д��VPָ��
		WriteDGUS(Dynamic_chart_adress_vp,buff_dynamic,sizeof(buff_dynamic));
	
}
void dynamic_first_Init(u16 x1,u16 y1,u16 x2,u16 y2)
{
		#define CHART_ADDR_dynamic			0xd000	//��̬���߹�������Ӧ��ϵͳ�����ӿڵ�ַ
	#define CHART_NUM_dynamic			0xff				//�߶�������,����ֻд��chart_idָ��������ͨ��,��1��
	//#define POINT_NUM			1  			//һ����д����ٸ����ݵ�
	#define colour_dynamci 0Xf800
	u8 chart_cmd[16] = {0x00,0x02,0x0f,CHART_NUM_dynamic};
	chart_cmd[4] = (u8)(colour_dynamci>>8);//����ͨ��id
	chart_cmd[5] = (u8)(colour_dynamci&0xff);//һ����д����ٸ����ݵ�
	chart_cmd[6] = (u8)(x1>>8);//x1
	chart_cmd[7] = (u8)(x1&0xff);
	chart_cmd[8] = (u8)(y1>>8);//y1
	chart_cmd[9] = (u8)(y1&0xff);
	chart_cmd[10] = (u8)(x2>>8);//x2
	chart_cmd[11] = (u8)(x2&0xff);
	chart_cmd[12] = (u8)(y2>>8);//y2
	chart_cmd[13] = (u8)(y2&0xff);
	chart_cmd[14] = (u8)0xff;//y2
	chart_cmd[15] = (u8)0x00;			
	Adress_Now_dynamic=CHART_ADDR_dynamic+0x7;
	WriteDGUS(CHART_ADDR_dynamic,chart_cmd,sizeof(chart_cmd));
	dynamic_X_zuobiao=tubiao_0zuobiao_x+2;
}
void dynamic_Write_New(u16 x1,u8 y1_h,u8 y1_l)//ÿ��д3��
{

		u8 chart_cmd[6] = {0};
		u8 chart[2]={0};
		u16 y1;
		EA=0;	
	chart_cmd[0] = (u8)(x1>>8);//x1
	chart_cmd[1] = (u8)(x1&0xff);
	chart_cmd[2] = (u8)(y1_h);//y1
	chart_cmd[3] = (u8)(y1_l);		//0xa007 
	chart_cmd[4] = (u8)0xff;
	chart_cmd[5] = (u8)0x00;			//0xa007 
		y1=((u16)(y1_h))<<8	|	(u16)(y1_l);
		/******************�ж�xy�������Ƿ񳬳����ֵ**********************************/
		if(x1>=(tubiao_chang+tubiao_zuoshang_x))	//�����Ҳ೤��ֵ
		{
			x1=tubiao_chang+tubiao_zuoshang_x;
		}
		else if(x1<tubiao_zuoshang_x){x1=tubiao_zuoshang_x;}
		
		if(y1>=(tubiao_zuoshang_y+tubiao_kuan))
		{
			y1=tubiao_zuoshang_y+tubiao_kuan-2;
		}
		else if(y1<tubiao_zuoshang_y){y1=tubiao_zuoshang_y+2;}
		
		/******************************************************************************/
		WriteDGUS(Adress_Now_dynamic,chart_cmd,sizeof(chart_cmd));
		Adress_Now_dynamic=Adress_Now_dynamic+0x2;
		EA=1;
}
	u8 dynamic_y[2];
void dynamic_X_Y_huoqu()		//�Ե�һ�����߽�������
{

	uint i,j=0;
//	u16 AD_NOW;
	uint draw_num=0;
float beishu=0;
	float cha;
u16 adress_now;
	EA=0;
	adress_now=CHART_ADDR+0x4;
	draw_num=290-chart_num;
//	beishu=chart_num/draw_num;
	beishu=(uint)((290.00/(float)(chart_num))*4);
			j=beishu;
	dynamic_first_Init(tubiao_0zuobiao_x,tubiao_0zuobiao_y,tubiao_0zuobiao_x+2,tubiao_0zuobiao_y);
	//AD_NOW=Adress_Now-0x1-0xb002;
	
	
	
					if(T_First==T_dynamic)	//����û�б仯
				{  
												for(i=0;i<=289;i++)	//2*2���� ������580 һ��Ҫд��192����ÿ������1������
					{
							adress_now=adress_now+0x2;
							ReadDGUS(adress_now,dynamic_y,sizeof(dynamic_y));	
							dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
							dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
					}
				}
				
				else if(beishu!=1)
				{
							for(i=0;i<=289;i++)	//2*2���� ������580 һ��Ҫд��192����ÿ������1������
					{
							if(j>=beishu)
							{
								j=1;
								adress_now=adress_now+0x2;
								ReadDGUS(adress_now,dynamic_y,sizeof(dynamic_y));	
								dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
								dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
							}
							else
							{
								j++;
								dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
								dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
								
							}
							
							
					}
				}
				else //>=195
				{
					cha=290-chart_num;
					beishu=((float)(chart_num)/cha)*5;
					j=0;
					for(i=0;i<=289;i++)
					{
						j++;
						if(j>=beishu)
						{
							j=1;
															dynamic_X_zuobiao=dynamic_X_zuobiao+0Xa;
								dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
						}
						else 
						{
							j++;
								adress_now=adress_now+0x2;
								ReadDGUS(adress_now,dynamic_y,sizeof(dynamic_y));	
								dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
								dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
						}
						
						if(dynamic_X_zuobiao>=tubiao_zuoshang_x+tubiao_chang-0x2)
						{
							break;
						}
					}
				}
				
				
				
				
				
//	if(beishu!=0)		//����һ�ĵ�С��192����
//	{
//					for(i=0;i<=289;i++)	//2*2���� ������580 һ��Ҫд��192����ÿ������1������
//			{
//				
//				//���жϵ�һ������ûд��ͽ����������

//				if(T_All==T_dynamic)	//����û�б仯
//				{  
//							adress_now=adress_now+0x2;
//							ReadDGUS(adress_now,dynamic_y,sizeof(dynamic_y));	
//							dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
//							dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
//				}
//				else
//				{
//				
//				if(j>beishu)
//				{

//					j=1;
//					
//							dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
//							dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
//					
//				}
//				else 
//						{
//							j++;
//							adress_now=adress_now+0x2;
//							ReadDGUS(adress_now,dynamic_y,sizeof(dynamic_y));	
//							dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
//							dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
//						}
//				
//					}
//					//dynamic_Y_zuobiao=(u16)(dynamic_y[0])<<8	||	(u16)(dynamic_y[1]);

//			}
//	}
//	else	//ʣ�µĵ��������Ѿ�д��� �ܹ�290�� �Ѿ�д��90 ����Ҫ200
//	{
//		
//		beishu=draw_num/chart_num;
//		j=beishu;
//							for(i=0;i<=289;i++)	//2*2���� ������580 һ��Ҫд��192����ÿ������1������
//			{
//						if(j>=beishu)
//				{

//					j=0;									
//							adress_now=adress_now+0x2;
//							ReadDGUS(adress_now,dynamic_y,sizeof(dynamic_y));	
//							dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
//							dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);

//					
//				}
//				else 
//						{
//							j++;
//							dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
//							dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
//						}
		//	}
	//}
	EA=1;
}
void dynamic_X_Y_huoqu_2()	//�Ե�һ���͵ڶ������߽��г���
{
	//u8 dynamic_y[2];
	u16 num_all;
	u16 num_throwaway,adress_now,adress_now_2;
	u8 i,j;
	u16 num_reset=0;
	float bei=0.0;
	EA=0;
	adress_now=CHART_ADDR+0x4;
	adress_now_2=CHART_ADDR_2+0x4;
	num_all=(chart_num_2+chart_num)-290;
	bei=((float)(chart_num_2+chart_num))/(float)(num_all);
	num_throwaway=bei*2;
	dynamic_first_Init(tubiao_0zuobiao_x,tubiao_0zuobiao_y,tubiao_0zuobiao_x+2,tubiao_0zuobiao_y);
	//AD_NOW=Adress_Now-0x1-0xb002;
	if(num_throwaway!=0)		//����һ�ĵ�С��192����
	{
					for(i=0;i<=285;i++)	//2*2���� ������580 һ��Ҫд��192����ÿ������1������
			{
						if(j>=num_throwaway)
						{
							num_reset++;
							j=1;
								if(adress_now<=Adress_Now-0x2)
							{
								adress_now=adress_now+0x6;	
							//	ReadDGUS(adress_now,dynamic_y,sizeof(dynamic_y));						
							}
							else 	if(adress_now_2<=Adress_Now_2-0x2)
							{
								adress_now_2=adress_now_2+0x6;		
							//	ReadDGUS(adress_now_2,dynamic_y,sizeof(dynamic_y));							
							}
							else break;
						}
						else 
						{
							j++;						
						}
						
						
							if(adress_now<=Adress_Now-0x2)
							{
								adress_now=adress_now+0x2;	
								ReadDGUS(adress_now,dynamic_y,sizeof(dynamic_y));	
								
							}
							else if(adress_now_2<=Adress_Now_2-0x2)
							{
								adress_now_2=adress_now_2+0x2;		
								ReadDGUS(adress_now_2,dynamic_y,sizeof(dynamic_y));					
							}	
							else 		DelayMs(5);					
							
							DelayMs(10);
							dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
							dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
								DelayMs(10);
			
				
					}
	
				}
	EA=1;
}


void dynamic_draw()
{
	EA=0;
	if(MODE_2_RUN_Draw==1&&	dynamic_static_flag)
	{
		if(second_firstdraw_flag)	//�õ��ڶ������� ����������Ӧ��Ϊ��һ�����ߺ͵ڶ������ߵĳ���
		{
			dynamic_X_Y_huoqu_2();MODE_2_RUN_Draw=0;			dynamic_draw_chart_Init();
		}
		else
			{dynamic_X_Y_huoqu();			dynamic_draw_chart_Init();}		//ֻ�õ���һ������
		MODE_2_RUN_Draw=0;
		//	dynamic_X_Y_huoqu();
	
	}
	EA=1;
}



uint maichong_off=0;	//����off���� ����uv���ڹر�״̬ ���߼��������ϴεĻ�
void zhexian_Draw()
{
//	if(Off_On_pwm_flag==0)		//��PWM��Ӱ�� 1ʱ����Ϣ״̬ 0Ϊ����״̬
//	{
//		L_mm_TO_P_mw(L_int);
//	}
//	else if(MODE!=1) change_P_mW=-1;
	EA=0;	
	dynamic_draw();
	if(MODE==1)S_D_Surfaces_All();	//ֱ�������
	
	if(Draw_W_Flag==1)	//��ʼ����Ļ�ϻ���
	{
	
		
		if(First_Draw_Flag==0)		//��¼��һ�γ�ʼֵ�Ĳο���(ִֻ��һ��)
		{

			if(MODE==2)Page_Lasttime=Page_zhiliao;
			else {Page_Lasttime=Page_s[1];}		//����ʱ���¼��ǰҳ�� �Զ�����������ҳ���ж�
			
			T_doing=0;
			
			if(MODE==1 || MODE==4) {Lock_current_page();}		//���� ��ֹ�һ�ҳ��

		//	T_All=T_First;					//�Զ������� �²ſ���ǰ ��ʱ�����	
			
			X_zuobiao=tubiao_zuoshang_x;
			
			chart_num=0;
			First_Draw_Flag=1;
			P_mW_First=change_P_mW_First;						//��ȡ��ʼֵ�ľ�̬���� ���ٱ仯
			
			
			/********************************ִֻ��һ��*********************************/			
				if(MODE_Static==1)
				{
							//	W_mJ_zong=P_mW_First*T_First;				//��ȡ��ʼֵ������ MODE1�������ǹ̶��������ž���仯���仯				
						S_Write=(float)((T_First-1)/tubiao_chang*2*1000.0f);
	
				}
				else if(MODE_Static==2)
				{
					//W_mJ_zong=W_FINISH;					//��ó�ʼֵ������ MODE2�����ǹ̶�ֵ
						if(dynamic_static_flag)Draw_Rest_2();
						S_Write=(float)((T_First-1)/tubiao_chang*2*1000.0f);
			
				}
				else if(MODE==3)
				{
						
						S_Write=(float)((T_First-1)/tubiao_chang*2*1000.0f);
				}
				else if(MODE==4)
				{
					S_Write=(float)((T_First-1)/tubiao_chang*2*1000.0f);
				}
				
				
				if(change_P_mW>=0)
			{
				Y_zuobiao	=	tubiao_0zuobiao_y-(change_P_mW/2.5*(tubiao_0zuobiao_y-tubiao_zuoshang_y));
			}
			else 
				{
					Y_zuobiao	=	tubiao_0zuobiao_y+((fabs(change_P_mW)/1*(tubiao_zuoshang_y+tubiao_kuan-tubiao_0zuobiao_y)));	
				}
				
				Y_zuobiao_static=Y_zuobiao;
				chart_num=1;
				sys_write_chart_zhexian_Two(tubiao_0zuobiao_x,tubiao_0zuobiao_y,tubiao_0zuobiao_x,tubiao_0zuobiao_y);
				sys_write_chart_zhexian_Two_Cankao(tubiao_zuoshang_x,Y_zuobiao,tubiao_zuoshang_x+tubiao_chang-2,Y_zuobiao);
				
				X_zuobiao=tubiao_zuoshang_x+5;
				Draw_1ms=0;
		}
			/*************************************************************************************/
			if((UV_St==0&&Off_On_pwm_flag==0)||(Off_On_pwm_flag&&UV_St))
	{
		X_Y_huoqu();
		W_mJ_doing=(W_mJ_XIANSHI);
	}
		//sys_write_chart_zhexian_Two_Cankao(tubiao_zuoshang_x,Y_zuobiao_static,tubiao_zuoshang_x+tubiao_chang-2,Y_zuobiao_static);
	}
	EA=1;

}


void Draw_Reset()
{

	u8 Reset[8]={0};
	/*********************************����ǰģʽ������*********************************/
	First_Draw_Flag=0;
	EA=0;
	Draw_W_Flag=0;		//ֹͣ����
	//chart_num=0;
	X_zuobiao=tubiao_zuoshang_x;
	/******************************************************************************************/
	second_firstdraw_flag=0;	//�ڶ��λ�ͼ�ĳ�ʼ����־λ
	T_doing=0;
	WriteDGUS(CHART_ADDR,Reset,sizeof(Reset));
	WriteDGUS(CHART_ADDR_cankao,Reset,sizeof(Reset));
	WriteDGUS(CHART_ADDR_dynamic,Reset,sizeof(Reset));
	WriteDGUS(CHART_ADDR_2,Reset,sizeof(Reset));
}

void Draw_Rest_2()
{u8 Reset[8]={0};
		WriteDGUS(CHART_ADDR_dynamic,Reset,sizeof(Reset));
	WriteDGUS(CHART_ADDR_2,Reset,sizeof(Reset));
}


/*********************************************************************************
����ģʽ���������ֱ��	���������ģʽ���ķ�֮һ��ֱ��������ģʽ�Ķ���֮һ
����ģʽ���������ֱ��	
���ܽ�ģʽ�������
********************************************************************************/
float Tanx=0.4557;
float pai=3.1415;
float Tan_zhongxin=0.04366094;
float zhongxin_35mm=0.4008159;	//=0.0175m/tan2.5
	float L_m=0.00;				//���� ��λ:M
	float Square=0.00;			//��� ��λ��M^2
	float D=0.00;					//ֱ�� ��λ��M
	float Volume=0.00;					//���ܽ���� ��λ��M^3
void S_D_Surfaces_All()
{

	L_m	=	 (float)(L_int/1000.000)+zhongxin_35mm;
	if(W_S[0]==2)//zx
	{
	D	=	Tan_zhongxin*2*L_m;
	Square	=	(float)(pow(L_m*Tan_zhongxin,2)*pai);	
	}
	else 
	{
	D	=	Tanx*2*L_m;
	Square	=	(float)(pow(L_m*Tanx,2)*pai);	
	}
	Volume	=	Square*L_m/3.00;
}

/********************************************��ȡ��ǰҳ�沢����*************************************/

static struct{
	u16 Head;
	u8 page_0;//�̶�Ϊ0x00
	u8 page_1;
	
		u8 touch_id;
		u8 touch_daima;

		u16 mode;

}PageLock;
uint Current_Lock=0;		//0��ʾδ����
	u16 mode_close = 0x0000;
	u16 mode_open = 0x0001;

	u16 ControlPageLock_Adress = 0xb0;
	u16 ReadCurrentPage_Adress	=	0x14;

	u8 PageSliding = 0x0b;
	u8 Returnkey = 0x05;
	u8 zengliang=0x03;
	u8 Current_page[2];
void Lock_current_page()
{
	EA=0;
		PageLock.Head=0x5aa5;
		PageLock.page_0=0x00;

	
	if(MODE==1)				
	{
				if(W_S[0]==1)//���ܽ�
		{
		WriteDGUS(0x5105,(uint8_t *)&(icon_exit),sizeof(icon_exit));
		Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
		}
		else if(W_S[0]==2)//zx
		{
		WriteDGUS(0x5106,(uint8_t *)&(icon_exit),sizeof(icon_exit));
		Lock_current_page_zengliang(0x04,0x02,0x05,0x02);
		}
				else if(W_S[0]==3)//qy
		{
					WriteDGUS(0x5107,(uint8_t *)&(icon_exit),sizeof(icon_exit));
		Lock_current_page_zengliang(0x04,0x02,0x05,0x02);

		}
	}
	
	ReadDGUS(ReadCurrentPage_Adress,(uint8_t *)&Current_page,sizeof(Current_page));	//����ֵ 00 06
	PageLock.page_1=Current_page[1];
	PageLock.touch_id=0x00;			//������ҳ����Ϊ 0λ��
	PageLock.touch_daima=PageSliding;
	PageLock.mode=mode_close;

	WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
	while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&Current_page,sizeof(Current_page));
		if(Current_page[0]==Current_page[1])
		{break;}
	}
	
	PageLock.touch_id=0x01;		//���ؼ� ����Ϊ1λ��
	PageLock.touch_daima=Returnkey;
	WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
		while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&Current_page,sizeof(Current_page));
		if(Current_page[0]==Current_page[1])
		{break;}
	}
	Current_Lock=1;
	EA=1;
}

void Unlock_current_page()
{
	u8 page_status[2];
	EA=0;
	if(Current_Lock==1)
	{
		
			if(MODE==1)				
	{
		if(W_S[0]==1)//���ܽ�
		{
		WriteDGUS(0x5105,(uint8_t *)&(icon_attend),sizeof(icon_attend));
		Unlock_current_page_zengliang(0x04,0x02,0x05,0x02);
		}
		else if(W_S[0]==2)//zx
		{
					WriteDGUS(0x5106,(uint8_t *)&(icon_attend),sizeof(icon_attend));
		Unlock_current_page_zengliang(0x04,0x02,0x05,0x02);

		}
				else if(W_S[0]==3)//qy
		{
		WriteDGUS(0x5107,(uint8_t *)&(icon_attend),sizeof(icon_attend));
		Unlock_current_page_zengliang(0x04,0x02,0x05,0x02);
		}
	}
	
			ReadDGUS(ReadCurrentPage_Adress,(uint8_t *)&Current_page,sizeof(Current_page));	//����ֵ 00 06
			PageLock.page_1=Current_page[1];
			PageLock.mode=mode_open;
			PageLock.touch_daima=Returnkey;	
			PageLock.touch_id=0x01;	
			WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
	while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&page_status,sizeof(page_status));
		if(page_status[0]==page_status[1])
		{break;}
	}
			PageLock.touch_daima=PageSliding;
			PageLock.touch_id=0x00;	
		
			WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
		while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&page_status,sizeof(page_status));
		if(page_status[0]==page_status[1])
		{break;}
	}
			Current_Lock=0;
	}
	EA=1;
}
void Lock_current_page_zengliang(u8 id_1,u8 daima_1,u8 id_2,u8 daima_2)
{
	u8 Current_page[2];
	EA=0;
		PageLock.Head=0x5aa5;
		PageLock.page_0=0x00;

	ReadDGUS(ReadCurrentPage_Adress,(uint8_t *)&Current_page,sizeof(Current_page));	//����ֵ 00 06
	PageLock.page_1=Current_page[1];
	PageLock.touch_id=id_1;			//������ҳ����Ϊ 0λ��
	PageLock.touch_daima=daima_1;
	PageLock.mode=mode_close;

	WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
	while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&Current_page,sizeof(Current_page));
		if(Current_page[0]==Current_page[1])
		{break;}
	}
	
	//if(id_2=0xff)break;
	
	PageLock.touch_id=id_2;		//���ؼ� ����Ϊ1λ��
	PageLock.touch_daima=daima_2;
	WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
		while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&Current_page,sizeof(Current_page));
		if(Current_page[0]==Current_page[1])
		{break;}
	}
	EA=1;
	//Current_Lock=1;
}
void Unlock_current_page_zengliang(u8 id_1,u8 daima_1,u8 id_2,u8 daima_2)
{
	u8 Current_page[2];
	EA=0;
	ReadDGUS(ReadCurrentPage_Adress,(uint8_t *)&Current_page,sizeof(Current_page));	
		PageLock.page_1=Current_page[1];
			PageLock.touch_id=id_2;		//���ؼ� ����Ϊ1λ��
	PageLock.touch_daima=daima_2;
			PageLock.mode=mode_open;
			WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
	while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&Current_page,sizeof(Current_page));
		if(Current_page[0]==Current_page[1])
		{break;}
	}
			PageLock.touch_daima=daima_1;
			PageLock.touch_id=id_1;	
		
			WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
	
	//	if(id_2=0xff)break;
		while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&Current_page,sizeof(Current_page));
		if(Current_page[0]==Current_page[1])
		{break;}
	}
EA=1;
	
}