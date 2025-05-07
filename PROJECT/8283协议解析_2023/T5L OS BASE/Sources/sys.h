/******************************************************************************
�� �� ��   : sys.h
�� �� ��   : V1.0
��    ��   : pinot
��������   : 2019��10��21��
��������   : ϵͳ�Ĵ����������
�޸���ʷ   :
��    ��   :
��    ��   :
�޸�����   :
******************************************************************************/

#ifndef _SYS_H_
#define _SYS_H_

/*****************************************************************************
ϵͳ��*/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "T5LOS8051.H"
#include "mode_p.h"
#include "mode_s.h"
#include  "i2c.h"
//#include "mode_4.h"


#define  uchar     unsigned char
#define  uint  unsigned int 
//#define  s16    signed int
#define  u32  unsigned long 
#define  u8   unsigned char  
//#define  s8     signed char
#define  uint  unsigned int 
//#define  s16    signed int
#define  u32  unsigned long 
/*****************************************************************************
�궨�����*/
#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#ifndef int8_t
typedef char int8_t;
#endif
//#ifndef uint323_t
//typedef unsigned long long uint323_t;
//#endif
#ifndef int16_t
typedef short int16_t;
#endif
#ifndef int32_t
typedef int int32_t;
#endif

	
#define DWIN_OK               (0x00)
#define DWIN_ERROR            (0xFF)
#define DWIN_NULL_POINT       (0x02)                /* ��ָ�� */
#define DWIN_PARAM_ERROR      (0x03)
#define NULL                  ((void *)0)           /* ����NULL */
#define FOSC                  (206438400UL)         /* T5L��Ƶ=����Ƶ��(11.0592M)*56/3 */
#define T1MS                  (65536-FOSC/12/1000)  /* 10MS��ʱ�� */ 

/*****************************************************************************
�궨�庯��*/
#define WDT_ON()  MUX_SEL|=0x02         /******�������Ź�*********/
#define WDT_OFF() MUX_SEL&=0xFD         /******�رտ��Ź�*********/
#define WDT_RST() MUX_SEL|=0x01         /******ι��*********/

/*****************************************************************************
���⺯������*/
void InitCPU(void);                    /* �Ĵ������ó�ʼ�� */
//void DelayUs(uint16_t n);
void DelayMs(uint16_t n);
sbit LED_R = P1^7;
sbit LED_G= P2^1;
sbit LED_B = P2^3;
sbit JIGUANG = P2^5;
sbit UV_St=P2^7;
sbit UV=P1^5;
extern uint L_int;
extern uint MODE;
extern  uint W_P_uJ[5];
extern  uint W_S[5];
extern u8 test_error[9];
void sys_read_vp(u16 addr,u8* buf,u16 len);
#define ZIWAIXIAN_P_Address 0x2300
//#define MODE_S 0x3000
#define ZIWAIXIAN_S_Address 0X2900
#define UART4_Control_Adress 0x3800
#define 	S_LED_0_Address	0x2000	
//#define ZIWAIXIAN_OPEN 0X01
//#define ZIWAIXIAN_CLOSE 0X00

#define ADC_08_AD 0X4300
#define ADC_09_AD 0X4400
#define ADC_10_AD 0X4500
#define W_S_Address 0x2400
#define 	TOF_Address_H	0x2500
#define 	TOF_Address_L	0x2501
#define SET_zhiliao_moshi_Adress 0x3200
extern bit OPEN_P;				
extern bit OPEN_S;	
void LED_S(void);
extern uint MODE_Static;
extern u8 UV_FLAG;
extern uint l_mode2;
#endif
