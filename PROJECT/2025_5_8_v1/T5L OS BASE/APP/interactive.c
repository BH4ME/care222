#include "interactive.h"
#include "dgus.h"
#include "norflash.h"


/**************************************
����ƵĽ������ܡ���Ļ����״̬�ơ�RGB��ɫ��״̬��

*******************************************/



/*********************״̬��*****************
˵����1����UI����ʾ����û�����ڽ��е���������ʾ�հ�
	����ʼ����������UVΪ��������ʾ�̵�
	��UV������ʾ���

	2��ָʾ�ƣ���������������ʾ�̵ƣ���UV����ʾ��� ��������

	3�����⣺������ҳ�����ý��桢���ӽ��棬�������濪ʼǰ��������
	����Ļ����򿪣���رռ���
*******************************************/




#define System_Config 0x80
//�رշ�����
void Buzzer_Close_INIT()
{
	u8 buffer_buzzer[4];
	
	ReadDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));
	buffer_buzzer[3] = buffer_buzzer[3] |  0x08;
	WriteDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));	
	
}


//���������������
void LED_Open_INIT()
{
	u8 buffer_buzzer[4];
	
	ReadDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));
	buffer_buzzer[3] = buffer_buzzer[3] |  0x04;
	WriteDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));	
	
}




#define LED_Config 0x82
//ע������������������ƣ�����Ҫ�������ʱ����С

void LED_NORFLASH_INIT()		//��ʼ��
{
	u8 Screen_Light[4];
	u16 Screen_set_Time_min;
	
	ReadNorFlash_Str(Nor_set_light_Ad,(uint8_t*)&Screen_Light,2);
	
	//����У�� 
	Screen_set_Time_min=Screen_Light[2]<<8|Screen_Light[3];	
	if(Screen_set_Time_min <= 0xffff && Screen_set_Time_min>=0x00)
	{
		
	}
	else {Screen_set_Time_min=0xffff;}	//һ����λ10ms��ffffΪ10min
	
	if(Screen_Light[0] >=0x00 && Screen_Light[0]<=0x64)
	{}
		else {Screen_Light[0]=0x64;}			//��������100%
		
	if(Screen_Light[1] >=0x00 && Screen_Light[1]<=0x64)
	{}
		else {Screen_Light[1]=0x32;}				//��������50%
		
	WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	
	WriteDGUS(0x1440,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	

}

/***********************�û�д�� ��Ļ���� �������� ����ʱ��****************/
void set_page_LED_write()	//handl set�������²�ִ��
{	
	u8 Screen_Light[4];
	//u16 Screen_set_Time_min;

	ReadDGUS(0x1440,(uint8_t *)&Screen_Light,sizeof(Screen_Light));	
	WriteDGUS(0x82,(uint8_t *)&Screen_Light,sizeof(Screen_Light));
	WriteNorFlash_Str(Nor_set_light_Ad,(uint8_t*)&Screen_Light,2);// 32λ���� 
	//Screen_set_Time_min=Screen_Light[2]<<8|Screen_Light[3];
}




//��ʼ��ֻ����һ��
void Interactive_INIT()
{
	Buzzer_Close_INIT();
	LED_Open_INIT();
	LED_NORFLASH_INIT();
	
	
	
}


//ѭ������ 
void Interactive_RUNNING()
{
	
	
}