#include "start_win.h"
#include "column_chart.h"
#include <stdio.h>
#include "uart4.h"
#include <math.h>
#include "handle.h"
//#define START_WIN_CHART_VP		0x4000		//基本图形控件的VP地址
//#define COLUMN_CHART_HEIGHT		295				//柱状图的高度


//bit is_anim = 1;
//bit style = 0;
//u16 color = 0xf800;
//uint16_t init_val[3];
//	uint16_t MAX;
//COLUMN_CHART chart;
//u16 start_win_get_val(s16 index);

//u16 x1_0=0x85;
//u16 y1_0=0x364;

uint Draw_W_Flag=0;		//0不启动画线，1启动画线
uint First_Draw_Flag=0;		//只加载一次


	u16 Y_zuobiao;
	u16 X_zuobiao;
	
	float S_Write;		


u16 Adress_Now;

double P_mW_First=0.0;
double W_mJ_zong=0.0;
float W_mJ_doing=0.0;
double T_First=0.0; //初始时间不变的
uint T_doing=0.0; //已进行的时间
uint T_All=0;
u8 Page_Lasttime=0;
u8 Page_s[2]={0};
void X_Y_Calculate();
void Draw_juxing();
void Draw_juxing_zuobiao();
void S_D_Surfaces_All();
void Unlock_current_page_zengliang(u8 id_1,u8 daima_1,u8 id_2,u8 daima_2);
void Lock_current_page_zengliang(u8 id_1,u8 daima_1,u8 id_2,u8 daima_2);
u8 second_firstdraw_flag=0;	//第二次画图的flag
uint Y_zuobiao_static=0;
u16 chart_num =0;
void sys_write_chart_zhexian_Two(u16 x1,u16 y1,u16 x2,u16 y2)		//输入2个点
{
	#define CHART_ADDR		0xb000	//动态曲线功能所对应的系统变量接口地址
	//#define CHART_NUM			0xff				//线段数数据,我们只写入chart_id指定的曲线通道,即1个
	//#define POINT_NUM			1  			//一次性写入多少个数据点
	#define colour 0Xffff
	u8 chart_cmd[16] = {0x00,0x02};
	chart_cmd[2]=(u8)(chart_num>>8);
	chart_cmd[3]=(u8)(chart_num&0xff);
	chart_cmd[4] = (u8)(colour>>8);//曲线通道id
	chart_cmd[5] = (u8)(colour&0xff);//一次性写入多少个数据点
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
	//发送命令
	WriteDGUS(CHART_ADDR,chart_cmd,sizeof(chart_cmd));
}
void sys_write_chart_zhexian_Two_Cankao(u16 x1,u16 y1,u16 x2,u16 y2)		//输入2个点
{
	#define CHART_ADDR_cankao		0x5500	//动态曲线功能所对应的系统变量接口地址
	#define CHART_NUM_cankao			0xff				//线段数数据,我们只写入chart_id指定的曲线通道,即1个
	//#define POINT_NUM			1  			//一次性写入多少个数据点
	#define colour_cankao 0X8410
	u8 chart_cmd[16] = {0x00,0x02,0x0f,CHART_NUM_cankao};
	chart_cmd[4] = (u8)(colour_cankao>>8);//曲线通道id
	chart_cmd[5] = (u8)(colour_cankao&0xff);//一次性写入多少个数据点
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
	//发送命令
	WriteDGUS(CHART_ADDR_cankao,chart_cmd,sizeof(chart_cmd));
}
#define tubiao_chang 580	//图表的长度
#define tubiao_kuan 120	//图表的宽度
#define tubiao_zuoshang_x 10	//图表的左上x坐标
#define tubiao_zuoshang_y 346	//图表的左上y坐标
#define tubiao_0zuobiao_x 10 //0点坐标x
#define tubiao_0zuobiao_y 426 //0点坐标y
//高度6段 2.5 -1
//#define tubiao_chang 570	//图表的长度
//#define tubiao_kuan 112	//图表的宽度
//#define tubiao_zuoshang_x 17	//图表的左上x坐标
//#define tubiao_zuoshang_y 350	//图表的左上y坐标
//#define tubiao_0zuobiao_x 17 //0点坐标x
//#define tubiao_0zuobiao_y 430 //0点坐标y
void Write_New(u16 x1,u16 y1)//每次写3字
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
		/******************判断xy的坐标是否超出表格值**********************************/
		if(x1>=(tubiao_chang+tubiao_zuoshang_x))	//超出右侧长度值
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
		WriteDGUS(CHART_ADDR+0x1,chart,sizeof(chart));	//画多少条线
		WriteDGUS(Adress_Now,chart_cmd,sizeof(chart_cmd));
				
		Adress_Now=Adress_Now+0x2;chart_num++;
		EA=1;
}

	u16 X_zuobiao_second;
