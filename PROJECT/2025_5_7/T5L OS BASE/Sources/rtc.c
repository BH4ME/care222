#include "rtc.h"
#include "sys.h"
#include "uart.h"
#include "timer.h"
u16 time[4];
u16 data SysTick_RTC=0;
bit RTC_Flog=0;
//====================================
u16 Read_Dgus(u16 Dgus_Addr)
{	   	   	 u16 R_Dgus=0;
			 EA = 0;ADR_H = 0x00;
			 ADR_M = (u8)((Dgus_Addr/2)>>8);
			 ADR_L = (u8)(Dgus_Addr/2);
			 RAMMODE = 0xAF; while(!APP_ACK);	//������
			 APP_EN = 1;     while(APP_EN);
			 if(Dgus_Addr%2)	R_Dgus = (DATA1<<8)+DATA0; 
			 else            R_Dgus=(DATA3<<8)+DATA2;
			 RAMMODE = 0; EA = 1;	return   R_Dgus;
}
//====================================
void Write_Dgus(u16 Dgus_Addr,u16 Val)
{           EA = 0;
						ADR_H = 0x00;
						ADR_M = (u8)((Dgus_Addr/2)>>8);
						ADR_L = (u8)(Dgus_Addr/2);
						RAMMODE = 0xAF;		//������
						while(!APP_ACK);
						APP_EN = 1;
						while(APP_EN);
						RAMMODE = 0;
							ADR_M = (u8)((Dgus_Addr/2)>>8);
						ADR_L = (u8)(Dgus_Addr/2);
						RAMMODE = 0x8F;		//д����
							while(!APP_ACK);
						if(Dgus_Addr%2){DATA1=(u8)(Val>>8); DATA0=(u8)(Val); }
						else { DATA3 =(u8)(Val>>8);	DATA2 =(u8)(Val);}
							APP_EN = 1;
						while(APP_EN);
						RAMMODE = 0;
						EA = 1;
}
//===========================
void Delayus() {	u8 i;for(i=0;i<50;i++);}
//===========================
void SDA_IN()  {	P3MDOUT=P3MDOUT&0xF7;}
//===========================
void SDA_OUT() {  P3MDOUT=P3MDOUT|0x08;}
//==========================
void Rx8130_Start()
{	
						SDA_OUT();
						RTC_SDA=1;
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
						RTC_SDA=0;
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
						u8 i;
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
void Rx8130_Writ_Byte(u8 dat)
{	
						u8 i;
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
u8 Rx8130_Read_Byte()
{	
						u8 i;
						u8 Dat;
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
//��RTC��ȡ������д��DGUS��Ӧ�ı����ռ䣬��������ÿ0.5�����һ��
void Read_Time()
{	
						u8 i,N,M; 
	                    u8 Rtcdata[]={0,0,0,0,0,0,0,0};
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
						for(i=0;i<3;i++)	//������ת����HEX
						{	
							                        N=Rtcdata[i]/16;
													M=Rtcdata[i]%16;
													Rtcdata[i]=N*10+M;
						}
						for(i=4;i<7;i++)	//ʱ����ת����HEX
						{	
													N=Rtcdata[i]/16;
													M=Rtcdata[i]%16;
													Rtcdata[i]=N*10+M;
						}
						Write_Dgus(0X0010,(Rtcdata[0]<<8)+Rtcdata[1]);
						Write_Dgus(0X0011,(Rtcdata[2]<<8)+Rtcdata[3]);
						Write_Dgus(0X0012,(Rtcdata[4]<<8)+Rtcdata[5]);
						Write_Dgus(0X0013,(Rtcdata[6]<<8)+Rtcdata[7]);
}
//====================================================================
void Init_Rtc()
{	
					u8 i;
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
									//��������ʱ��
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
									Rx8130_Start();		//10-16=RTC����ֵ BCD��ʽ
									Rx8130_Writ_Byte(0x64);
									Rx8130_Writ_Byte(0x10);
									Rx8130_Writ_Byte(0x00);	//��
									Rx8130_Writ_Byte(0x00);	//��
									Rx8130_Writ_Byte(0x00);	//ʱ
									Rx8130_Writ_Byte(0x01);	//����
									Rx8130_Writ_Byte(0x01);	//��
									Rx8130_Writ_Byte(0x01);	//��
									Rx8130_Writ_Byte(0x17);	//��
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
						u16  Va=Read_Dgus(0x009C); //0x5AA5
						if(Va==0x5AA5)
						{			u8 N=0,S=0;bit  Flog=1;
									u8 TimeS[]={0,0,0,0,0,0,0};
									u16  V1=Read_Dgus(0x009D); //�꣬��
									N=(u8)(V1>>8);TimeS[0]=((N/10)*16)+(N%10);
									N=(u8)(V1);   TimeS[1]=((N/10)*16)+(N%10);
									V1=Read_Dgus(0x009E); //�գ�����
									N=(u8)(V1>>8);TimeS[2]=((N/10)*16)+(N%10);
									N=(u8)(V1);   TimeS[3]=((N/10)*16)+(N%10);
									V1=Read_Dgus(0x009F); //ʱ����
									N=(u8)(V1>>8);TimeS[4]=((N/10)*16)+(N%10);
									N=(u8)(V1);   TimeS[5]=((N/10)*16)+(N%10);
						            V1=Read_Dgus(0x00A0); //�룬x  
						            N=(u8)(V1>>8);TimeS[6]=((N/10)*16)+(N%10);
									Write_Dgus(0x009c,0);	
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
												Rx8130_Start();		//10-16=RTC����ֵ BCD��ʽ
												Rx8130_Writ_Byte(0x64);
												Rx8130_Writ_Byte(0x10);
												Rx8130_Writ_Byte(TimeS[6]);	//��
												Rx8130_Writ_Byte(TimeS[5]);	//��
												Rx8130_Writ_Byte(TimeS[4]);	//ʱ
												Rx8130_Writ_Byte(TimeS[3]);	//����
												Rx8130_Writ_Byte(TimeS[2]);	//��
												Rx8130_Writ_Byte(TimeS[1]);	//��
												Rx8130_Writ_Byte(TimeS[0]);	//��
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
//==================================================================== 

void        Clock()
{
                  	   if(RTC_Flog==0){ RTC_Flog=1;  Init_Rtc(); }
				       if(SysTick_RTC >= 500) //ԭ����500�����Ը�Ϊ50
				       {
										Read_Time();								//����Ӳ��RTCʱ��
										SysTick_RTC = 0;
				       }
					   RTC_Set_Time();
}
/*****************************************************************************
������*/

//void        main()
//{
//	            
//				 InitCPU();
//			     while(1)
//			     {
//			                Clock();
//											time[0] = Read_Dgus(0X10);
//											time[1] = Read_Dgus(0X11);
//											time[2] = Read_Dgus(0X12);
//											time[3] = Read_Dgus(0X13); //read time to time[]
//			     }
//}
////=========================================
//void T2_ISR_PC()    interrupt 5
//{
//                TF2=0;
//	            SysTick_RTC++;
//}

void RTC_Start(void)
{
	
			                Clock();
											time[0] = Read_Dgus(0X10);
											time[1] = Read_Dgus(0X11);
											time[2] = Read_Dgus(0X12);
											time[3] = Read_Dgus(0X13); //read time to time[]
}