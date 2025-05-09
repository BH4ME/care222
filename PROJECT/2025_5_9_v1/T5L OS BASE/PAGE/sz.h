#ifndef __SZ_H__
#define __SZ_H__
#include "sys.h"
#include "home.h"
#include "qrj.h"
static struct{
	u8 Head_set_H;
	u8 H_mj;
	u8 H_on;
	u8 H_off;
	
	u8 Head_set_M;
	u8 M_mj;
	u8 M_on;
	u8 M_off;
	
	u8 Head_set_L;
	u8 L_mj;
	u8 L_on;
	u8 L_off;

}zhiliao_set_nor;


void SZ_INIT();
extern void SZ_Home();
void  zhiliao_SET();	//…Ë÷√“≥√Ê handle÷–
void buzzer_open_handle();//handle open
void buzzer_close_handle();//handle close
//extern void QRJ_prepare();
#endif	