//u8 second_flag=0;		//没用的到第二条曲线的时候为0 用到第二条曲线的时候为1
/*******************************写入第二条曲线 当时间变长的时候**************************/
u16 chart_num_2 =0;
u16 Adress_Now_2;
void sys_write_chart_zhexian_Two_2(u16 x1,u16 y1,u16 x2,u16 y2)		//输入2个点
{
	#define CHART_ADDR_2		0x8000	//动态曲线功能所对应的系统变量接口地址
	//#define CHART_NUM			0xff				//线段数数据,我们只写入chart_id指定的曲线通道,即1个
	//#define POINT_NUM			1  			//一次性写入多少个数据点
	#define colour_2 0Xffe0
	
	u8 chart_cmd[16] = {0x00,0x02};
	EA=0;
	chart_num_2=1;
	chart_cmd[2]=(u8)(chart_num_2>>8);
	chart_cmd[3]=(u8)(chart_num_2&0xff);
	chart_cmd[4] = (u8)(colour_2>>8);//曲线通道id
	chart_cmd[5] = (u8)(colour_2&0xff);//一次性写入多少个数据点
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
	//发送命令
	WriteDGUS(CHART_ADDR_2,chart_cmd,sizeof(chart_cmd));
	EA=1;
}
void Write_New_2(u16 x1,u16 y1)//每次写3字
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
		/******************判断xy的坐标是否超出表格值**********************************/
		if(x1>=(tubiao_chang+tubiao_zuoshang_x))	//超出右侧长度值
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
		WriteDGUS(CHART_ADDR_2+0x1,chart,sizeof(chart));	//画多少条线
		WriteDGUS(Adress_Now_2,chart_cmd,sizeof(chart_cmd));
				
		Adress_Now_2=Adress_Now_2+0x2;chart_num_2++;
		EA=1;
}

