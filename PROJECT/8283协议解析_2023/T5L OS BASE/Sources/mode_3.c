#include "mode_3.h"
#include "handle.h"

/************************���򻯽���ִ�к���
˵������ȡDGUS��ַ0x2560~0x2582��д�뵽NORFLAH��ַ��36�֣�		������35�� ���0x2583 0x2584
***************************************/
u8 week_check[5];
 u8 weekday;
	u8 today_date[5];
u8 rtc_chengxu[8];
u8 icon_exit_[2]={0};
u8 chengxu_run_flag=0;
u8 UE=0;
void mode_3_reset();
void chengxu_Updae()
{
	WriteNorFlash_Dgus(Nor_chengxu_Ad,0x2560,34);	
	WriteNorFlash_Dgus(Nor_chengxu_week_Ad,0x257e,6);	
	WriteNorFlash_Dgus(0x48000,0x2590,2);	
}

void chengxu_Init()		//������ʼ��
{
		ReadNorFlash_Dgus(Nor_chengxu_Ad,0x2560,34);
		ReadNorFlash_Dgus(Nor_chengxu_week_Ad,0x257e,6);
		ReadNorFlash_Dgus(0x48000,0x2590,2);	
}
	//u8 PWM_flag=0; 	//0=on 1=off

	//  u16 run_week;

#define week_ad 0x257e
void chengxu_only_first()	//������ֻ����һ�Σ���λ��������ֹͣ
{
	u8 hang=0;
	u8 i_f;
	u8 week_read[2];
	 u16 run_date;
	EA=0;
	chengxu_Updae();
	ReadDGUS(RTC_Adress,(uint8_t *)&rtc_chengxu,sizeof(rtc_chengxu));
	weekday=rtc_chengxu[3];																							//0x00~0x06	������=0x00
		UV=0;
	ReadDGUS(0x2590,(uint8_t *)&run_date,sizeof(run_date));
	for(i_f=0;i_f<=4;i_f++)
	{

		if((run_date&(0x01<<i_f))>0)		//�����
		{
				ReadDGUS(week_ad+i_f,(uint8_t *)&week_read,sizeof(week_read));
				if((week_read[1]&(0x01<<weekday))>0)		//�������
				{
					week_check[i_f]=0x1;	
				}
				else {week_check[i_f]=0;}
		}
		else hang++;
	}
	if(hang==5)Analog_click(0x02b2,0x0106);	//ģ�⸴λ	��⵽��û��һ��ѡ��
	EA=1;
}


	
	
	
	
	
void chengxu_Run()		
{
		static u8 run_t1[4];
	 static u8 run_t2[8];
	//static uint t_doing;

//	static u8 run_current[4]; //����Сʱ �������� ����on ����off
	//u16 run_date_change;
		u8 i;
	
	if(chengxu_start_1==1)
	{
		chengxu_start_1=0;
		chengxu_only_first();
	}
	ReadDGUS(RTC_Adress,(uint8_t *)&rtc_chengxu,sizeof(rtc_chengxu));
	//weekday=rtc_chengxu[3];	
	EA=0;

	if(chengxu_run_flag==1)
	{
		if(run_t2[7]!=0)
		{
			if(Draw_1s>=run_t2[5]	&&	Off_On_pwm_flag==0)		//Ҫ��Ϣ��
			{
				UV=0;Draw_1s=0;Off_On_pwm_flag=1;UE=0;
			}else if(Off_On_pwm_flag==1	&&	Draw_1s>=run_t2[7])
			{
				UV=1;Draw_1s=0;Off_On_pwm_flag=0;UE=1;
			}

		}
		else if(run_t2[7]==0)	//������
		{
				UV=1;
		}
		
					
			 if(run_t2[1]==rtc_chengxu[4]	&&	run_t2[3]==rtc_chengxu[5])	//���н���
			{
				chengxu_run_flag=0;
				UV=0;UE=0;
				mode_3_reset();
			}
		
	}
	else if(chengxu_run_flag==0)
	{

		for(i=0;i<=0x4;i++)
		{				
		if(week_check[i]==0x1)
		{	
			//�˴�������UE=1;
						ReadDGUS(0x2560+(i*0x6),(uint8_t *)&run_t1,sizeof(run_t1));	//��ȡ��ǰ�еĿ�ʼСʱ
						if(run_t1[1]==rtc_chengxu[4])			//Сʱ��δ��
						{		
						//	ReadDGUS(0x2561+(i*0x6),(uint8_t *)&run_date[3],sizeof(run_date[3]));	//��ȡ��ǰ�еĿ�ʼ����
							if(run_t1[3]==rtc_chengxu[5])
							{
								//����
									ReadDGUS(0x2562+(i*0x6),(uint8_t *)&run_t2,sizeof(run_t2));	//��ȡ��ǰ�еĽ���Сʱ ���� on off
							
									T_First=(((uint)(run_t2[1])*60+(uint)(run_t2[3]))-((uint)(run_t1[1])*60+(uint)(run_t1[3])))*60;					//ͼ�������ı���

									chengxu_run_flag=1;
									UV=1;
									UE=1;
									Draw_1s=0;
									Draw_W_Flag=1;
									First_Draw_Flag=0;										
							}
						}
						
			}
		}

	
	
}
	EA=1;
	
}



void mode_3_isr()
{
		chengxu_Run();
}
	
void mode_3_reset()	//��ǰ�׶ν���
{
	start_first_flag=0;
	chengxu_run_flag=0;
	Off_On_pwm_flag=0;
	Draw_W_Flag=0;
	First_Draw_Flag=0;
	chengxu_start_1=1;
		norflash_Close_UV_Flag=1;
	//chengxu_only_first();	
}