#ifndef __INTERACTIVE_H__
#define __INTERACTIVE_H__
#include "sys.h"

#define Nor_set_light_Ad 0x45000	//��Ļ���ȡ��������ȡ�����ʱ��洢��ַ
#define Nor_set_sudoopen_Ad	0x45100	//�����Զ�����	���ݾ��������
#define Nor_set_sudoopen_initial_Ad	0x45150	//�����Զ������ĳ�ʼ���� û�����㷨����
#define Nor_zhiliao_user_Ad 0x45160	//����ģʽ������ �û��������õ�mj����onoff��ֵ û�а��¿�ݼ�
#define Nor_shoudong_user_Ad 0x4520	//�ֶ������� �û��������õķ�������onoff��ֵ û�а��¿�ݼ�
#define Nor_P_set 0x48000 //��������Ӧ 

#define Nor_zhiliao_set_initial_1_Ad	0x44100	//���ý����� ����ģʽ��ݼ� ��Դ���ַ δ�����㷨����
#define Nor_zhiliao_set_initial_2_Ad	0x44400	
#define Nor_zhiliao_set_initial_3_Ad	0x44300	
#define Nor_zhiliao_set_Ad 0x44000	//���ý����е�����Ԥ��10byt
#define SET_zhiliao_moshi_Adress 0x3800	//��nor�����й�����ҳ��Ķ�ȡ��dgus3800�Ĵ���

	
#define Dgus_zhiliao_mode_1_virtual	0x2400	//�洢��DGUS������ģʽ��δ���������u16
#define Dgus_zhiliao_mode_2_virtual	0x2410	
#define Dgus_zhiliao_mode_3_virtual	0x2420	
	
		
#define Dgus_shezhi_zhiliao_mode_1	0X1310	//�洢��DGUS������ҳ������ģʽ�����������u16
#define Dgus_shezhi_zhiliao_mode_2	0X1450	
#define Dgus_shezhi_zhiliao_mode_3	0X1320	

void set_page_LED_write();
extern u16 shandong_LED_100ms;
#endif