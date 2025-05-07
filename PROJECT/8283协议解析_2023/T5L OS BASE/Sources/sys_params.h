#ifndef __SYS_PARAMS_H__
#define __SYS_PARAMS_H__
#include "sys.h"
#include "timer.h"

//#define SYS_PARAMS_ADDR				0
#define SYS_PARAMS_FLAG_OK		0x55555555
#define RTC_Y_M 0X9D
#define RTC_D_H 0X9E
#define RTC_M_S 0X9F
#define RTC_Adress 0x10
#define NOR_FLASH_BUFF_Reserve 0x0		/***0x00-0x04****/
#define Dgus_Buff_Reserve 0xff20			/***0xff00-0xff10***/

#define Nor_Next_Adress 0				/****0x00-0x03**�˵�ַΪ�ػ�ǰ�ĵ�ַ******/
#define Nor_Guanji_Adress 0x04				/****0x04-0x07**�˵�ַΪ�ػ�RTC��ַ*0x07-0xa0**Ϊ�ػ���ַ***/
#define Nor_24_Adress 0x000028

#define Nor_Mode 0x22
#define Nor_Last_Mode 0x24
#define Nor_zhiliao_set_Ad 0x44000	//���ý����е�����Ԥ��10byt
#define Nor_zhiliao_set_initial_1_Ad	0x44100	//���ý����� ����ģʽ��ݼ� ��Դ���ַ δ�����㷨����
#define Nor_zhiliao_set_initial_2_Ad	0x44400	
#define Nor_zhiliao_set_initial_3_Ad	0x44300	
#define Nor_chengxu_Ad 0x46000	//������������36�ֵĴ洢�ռ�
#define Nor_chengxu_week_Ad 0x47000	//�����������ڴ洢�ռ�
//#define Nor_chengxu_week_Ad 0x48000	//�����������ڴ洢�ռ�
#define Nor_set_light_Ad 0x45000	//��Ļ���ȡ��������ȡ�����ʱ��洢��ַ
#define Nor_set_sudoopen_Ad	0x45100	//�����Զ�����	���ݾ��������
#define Nor_set_sudoopen_initial_Ad	0x45150	//�����Զ������ĳ�ʼ���� û�����㷨����
#define Nor_zhiliao_user_Ad 0x45160	//����ģʽ������ �û��������õ�mj����onoff��ֵ û�а��¿�ݼ�
#define Nor_shoudong_user_Ad 0x4520	//�ֶ������� �û��������õķ�������onoff��ֵ û�а��¿�ݼ�
#define Nor_P_set 0x48000 //��������Ӧ 

void Init_Adress(void);
void norflash_Write_Open_UV(void);
void norflash_Write_guanji(void);
void norflash_Write_Close_UV(void);
void norflash_Write_Stop_UV(void);
void Uart4_Open(void);
void rtc_write(void);
void set_page_write();
void Screen_Init();
void Screen_review();
void open_sudo_Init();
void Sudoopen_set_handle();
void lianji_run();
void reset_data();
void sudo_click_start();
extern u16 Adress_Change_0;
//extern float percent;
extern u8 Screen_Light[4];
extern u16 Screen_set_Time_min;	//Ϣ��ʱ��
extern u8 Screen_Touch_flag;
//extern  u8 sudo_buff[4];//�����Զ�����
#endif