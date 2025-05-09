#include "interactive.h"
#include "dgus.h"
#include "norflash.h"
#include "home.h"
#include "qrj.h"
/**************************************
激光灯的交互功能、屏幕工作状态灯、RGB三色灯状态灯

*******************************************/



/*********************状态灯*****************
说明：1）在UI上显示，若没有正在进行的任务则显示空白
	若开始启动程序但是UV为开，即显示绿点
	若UV关则显示红点

	2）指示灯：若启动程序则显示绿灯，若UV开显示红灯 两个兼容

	3）激光：除了首页、设置界面、连接界面，其他界面开始前都开启，
	若屏幕背光打开，则关闭激光
*******************************************/





//关闭蜂鸣器
void Buzzer_Close_INIT()
{
	u8 buffer_buzzer[4];
	u8 close_buzzer[4]={0};
	
	ReadDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));
	buffer_buzzer[3] = buffer_buzzer[3] &  0xf7;
	WriteDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));	
	
	//写入nor flash	
	WriteNorFlash_Str(Nor_Buzzer_set,(uint8_t*)&close_buzzer,2);
	WriteDGUS(0x1280,(uint8_t *)&(close_buzzer),sizeof(close_buzzer));	//写入设置页面
	
}

void BUZZER_NORFLASH_INIT()		//初始化
{
	u8 nor_buzzer[4];
	u8 buffer_buzzer[4];
	ReadNorFlash_Str(Nor_Buzzer_set,(uint8_t*)&nor_buzzer,2);
	
	ReadDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));
	if(nor_buzzer[1]==1)
	{//open
		buffer_buzzer[3] = buffer_buzzer[3] |  0x08;		
		WriteDGUS(0x1280,(uint8_t *)&(nor_buzzer),sizeof(nor_buzzer));	//写入设置页面
	}
	else if(nor_buzzer[1]==0)
	{//close
		buffer_buzzer[3] = buffer_buzzer[3] &  0xf7;
		WriteDGUS(0x1280,(uint8_t *)&(nor_buzzer),sizeof(nor_buzzer));	//写入设置页面
	}

	WriteDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));	
	
	
}



//开启背光待机控制
void LED_Open_INIT()
{
	u8 buffer_buzzer[4];
	
	ReadDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));
	buffer_buzzer[3] = buffer_buzzer[3] |  0x04;
	WriteDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));	
	
}




#define LED_Config 0x82
//注意若开启背光待机控制，则不需要软件控制时长大小

void LED_NORFLASH_INIT()		//初始化
{
	u8 Screen_Light[4];
	u16 Screen_set_Time_min;
	
	ReadNorFlash_Str(Nor_set_light_Ad,(uint8_t*)&Screen_Light,2);
	
	//数据校验 
	Screen_set_Time_min=Screen_Light[2]<<8|Screen_Light[3];	
	if(Screen_set_Time_min <= 0xffff && Screen_set_Time_min>=0x00)
	{
		
	}
	else {Screen_set_Time_min=0xffff;}	//一个单位10ms，ffff为10min
	
	if(Screen_Light[0] >=0x00 && Screen_Light[0]<=0x64)
	{}
		else {Screen_Light[0]=0x64;}			//工作亮度100%
		
	if(Screen_Light[1] >=0x00 && Screen_Light[1]<=0x64)
	{}
		else {Screen_Light[1]=0x32;}				//待机亮度50%
		
	WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	
	WriteDGUS(0x1440,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	

}

/***********************用户写入 屏幕亮度 待机亮度 待机时间****************/
void set_page_LED_write()	//handl set界面摁下才执行
{	
	u8 Screen_Light[4];
	//u16 Screen_set_Time_min;

	ReadDGUS(0x1440,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	
	WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));
	WriteNorFlash_Str(Nor_set_light_Ad,(uint8_t*)&Screen_Light,2);// 32位长度 
	//Screen_set_Time_min=Screen_Light[2]<<8|Screen_Light[3];
}

/***********************************************************************
状态灯
运行状态，灯亮红灯
暂停状态，绿灯闪烁
运行状态，但uv不亮，亮绿灯
***************************************************************************/
void RGB_JIGUANG_STATELED_INIT()
{
	
		
}



u16 shandong_LED_100ms=0;
	//放在主程序中，唯一问题就是一直在写入，浪费内存空间
void RGB_JIGUANG_STATEKED_RUNNING()
{
		static bit shandong_flag=0;
	//主页面，联机，设置不运行
	if(CurrentPageNum[1]!=Page_home	&&CurrentPageNum[1]!=Page_lianji	&&CurrentPageNum[1]!=Page_shezhi	)
	{
//初始化0x00，第一次进入界面0x01，就绪态0x02（第一次进入界面只运行一次后，未执行任务），运行态0x03开始，
//运行态暂停0x04，运行态结束0x05,复位0x06
	
		JIGUANG = 1;
		
		if(current_st == 0x03 && UV_St==0)	//开始运行，灯在工作
		{
			WriteDGUS(state_icon,(uint8_t *)&(icon[2]),sizeof(icon[2]));	//出现log图标 红灯照射中
			LED_B=1; LED_G=0;
			
		}
		else if( current_st== 0x03 && UV_St==1)//运行下，灯不亮
		{
			WriteDGUS(state_icon,(uint8_t *)&(icon[1]),sizeof(icon[1]));	//绿灯运行中，但未照射
			LED_B=0; LED_G=1;
		}
		else if(current_st== 0x04 && UV_St==1)//暂停下且灯不工作
		{
			if(shandong_LED_100ms>=5)	//500ms
			{
				if(shandong_flag)
				{
					shandong_flag=~shandong_flag;
					WriteDGUS(state_icon,(uint8_t *)&(icon[1]),sizeof(icon[1]));	//绿灯运行中，但未照射
					LED_B=0; LED_G=1;				
				}
				else
				{
					shandong_flag=~shandong_flag;					
					WriteDGUS(state_icon,(uint8_t *)&(icon[0]),sizeof(icon[0]));	//透明
					LED_B=0; LED_G=0;
				}			
				shandong_LED_100ms=0;
			}

		}
		else //除此自外
		{
			WriteDGUS(state_icon,(uint8_t *)&(icon[0]),sizeof(icon[0]));	//透明
			LED_B=0; LED_G=0;
		}		
	}
	else
	{JIGUANG=0;}

}


//初始化只进行一次
void Interactive_INIT()
{
	//Buzzer_Close_INIT();	//关闭蜂鸣器配置
	LED_Open_INIT();			//打开背光控制配置
	BUZZER_NORFLASH_INIT();	//初始化蜂鸣器	
	LED_NORFLASH_INIT();	//初始化背光
	RGB_JIGUANG_STATELED_INIT();//初始化状态灯，激光，页面状态灯
	
	
}


//循环进行 
void Interactive_RUNNING()
{
	
	
}