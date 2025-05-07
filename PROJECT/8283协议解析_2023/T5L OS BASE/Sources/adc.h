#ifndef __ADC_H__
#define __ADC_H__
#include "sys.h"


enum {
	ADC_CHANNEL0 = 0x00,		/*****P8*****/
	ADC_CHANNEL1=0x01,
	ADC_CHANNEL2=0x02,
	ADC_CHANNEL3,
	ADC_CHANNEL4,
	ADC_CHANNEL5,
	ADC_CHANNEL6,
	ADC_CHANNEL7
};

//º¯ÊýÉêÃ÷
u16 adc_read(u16 channel);
u16 adc_read_avg(u16 channel,u16 avg_cnt);
extern u16 ad;
extern	float adc_10;
	extern	float adc_09;
	extern 		float adc_08;

#endif


