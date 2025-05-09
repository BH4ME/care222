#include "interactive.h"
#include "dgus.h"
#include "norflash.h"
#include "home.h"
#include "qrj.h"
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





//�رշ�����
void Buzzer_Close_INIT()
{
	u8 buffer_buzzer[4];
	u8 close_buzzer[4]={0};
	
	ReadDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));
	buffer_buzzer[3] = buffer_buzzer[3] &  0xf7;
	WriteDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));	
	
	//д��nor flash	
	WriteNorFlash_Str(Nor_Buzzer_set,(uint8_t*)&close_buzzer,2);
	WriteDGUS(0x1280,(uint8_t *)&(close_buzzer),sizeof(close_buzzer));	//д������ҳ��
	
}

void BUZZER_NORFLASH_INIT()		//��ʼ��
{
	u8 nor_buzzer[4];
	u8 buffer_buzzer[4];
	ReadNorFlash_Str(Nor_Buzzer_set,(uint8_t*)&nor_buzzer,2);
	
	ReadDGUS(System_Config,(uint8_t *)&buffer_buzzer,sizeof(buffer_buzzer));
	if(nor_buzzer[1]==1)
	{//open
		buffer_buzzer[3] = buffer_buzzer[3] |  0x08;		
		WriteDGUS(0x1280,(uint8_t *)&(nor_buzzer),sizeof(nor_buzzer));	//д������ҳ��
	}
	else if(nor_buzzer[1]==0)
	{//close
		buffer_buzzer[3] = buffer_buzzer[3] &  0xf7;
		WriteDGUS(0x1280,(uint8_t *)&(nor_buzzer),sizeof(nor_buzzer));	//д������ҳ��
	}

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

/***********************************************************************
״̬��
����״̬���������
��ͣ״̬���̵���˸
����״̬����uv���������̵�
***************************************************************************/
void RGB_JIGUANG_STATELED_INIT()
{
	
		
}



u16 shandong_LED_100ms=0;
	//�����������У�Ψһ�������һֱ��д�룬�˷��ڴ�ռ�
void RGB_JIGUANG_STATEKED_RUNNING()
{
		static bit shandong_flag=0;
	//��ҳ�棬���������ò�����
	if(CurrentPageNum[1]!=Page_home	&&CurrentPageNum[1]!=Page_lianji	&&CurrentPageNum[1]!=Page_shezhi	)
	{
//��ʼ��0x00����һ�ν������0x01������̬0x02����һ�ν������ֻ����һ�κ�δִ�����񣩣�����̬0x03��ʼ��
//����̬��ͣ0x04������̬����0x05,��λ0x06
	
		JIGUANG = 1;
		
		if(current_st == 0x03 && UV_St==0)	//��ʼ���У����ڹ���
		{
			WriteDGUS(state_icon,(uint8_t *)&(icon[2]),sizeof(icon[2]));	//����logͼ�� ���������
			LED_B=1; LED_G=0;
			
		}
		else if( current_st== 0x03 && UV_St==1)//�����£��Ʋ���
		{
			WriteDGUS(state_icon,(uint8_t *)&(icon[1]),sizeof(icon[1]));	//�̵������У���δ����
			LED_B=0; LED_G=1;
		}
		else if(current_st== 0x04 && UV_St==1)//��ͣ���ҵƲ�����
		{
			if(shandong_LED_100ms>=5)	//500ms
			{
				if(shandong_flag)
				{
					shandong_flag=~shandong_flag;
					WriteDGUS(state_icon,(uint8_t *)&(icon[1]),sizeof(icon[1]));	//�̵������У���δ����
					LED_B=0; LED_G=1;				
				}
				else
				{
					shandong_flag=~shandong_flag;					
					WriteDGUS(state_icon,(uint8_t *)&(icon[0]),sizeof(icon[0]));	//͸��
					LED_B=0; LED_G=0;
				}			
				shandong_LED_100ms=0;
			}

		}
		else //��������
		{
			WriteDGUS(state_icon,(uint8_t *)&(icon[0]),sizeof(icon[0]));	//͸��
			LED_B=0; LED_G=0;
		}		
	}
	else
	{JIGUANG=0;}

}


//��ʼ��ֻ����һ��
void Interactive_INIT()
{
	//Buzzer_Close_INIT();	//�رշ���������
	LED_Open_INIT();			//�򿪱����������
	BUZZER_NORFLASH_INIT();	//��ʼ��������	
	LED_NORFLASH_INIT();	//��ʼ������
	RGB_JIGUANG_STATELED_INIT();//��ʼ��״̬�ƣ����⣬ҳ��״̬��
	
	
}


//ѭ������ 
void Interactive_RUNNING()
{
	
	
}