#include "interactive.h"
#include "dgus.h"
#include "norflash.h"


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




#define System_Config 0x80
//关闭蜂鸣器
void Buzzer_Close_INIT()
{
	u8 buffer_buzzer[4];
	
	ReadDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));
	buffer_buzzer[3] = buffer_buzzer[3] |  0x08;
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




//初始化只进行一次
void Interactive_INIT()
{
	Buzzer_Close_INIT();
	LED_Open_INIT();
	LED_NORFLASH_INIT();
	
	
	
}


//循环进行 
void Interactive_RUNNING()
{
	
	
}