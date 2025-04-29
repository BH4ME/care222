
#include  "i2c.h"
//#include "sys.h"
#include "dgus.h"
//#include "sys_params.h"

uint16_t  SysTick_RTC=0;
//bit RTC_Flog=0;



uint8_t  Select_rtc=SELECT_RTC_TYPE; //选择RTC类型
#if SELECT_RTC_TYPE		
//===========================
void Delayus() {uint8_t i;for(i=0;i<80;i++);}  //50
//===========================
void SDA_IN()  {P3MDOUT=P3MDOUT&0xF7;}
//===========================
void SDA_OUT() {P3MDOUT=P3MDOUT|0x08;}
//===========================
/**********************设置界面写入rtc检测*************************
当提交键按下后运行此函数，先将年月日时分秒写入到9d地址，然后在将9c写入5aa5
配合rx8130
********************************************************************/
void RTC_SET_IN()
{
	uint8_t RTC_H[4];	//2023 02 18  年月日
								//0	 1	2	 3
	uint8_t RTC_L[4];	//00	12 45 23	时分秒
								//0		1	 2	3
	
 uint8_t RTC_6[6]={0};
	static uint8_t RTC_9C[2]={0x5a,0xa5};
	 uint32_t RTC_H_HEX;
	static uint32_t RTC_L_HEX;
	//long i;
// u64 RTC_8;
	ReadDGUS(0X1300,(uint8_t *)&RTC_H,sizeof(RTC_H));
	ReadDGUS(0X1305,(uint8_t *)&RTC_L,sizeof(RTC_L));
	

//RTC[2]=0x12;
//RTC[3]=0x66;
//RTC[4]=0x37;
//RTC[5]=0x06;
//RTC[6]=0x4f;
//RTC[7]=0x2b;
//RTC_H[0]=0x01;
//RTC_H[1]=0x34;
//RTC_H[2]=0xb0;
//RTC_H[3]=0x4b;
//		else RTC[4]=1;
//	}
//	else RTC[3]=1;
	//RTC_8=(RTC[0]<<56 |	RTC[1]<<48	|	RTC[2]<<40|	RTC[3]<<32	|	RTC[4]<<24	|	RTC[5]<<16	|RTC[6]<<8	|RTC[7]);
	//for(i=0;i<=5;i++)		RTC_H_HEX=(uint32_t)(0x0134b04b);
	//{
		RTC_H_HEX=(uint32_t)(RTC_H[0])<<24	|	(uint32_t)(RTC_H[1])<<16	| (uint32_t)(RTC_H[2])<<8	| (uint32_t)(RTC_H[3]);
		RTC_L_HEX=(uint32_t)(RTC_L[0])<<24	|	(uint32_t)(RTC_L[1])<<16	| (uint32_t)(RTC_L[2])<<8	| (uint32_t)(RTC_L[3]);
		//RTC_H_HEX=(uint32_t)(0x0134b04b);
	//	RTC_L_HEX=((RTC_L[0]<<18	|	RTC_L[1]<<10	|RTC_L[2]<<8	|RTC_L[3]));
		//RTC_6[5]=((uint32_t)(0x126637064f2b))%100;
		RTC_6[5]=RTC_L_HEX%100;
		RTC_6[4]=(RTC_L_HEX/100)%100;
		RTC_6[3]=(RTC_L_HEX/10000)%100;
		RTC_6[2]=RTC_H_HEX%100;
		RTC_6[1]=(RTC_H_HEX/100)%100;
		RTC_6[0]=(RTC_H_HEX/10000)%100;
	//}
	/***********************时间校准*********************/
	if(RTC_6[1]>0&&RTC_6[1]<=12)	//月
	{
		if(RTC_6[2]<=31&&RTC_6[2]>0)	//日
		{
			if(RTC_6[3]<=23&&RTC_6[3]>=0)
			{
				if(RTC_6[4]<=59&&RTC_6[4]>=0)
				{
					if(RTC_6[5]<=59&&RTC_6[5]>=0)
					{}
						else RTC_6[5]=0;
				}else RTC_6[4]=0;
			}else RTC_6[3]=0;
		}
			else RTC_6[2]=1;
	}
	else RTC_6[1]=1;
	
			WriteDGUS(0x9d, (uint8_t *)&RTC_6,sizeof(RTC_6));	
			WriteDGUS(0x9c, (uint8_t *)&RTC_9C,sizeof(RTC_9C));	
	
}
void Rx8130_Start()
{	
						SDA_OUT();
						RTC_SDA=1;		//开始信号 scl为高时，sda由高到低
						RTC_SCL=1;
						Delayus();  //15
						RTC_SDA=0;
						Delayus();  //15
						RTC_SCL=0;
						Delayus();  //15
}
//================================
void Rx8130_Stop()
{	
						SDA_OUT();
						RTC_SDA=0;	//end信号 scl为高时，sda由0到1
						RTC_SCL=1;
						Delayus();//15
						RTC_SDA=1;
						Delayus();//15
						SDA_IN();
}
//================================
void Rx8130_Mack()
{	
						SDA_OUT();
						RTC_SDA=0;
						Delayus();
						RTC_SCL=1;
						Delayus();
						RTC_SCL=0;
						Delayus();
}
//=================================
void Rx8130_Mnak()
{	
						SDA_OUT();
						RTC_SDA=1;
						Delayus();
						RTC_SCL=1;
						Delayus();
						RTC_SCL=0;
						Delayus();
}
//=================================
void Rx8130_Cack()
{	
						uint8_t i;
						SDA_IN();
						RTC_SDA=1;
						Delayus();
						RTC_SCL=1;
						Delayus();
						for(i=0;i<50;i++)
						{   
										if(!RTC_SDA) break;
										Delayus();
						}
						RTC_SCL=0;
						Delayus();
						SDA_OUT();
}
//=========================================
void Rx8130_Writ_Byte(uint8_t dat)
{	
						uint8_t i;
						SDA_OUT();
						for(i=0;i<8;i++)
						{	
									    if(dat&0x80)   RTC_SDA=1;
										else           RTC_SDA=0;
										dat=(dat<<1);
										Delayus();
										RTC_SCL=1;
										Delayus();
										RTC_SCL=0;
										Delayus();
						}
						Rx8130_Cack();
}
//====================================================
uint8_t Rx8130_Read_Byte()
{	
						uint8_t i;
						uint8_t Dat;
						SDA_IN();
						for(i=0;i<8;i++)
						{	
										Delayus();
										RTC_SCL=1;
										Delayus();
										Dat=(Dat<<1);
										if(RTC_SDA)  Dat=Dat|0x01;
										else         Dat=Dat&0xFE;
										RTC_SCL=0;
										Delayus();
						}
						return(Dat);
}
//====================================================
uint8_t RTC_Get_Week(uint8_t years,uint8_t month,uint8_t day)
{	
	uint8_t xdata weak;
	uint16_t xdata year;
	year= years+2000;
  if(month==1||month==2)
	{
		month += 12, year--;
	}
  if((year<1752)||(year==1752&&month<9)||(year==1752&&month==9&&day<3))
	{
		weak = (day + 2*month + 3*(month+1)/5 + year + year/4 + 6)%7;
	}
  else
	{
		weak = (day +1 + 2*month + 3*(month+1)/5 + year + year/4 -year/100 + year/400)%7;
	}
   return weak;
}
//把RTC读取并处理，写到DGUS对应的变量空间，主程序中每0.5秒调用一次
void Read_Time()
{	
						uint8_t i,N,M; 
	          uint8_t Rtcdata[]={0,0,0,0,0,0,0,0};
						Rx8130_Start();
						Rx8130_Writ_Byte(0x64);
						Rx8130_Writ_Byte(0x10);
						Rx8130_Stop();
						Rx8130_Start();
						Rx8130_Writ_Byte(0x65);
						for(i=6;i>0;i--)
						{	
													Rtcdata[i]=Rx8130_Read_Byte();
													Rx8130_Mack();
						}
						Rtcdata[0]=Rx8130_Read_Byte(); Rx8130_Mnak();
						Rx8130_Stop();
						for(i=0;i<3;i++)	//年月日转换成HEX
						{	
							            N=Rtcdata[i]/16;
													M=Rtcdata[i]%16;
													Rtcdata[i]=N*10+M;
						}
						for(i=4;i<7;i++)	//时分秒转换成HEX
						{	
													N=Rtcdata[i]/16;
													M=Rtcdata[i]%16;
													Rtcdata[i]=N*10+M;
						}
						Rtcdata[3]=RTC_Get_Week(Rtcdata[0],Rtcdata[1],Rtcdata[2]);//周						
						WriteDGUS(0X0010,Rtcdata,2);
						WriteDGUS(0X0011,Rtcdata+2,2);
						WriteDGUS(0X0012,Rtcdata+4,2);
						WriteDGUS(0X0013,Rtcdata+6,2);
}
//====================================================================
void Init_Rtc()
{	
					   uint8_t i;
				  	   Rx8130_Start();
					   Rx8130_Writ_Byte(0x64);
					   Rx8130_Writ_Byte(0x1d);
					   Rx8130_Stop();
					   Rx8130_Start();
					   Rx8130_Writ_Byte(0x65);
					   i=Rx8130_Read_Byte();
					   Rx8130_Mack();
					   Rx8130_Read_Byte();
					   Rx8130_Mnak();
					   Rx8130_Stop();
					   if((i&0x02)==0x02)
				       {	
									//重新配置时间
									Rx8130_Start();		//30=00
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x30);
									Rx8130_Writ_Byte(0x00);
									Rx8130_Stop();
									Rx8130_Start();		//1C-1F=48 00 40 10
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x1C);
									Rx8130_Writ_Byte(0x48);
									Rx8130_Writ_Byte(0x00);
									Rx8130_Writ_Byte(0x40);
									Rx8130_Writ_Byte(0x10);
									Rx8130_Stop();
									Rx8130_Start();		//10-16=RTC设置值 BCD格式
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x10);
									Rx8130_Writ_Byte(0x00);	//秒
									Rx8130_Writ_Byte(0x00);	//分
									Rx8130_Writ_Byte(0x00);	//时
									Rx8130_Writ_Byte(0x01);	//星期
									Rx8130_Writ_Byte(0x01);	//日
									Rx8130_Writ_Byte(0x01);	//月
									Rx8130_Writ_Byte(0x17);	//年
									Rx8130_Stop();
									Rx8130_Start();		//1E-1F 00 10
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x1E);
									Rx8130_Writ_Byte(0x00);	
									Rx8130_Writ_Byte(0x10);			
									Rx8130_Stop();
				       }
}
//=========================================
void RTC_Set_Time()
{					  
  
					//	uint16_t  Va=ReadDGUS(0x009C); //0x5AA5
						uint16_t  Va;
						uint8_t  zero[2]={0,0};
						ReadDGUS(0x009C,(uint8_t *)&Va,2);
						if(Va==0x5AA5)
						{			
									uint8_t N=0,S=0;
									bit  Flog=1;
									uint8_t TimeS[]={0,0,0,0,0,0,0};
									uint16_t  V1;
									ReadDGUS(0x009D,(uint8_t *)&V1,2); //年，月
									N=(uint8_t)(V1>>8);TimeS[0]=((N/10)*16)+(N%10);
									N=(uint8_t)(V1);   TimeS[1]=((N/10)*16)+(N%10);
									ReadDGUS(0x009E,(uint8_t *)&V1,2); //日，时
									N=(uint8_t)(V1>>8);TimeS[2]=((N/10)*16)+(N%10);
									N=(uint8_t)(V1);   TimeS[4]=((N/10)*16)+(N%10);
									ReadDGUS(0x009F,(uint8_t *)&V1,2); //分，秒
									N=(uint8_t)(V1>>8);TimeS[5]=((N/10)*16)+(N%10);
									N=(uint8_t)(V1);   TimeS[6]=((N/10)*16)+(N%10);
//						            V1=ReadDGUS(0x00A0); //秒，x  
//						            N=(uint8_t)(V1>>8);TimeS[6]=((N/10)*16)+(N%10);
									WriteDGUS(0x009c,zero,1);	
									while(Flog)
									{	
										        Rx8130_Start();		//30=00
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x30);
												Rx8130_Writ_Byte(0x00);
												Rx8130_Stop();
												Rx8130_Start();		//1C-1F=48 00 40 10
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x1C);
												Rx8130_Writ_Byte(0x48);
												Rx8130_Writ_Byte(0x00);
												Rx8130_Writ_Byte(0x40);
												Rx8130_Writ_Byte(0x10);
												Rx8130_Stop();
												Rx8130_Start();		//10-16=RTC设置值 BCD格式
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x10);
												Rx8130_Writ_Byte(TimeS[6]);	//秒
												Rx8130_Writ_Byte(TimeS[5]);	//分
												Rx8130_Writ_Byte(TimeS[4]);	//时
												Rx8130_Writ_Byte(TimeS[3]);	//星期
												Rx8130_Writ_Byte(TimeS[2]);	//日
												Rx8130_Writ_Byte(TimeS[1]);	//月
												Rx8130_Writ_Byte(TimeS[0]);	//年
												Rx8130_Stop();
												Rx8130_Start();		//1E-1F 00 10
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x1E);
												Rx8130_Writ_Byte(0x00);	
												Rx8130_Writ_Byte(0x10);			
												Rx8130_Stop(); 
												Flog=0;
								   }
						}
}
//========================================================================================================================================
//**********************SD2058接口程序，SDA 10K上拉到3.3V**************
	



#endif

	   
//========================================================================================================================================

//		bit s=0;
//	uint8_t flag_rtc_init[8]={0};
//		uint8_t flag_rtc_in[6]={0};
	
		
		
void Clock()
{
        // if(RTC_Flog==0){ RTC_Flog=1;  Init_Rtc();}
				       if(SysTick_RTC >= 500) 
				       {
										Read_Time();								//更新硬件RTC时间
										SysTick_RTC = 0;	 
				       }
								RTC_Set_Time();		 
}