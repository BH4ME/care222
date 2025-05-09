#include "rtc_sys.h"


u16    xdata        SysTick_RTC=0;
bit                 RTC_Flog=0;

//=====================================
//void Sys_Cpu_Init()
//{
//		    IEN0       = 0x00;					   //IEN0.7=EA 中断总控制 .6=0，.5=ET2，.4=ES0，.3=ET1，.2=EX1，.1=ET0，.0=EX0   ES0 串口2 接受+发送中断
//			IEN1       = 0x00;					   //IEN1.7=0，.6=0，.5=串口5接受中断,.4=串口5发送中断,.3=串口4接受中断,.2=串口4发送中断,.1=can中断
//			IEN2       = 0x00;					   // .7-.1=0 , .0=ES1 串口3 接受+发送中断 
////========================================MODE			
//			DPC = 0x00;		                       //DPTR 是否+1 C51=0
//			CKCON      = 0x00;	                   //CPU运行=0, 1T模式
//		
//			IP0 = 0x00;IP1 = 0x00;        		   //中断优先级默认
//			RAMMODE = 0x00;
////========================================IO初始化 	        
//			P0MDOUT    =0x53;	           //p0.0 p0.1 强推 用于485方向控制 P0.4引出到串口2 P0.6引出到串口3
//			P2MDOUT    =0xC0; 
//	    P3MDOUT    =0x0C;					   //开启P3_2和P3_3的输出 P3_2=RTC_SCL P3_3=RTC_SDA
//			PORTDRV    = 0x01;					   //IO驱动能力设定 4mA=0,8mA=1,16mA=2,32mA=3
//			P2         = 0XC0;
//	    P3         = 0x00; 
//			WDT_OFF();        //关闭开门狗		
////========================================
//			TH2 = 0x00;
//			TL2 = 0x00;
//			T2CON = 0x70;
//			TRL2H = 0xBC;	//1ms的定时器
//			TRL2L = 0xCD;
//			IEN0 |= 0x20;	//启动定时器2
//			TR2 = 0x01;
//      ET2 = 1;        //T2定时器中断使能控制位
//			EA  = 1;