/**********************************************************************************************/
void X_Y_huoqu()
{
	EA=0;
	/*******************************计算Y坐标结果值为像素**********************************/
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
	/****************************************计算X坐标结果值为像素*************************************/			

					if(Draw_1ms>=S_Write)
					{	
						
						Draw_1ms=0;
						
						if(X_zuobiao<=tubiao_zuoshang_x+tubiao_chang-0x2)	//图像 1
						{
							X_zuobiao=2+X_zuobiao;					
							Write_New(X_zuobiao,Y_zuobiao);
						}
						else	if(dynamic_static_flag	&&	T_All!=T_dynamic)	//图像	2
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


/***************治疗模式下时间处于动态**最后汇总曲线****************/
u16 Adress_Now_dynamic;

u16 dynamic_X_zuobiao;
u16 dynamic_Y_zuobiao;
void dynamic_draw_chart_Init()
{
	#define Dynamic_chart_adress_vp 0x1907
	u8 buff_dynamic[2]={0x00,0x02};
	//第三条曲线最后归总的曲线从1*1，变成2*2 写入VP指针
		WriteDGUS(Dynamic_chart_adress_vp,buff_dynamic,sizeof(buff_dynamic));
	
}
void dynamic_first_Init(u16 x1,u16 y1,u16 x2,u16 y2)
{
		#define CHART_ADDR_dynamic			0xd000	//动态曲线功能所对应的系统变量接口地址
	#define CHART_NUM_dynamic			0xff				//线段数数据,我们只写入chart_id指定的曲线通道,即1个
	//#define POINT_NUM			1  			//一次性写入多少个数据点
	#define colour_dynamci 0Xf800
	u8 chart_cmd[16] = {0x00,0x02,0x0f,CHART_NUM_dynamic};
	chart_cmd[4] = (u8)(colour_dynamci>>8);//曲线通道id
	chart_cmd[5] = (u8)(colour_dynamci&0xff);//一次性写入多少个数据点
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
void dynamic_Write_New(u16 x1,u8 y1_h,u8 y1_l)//每次写3字
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
		/******************判断xy的坐标是否超出表格值**********************************/
		if(x1>=(tubiao_chang+tubiao_zuoshang_x))	//超出右侧长度值
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
void dynamic_X_Y_huoqu()		//对第一条曲线进行拉伸
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
	
	
	
					if(T_First==T_dynamic)	//距离没有变化
				{  
												for(i=0;i<=289;i++)	//2*2曲线 长度是580 一共要写入192个点每个点间距1个像素
					{
							adress_now=adress_now+0x2;
							ReadDGUS(adress_now,dynamic_y,sizeof(dynamic_y));	
							dynamic_X_zuobiao=dynamic_X_zuobiao+0X2;
							dynamic_Write_New(dynamic_X_zuobiao,dynamic_y[0],dynamic_y[1]);
					}
				}
				
				else if(beishu!=1)
				{
							for(i=0;i<=289;i++)	//2*2曲线 长度是580 一共要写入192个点每个点间距1个像素
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
				
				
				
				
				
//	if(beishu!=0)		//曲线一的点小于192个点
//	{
//					for(i=0;i<=289;i++)	//2*2曲线 长度是580 一共要写入192个点每个点间距1个像素
//			{
//				
//				//先判断第一条曲线没写完就结束的情况下

//				if(T_All==T_dynamic)	//距离没有变化
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
//	else	//剩下的点数大于已经写入的 总共290点 已经写入90 还需要200
//	{
//		
//		beishu=draw_num/chart_num;
//		j=beishu;
//							for(i=0;i<=289;i++)	//2*2曲线 长度是580 一共要写入192个点每个点间距1个像素
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
void dynamic_X_Y_huoqu_2()	//对第一条和第二条曲线进行抽样
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
	if(num_throwaway!=0)		//曲线一的点小于192个点
	{
					for(i=0;i<=285;i++)	//2*2曲线 长度是580 一共要写入192个点每个点间距1个像素
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
		if(second_firstdraw_flag)	//用到第二条曲线 第三条曲线应该为第一条曲线和第二条曲线的抽样
		{
			dynamic_X_Y_huoqu_2();MODE_2_RUN_Draw=0;			dynamic_draw_chart_Init();
		}
		else
			{dynamic_X_Y_huoqu();			dynamic_draw_chart_Init();}		//只用到第一条曲线
		MODE_2_RUN_Draw=0;
		//	dynamic_X_Y_huoqu();
	
	}
	EA=1;
}



uint maichong_off=0;	//脉冲off开启 现在uv处于关闭状态 折线继续接着上次的画
void zhexian_Draw()
{
//	if(Off_On_pwm_flag==0)		//受PWM的影响 1时是休息状态 0为工作状态
//	{
//		L_mm_TO_P_mw(L_int);
//	}
//	else if(MODE!=1) change_P_mW=-1;
	EA=0;	
	dynamic_draw();
	if(MODE==1)S_D_Surfaces_All();	//直径和面积
	
	if(Draw_W_Flag==1)	//开始在屏幕上画线
	{
	
		
		if(First_Draw_Flag==0)		//记录第一次初始值的参考线(只执行一次)
		{

			if(MODE==2)Page_Lasttime=Page_zhiliao;
			else {Page_Lasttime=Page_s[1];}		//开的时候记录当前页面 自动结束后会继续页面判断
			
			T_doing=0;
			
			if(MODE==1 || MODE==4) {Lock_current_page();}		//上锁 禁止乱换页面

		//	T_All=T_First;					//自动结束后 下才开启前 总时间更新	
			
			X_zuobiao=tubiao_zuoshang_x;
			
			chart_num=0;
			First_Draw_Flag=1;
			P_mW_First=change_P_mW_First;						//获取初始值的静态功率 不再变化
			
			
			/********************************只执行一次*********************************/			
				if(MODE_Static==1)
				{
							//	W_mJ_zong=P_mW_First*T_First;				//获取初始值的能量 MODE1能量不是固定的是随着距离变化而变化				
						S_Write=(float)((T_First-1)/tubiao_chang*2*1000.0f);
	
				}
				else if(MODE_Static==2)
				{
					//W_mJ_zong=W_FINISH;					//获得初始值的能量 MODE2能量是固定值
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
	/*********************************画当前模式的曲线*********************************/
	First_Draw_Flag=0;
	EA=0;
	Draw_W_Flag=0;		//停止画线
	//chart_num=0;
	X_zuobiao=tubiao_zuoshang_x;
	/******************************************************************************************/
	second_firstdraw_flag=0;	//第二次画图的初始化标志位
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
中心模式：求面积和直径	面积是区域模式的四分之一，直径是区域模式的二分之一
区域模式：求面积和直径	
气溶胶模式：求体积
********************************************************************************/
float Tanx=0.4557;
float pai=3.1415;
float Tan_zhongxin=0.04366094;
float zhongxin_35mm=0.4008159;	//=0.0175m/tan2.5
	float L_m=0.00;				//距离 单位:M
	float Square=0.00;			//面积 单位：M^2
	float D=0.00;					//直径 单位：M
	float Volume=0.00;					//气溶胶体积 单位：M^3
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

/********************************************读取当前页面并锁定*************************************/

static struct{
	u16 Head;
	u8 page_0;//固定为0x00
	u8 page_1;
	
		u8 touch_id;
		u8 touch_daima;

		u16 mode;

}PageLock;
uint Current_Lock=0;		//0表示未上锁
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
				if(W_S[0]==1)//气溶胶
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
	
	ReadDGUS(ReadCurrentPage_Adress,(uint8_t *)&Current_page,sizeof(Current_page));	//返回值 00 06
	PageLock.page_1=Current_page[1];
	PageLock.touch_id=0x00;			//滑动翻页设置为 0位置
	PageLock.touch_daima=PageSliding;
	PageLock.mode=mode_close;

	WriteDGUS(ControlPageLock_Adress, (uint8_t *)&PageLock,sizeof(PageLock));	
	while(1)
	{
		ReadDGUS(ControlPageLock_Adress,(uint8_t *)&Current_page,sizeof(Current_page));
		if(Current_page[0]==Current_page[1])
		{break;}
	}
	
	PageLock.touch_id=0x01;		//返回键 设置为1位置
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
		if(W_S[0]==1)//气溶胶
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
	
			ReadDGUS(ReadCurrentPage_Adress,(uint8_t *)&Current_page,sizeof(Current_page));	//返回值 00 06
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

	ReadDGUS(ReadCurrentPage_Adress,(uint8_t *)&Current_page,sizeof(Current_page));	//返回值 00 06
	PageLock.page_1=Current_page[1];
	PageLock.touch_id=id_1;			//滑动翻页设置为 0位置
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
	
	PageLock.touch_id=id_2;		//返回键 设置为1位置
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
			PageLock.touch_id=id_2;		//返回键 设置为1位置
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