//}
/****************************************************************************/
//void Sys_Timer2_isr()	interrupt 5	  //1MS
//{
//	        TF2=0;//清除定时器2的中断标志位
//	        if(TimVal<255)TimVal++;
//			    SysTick_RTC++;
////	        if(T_O5>0)T_O5--;
////	        if(T_O4>0)T_O4--;
////	        if(T_O3>0)T_O3--;
////	        if(T_O2>0)T_O2--;
//	        if(Count_num1>0) Count_num1--; 
//}
/****************************************************************************/
u16 Read_Dgus(u16 Dgus_Addr)
{	   	   	u16 R_Dgus=0;
			EA = 0;
			ADR_H = 0x00;
			ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0xAF;		//读操作
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			if(Dgus_Addr%2)	R_Dgus = (DATA1<<8)+DATA0; 
			else   R_Dgus=(DATA3<<8)+DATA2;
			RAMMODE = 0;
			EA = 1;
			return   R_Dgus;		
}
/****************************************************************************/
void Write_Dgus(u16 Dgus_Addr,u16 Val)
{          
    	EA = 0;
			ADR_H = 0x00;
			ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0xAF;		//读操作
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
		   	ADR_M = (u8)((Dgus_Addr/2)>>8);
			ADR_L = (u8)(Dgus_Addr/2);
			RAMMODE = 0x8F;		//写操作
		    while(!APP_ACK);
			if(Dgus_Addr%2){DATA1=(u8)(Val>>8); DATA0=(u8)(Val); }
			else { DATA3 =(u8)(Val>>8);	DATA2 =(u8)(Val);}
		    APP_EN = 1;
			while(APP_EN);
			RAMMODE = 0;
			EA = 1;
}
//======================================
/**************************************************************************   ***
 函 数 名  : void write_dgus_vp(u32 addr,u8* buf,u16 len)
 功能描述  : 写dgus地址数据
输入参数  :	 addr：写地址值	buf：写入的数据保存缓存区 len：字长度
 输出参数  : 
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 修改读写dgus流程，不使用嵌套
*****************************************************************************
void write_dgus_vp(unsigned int addr,unsigned char *buf,unsigned int len)
{
	unsigned char i;

//	EA = 0;
	i=(unsigned char)(addr&0x01);
	addr=addr/2;
	ADR_H=0x00;
	ADR_M=(unsigned char)(addr>>8);
	ADR_L=(unsigned char)(addr);
	ADR_INC=0x01;
	RAMMODE=0x8F;
	while(APP_ACK==0);
	if(i && len>0)
	{
		RAMMODE=0x83;
		DATA1=*buf++;
		DATA0=*buf++;
		APP_EN=1;
		while(APP_EN==1);
		len--;
	}
	RAMMODE=0x8F;
	while(len>=2)
	{
		DATA3=*buf++;
		DATA2=*buf++;
		DATA1=*buf++;
		DATA0=*buf++;
		APP_EN=1;
		while(APP_EN==1);
		len=len-2;
	}
	if(len)
	{
		RAMMODE=0x8C;
		DATA3=*buf++;
		DATA2=*buf++;
		APP_EN=1;
		while(APP_EN==1);
	}
	RAMMODE=0x00;
//	EA = 1;
}
/*****************************************************************************/
#if 0
void Write_Dgusii_Vp_byChar(unsigned int addr,unsigned char *buf,unsigned int len){
	unsigned char i;
//	bit EA_Bak;
//	EA_Bak = EA;
	EA = 0;
	i = (unsigned char)(addr & 0x01);
	addr = addr / 2;
	ADR_H = 0x00;
	ADR_M = (unsigned char)(addr >> 8);
	ADR_L = (unsigned char)(addr);
	ADR_INC = 0x01;
	RAMMODE = 0x8F;
	while(APP_ACK == 0);
	if(i && len > 0){
		if(len == 1){
			RAMMODE = 0x82;
			DATA1 = *buf++;
			APP_EN = 1;
			while(APP_EN == 1);
			len--;
		} else {
			RAMMODE = 0x83;
			DATA1 = *buf++;
			DATA0 = *buf++;
			APP_EN = 1;
			while(APP_EN == 1);
			len -= 2;
		}
	}
	RAMMODE = 0x8F;
	while(len >= 4){
		DATA3 = *buf++;
		DATA2 = *buf++;
		DATA1 = *buf++;
		DATA0 = *buf++;
		APP_EN = 1;
		while(APP_EN == 1);
		len -= 4;
	}
	
	if(len == 3){
		RAMMODE = 0x8E;
		DATA3 = *buf++;
		DATA2 = *buf++;
		DATA1 = *buf++;
		APP_EN = 1;
		while(APP_EN == 1);
	} else if(len == 2){
		RAMMODE = 0x8C;
		DATA3 =* buf++;
		DATA2 =* buf++;
		APP_EN = 1;
		while(APP_EN == 1);
	} else if(len == 1){
		RAMMODE = 0x88;
		DATA3 = *buf++;
		APP_EN = 1;
		while(APP_EN == 1);
	}
	RAMMODE = 0x00;
//	EA = EA_Bak;
	EA=1;
}

/*****************************************************************************
 函 数 名  : void read_dgus_vp(u32 addr,u8* buf,u16 len)
 功能描述  : 读dgus地址的值
 输入参数  : addr：dgus地址值  len：读数据长度
 输出参数  : buf：数据保存缓存区
 修改历史  :
  1.日    期   : 
    作    者   : 
    修改内容   : 修改读写dgus流程，不使用嵌套
*****************************************************************************/
void read_dgus_vp(unsigned int addr,unsigned char *buf,unsigned int len)
{
	
	unsigned char i;
	i=(unsigned char)(addr&0x01);
	EA=0;
	addr=addr/2;
	ADR_H=0x00; 
	ADR_M=(unsigned char)(addr>>8);
	ADR_L=(unsigned char)(addr);
	ADR_INC=0x01;
	RAMMODE=0xAF;
	while(APP_ACK==0);
	while(len>0)
	{
		APP_EN=1;
		while(APP_EN==1);
		if((i==0)&&(len>0))
		{
			*buf++=DATA3;
			*buf++=DATA2;
			i=1;
			len--;
		}
		if((i==1)&&(len>0))
		{
			*buf++=DATA1;
			*buf++=DATA0;
			i=0;
			len--;
		}
	}
	RAMMODE=0x00;
	EA=1;
}
#endif